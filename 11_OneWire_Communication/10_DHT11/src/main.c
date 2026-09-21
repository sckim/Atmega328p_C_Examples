/*=======================================================*/
// DHT11 : 온습도 센서의 단선(single-wire) 프로토콜을 GPIO 로 직접 구현한다
//
// 01_Arduino_Examples/11_OneWire_Communication/10_DHT11 의 AVR C 버전이다.
// (01 은 라이브러리를 쓰고 이슬점 등을 float 로 계산한다. 여기서는 정수만 쓴다)
//
// 연결 : DHT11 DATA 를 PD2 에 연결하고 4.7~10kΩ 으로 5V 풀업 (모듈에는 보통 내장)
//
// 통신 순서 (DHT11 데이터시트)
//   1. MCU 가 DATA 를 18ms 이상 Low 로 내려 시작을 알린다
//   2. MCU 가 DATA 를 놓으면(High) 20~40us 뒤 DHT11 이 Low 80us, High 80us 로 응답한다
//   3. 데이터 40비트 : 각 비트는 Low 약 50us 다음에 High 가 이어지는데
//        High 가 약 27us 이면 '0',  약 70us 이면 '1'
//   4. 40비트 = 습도 정수, 습도 소수, 온도 정수, 온도 소수, 체크섬(앞 4바이트의 합)
//
// 타이밍이 us 단위이므로 읽는 동안에는 인터럽트를 막는다 (cli/sei).
// High 의 길이는 "1us 지연 + 핀 읽기"를 반복한 횟수로 잰다.
//   - 27us 는 약 20회, 70us 는 약 50회 -> 35회를 기준으로 0/1 을 판별한다
// DHT11 은 1초에 1번보다 자주 읽으면 안 된다 (여기서는 2초 간격).
//
// 결과 : UART(9600bps)
//
// 선행 학습 : 10_Blink, 20_Button, 12_Timer_Overflow      다음 단계 : 20_DS18B20
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
    return 0;
}
static FILE uart_out = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static void uart_init(void)
{
    UBRR0 = F_CPU / 16 / 9600 - 1;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart_out;
}
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

#define DHT_PIN PD2
#define DHT_OK              0
#define DHT_ERROR_CHECKSUM  1
#define DHT_ERROR_TIMEOUT   2

// 핀이 level 인 동안 기다리며 지속 시간을 센다. 제한(limit)을 넘으면 0 을 돌려준다.
static uint8_t wait_while(uint8_t level, uint8_t limit)
{
    uint8_t count = 0;
    while (((PIND >> DHT_PIN) & 1) == level) {
        _delay_us(1);
        if (++count >= limit)
            return 0;
    }
    return count ? count : 1;
}

static uint8_t dht11_read(uint8_t *humidity, uint8_t *temperature)
{
    uint8_t data[5] = {0, 0, 0, 0, 0};

    // 1. 시작 신호 : 18ms 이상 Low
    DDRD |= (1 << DHT_PIN);
    PORTD &= ~(1 << DHT_PIN);
    _delay_ms(20);

    cli();                                              // 이후 us 단위 타이밍을 지켜야 한다
    // 2. 버스를 놓는다 : 입력 + 풀업
    PORTD |= (1 << DHT_PIN);
    DDRD &= ~(1 << DHT_PIN);
    _delay_us(10);

    // DHT11 의 응답 : (High) -> Low 80us -> High 80us
    if (!wait_while(1, 100)) { sei(); return DHT_ERROR_TIMEOUT; }   // DHT11 이 Low 로 내릴 때까지
    if (!wait_while(0, 100)) { sei(); return DHT_ERROR_TIMEOUT; }   // Low 80us
    if (!wait_while(1, 100)) { sei(); return DHT_ERROR_TIMEOUT; }   // High 80us

    // 3. 40비트 수신
    for (uint8_t i = 0; i < 40; i++) {
        if (!wait_while(0, 100)) { sei(); return DHT_ERROR_TIMEOUT; }   // 비트 사이의 Low 50us
        uint8_t high = wait_while(1, 120);                              // High 의 길이
        if (!high) { sei(); return DHT_ERROR_TIMEOUT; }
        data[i / 8] <<= 1;
        if (high > 35)                                                  // 길면 '1'
            data[i / 8] |= 1;
    }
    sei();

    // 4. 체크섬
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return DHT_ERROR_CHECKSUM;

    *humidity = data[0];
    *temperature = data[2];
    return DHT_OK;
}

int main(void)
{
    uart_init();
    PRINT("DHT11 TEST PROGRAM\n");

    while (1) {
        uint8_t hum = 0, temp = 0;
        uint8_t chk = dht11_read(&hum, &temp);

        PRINT("\nRead sensor: ");
        switch (chk) {
        case DHT_OK:
            PRINT("OK\n");
            PRINT("Humidity (%%): %u\n", hum);
            PRINT("Temperature (C): %u\n", temp);
            PRINT("Temperature (F): %u\n", (uint16_t)temp * 9 / 5 + 32);   // 정수 근사
            PRINT("Temperature (K): %u\n", temp + 273);
            break;
        case DHT_ERROR_CHECKSUM:
            PRINT("Checksum error\n");
            break;
        case DHT_ERROR_TIMEOUT:
            PRINT("Time out error\n");
            break;
        default:
            PRINT("Unknown error\n");
            break;
        }
        _delay_ms(2000);
    }
}
