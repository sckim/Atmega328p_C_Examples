/*=======================================================*/
// LED_bar : 가변저항(ADC0) 값을 LED 8개의 막대 그래프로 표시
//
// 01_Arduino_Examples/01_Digital_IO/30_LED_bar 의 AVR C 버전이다.
//
// 연결 (LED 는 저항을 거쳐 GND 로, 즉 High 에서 켜진다)
//   LED0 ~ LED1 : PB0, PB1   (아두이노 핀 8, 9)
//   LED2 ~ LED7 : PD2 ~ PD7  (아두이노 핀 2 ~ 7)
//   가변저항    : ADC0 (PC0)
//
// LED 번호 i 가 PORTB/PORTD 의 "i번 비트"와 일치하도록 배선했다.
// 그래서 막대 값 하나(0~8)를 8비트 마스크로 만든 뒤, 두 포트에 나누어 쓰면 된다.
//   level = 3  ->  bar = 0b00000111
//   PB0,PB1 <- bar 의 0,1 번 비트 / PD2~PD7 <- bar 의 2~7 번 비트
// (PD0, PD1 은 UART 핀이므로 마스크로 보호한다)
//
// 선행 학습 : 10_Blink      다음 단계 : 04_ADC
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
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

static void adc_init(void)
{
    ADMUX = (1 << REFS0);                                        // 기준전압 AVcc
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // ADC 허용, 16MHz/128 = 125kHz
}

static uint16_t adc_read(uint8_t ch)
{
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);       // 채널 선택
    ADCSRA |= (1 << ADSC);                      // 변환 시작
    while (ADCSRA & (1 << ADSC))                // 변환이 끝나면 ADSC 가 0 이 된다
        ;
    return ADC;
}

static void show_bar(uint8_t level)             // level : 0 ~ 8
{
    uint8_t bar = (uint8_t)((1u << level) - 1);           // 하위 level 비트만 1
    PORTB = (PORTB & ~0x03) | (bar & 0x03);               // LED0, LED1
    PORTD = (PORTD & ~0xFC) | (bar & 0xFC);               // LED2 ~ LED7
}

int main(void)
{
    DDRB |= 0x03;               // PB0, PB1 출력
    DDRD |= 0xFC;               // PD2 ~ PD7 출력
    adc_init();
    uart_init();

    uint8_t prev = 0xFF;
    while (1) {
        uint16_t v = adc_read(0);
        uint8_t level = (uint8_t)((uint32_t)v * 9 / 1024);    // 0..1023 -> 0..8
        if (level != prev) {                                  // 값이 바뀔 때만 갱신
            show_bar(level);
            PRINT("Voltage level = %u\n", level);
            prev = level;
        }
    }
}
