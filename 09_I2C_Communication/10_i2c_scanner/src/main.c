/*=======================================================*/
// i2c_scanner : I2C(TWI) 버스에 연결된 장치의 7비트 주소를 모두 찾는다
//
// 01_Arduino_Examples/09_I2C_Communication/10_i2c_scanner 의 AVR C 버전이다.
//
// 원리 : 주소 1~126 에 대해 "START + (주소<<1 | W)" 를 보내 보고,
//        슬레이브가 ACK 를 돌려주면(TWSR 상태 코드 0x18, TW_MT_SLA_ACK) 그 주소에 장치가 있는 것이다.
//        NACK(0x20)이면 장치가 없다.
//
// 연결 : SDA = PC4(A4), SCL = PC5(A5), 각각 4.7kΩ 으로 5V 풀업 (모듈에 내장된 경우가 많다)
// 결과 : UART(9600bps)로 출력. 5초마다 다시 검색한다.
//
// TWI 레지스터
//   TWBR/TWSR.TWPS : SCL 주파수   SCL = F_CPU / (16 + 2 x TWBR x 4^TWPS)
//   TWCR : TWINT 완료 플래그(1을 써서 지운다), TWSTA START, TWSTO STOP, TWEA ACK, TWEN 허용
//   TWSR : 상위 5비트가 상태 코드     TWDR : 주소 또는 데이터
//
// 다음 단계 : 20_I2C_write
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
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

/* ---- TWI 마스터 ---- */
#define TWI_ERROR 0xFF

static void twi_init(void)
{
    TWSR = 0;                                   // 분주 1
    TWBR = (F_CPU / 100000UL - 16) / 2;         // SCL = 100kHz -> TWBR = 72
    TWCR = (1 << TWEN);
}

static uint8_t twi_wait(void)                   // TWINT 를 기다린다. 응답이 없으면 포기(타임아웃)
{
    uint16_t timeout = 0xFFFF;
    while (!(TWCR & (1 << TWINT)))
        if (--timeout == 0)
            return TWI_ERROR;
    return TW_STATUS;
}

static uint8_t twi_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    return twi_wait();
}

static void twi_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    uint16_t timeout = 0xFFFF;
    while ((TWCR & (1 << TWSTO)) && --timeout)  // STOP 이 실제로 전송될 때까지
        ;
}

static uint8_t twi_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    return twi_wait();
}

int main(void)
{
    uart_init();
    twi_init();
    PRINT("\nI2C Scanner\n");

    while (1) {
        uint8_t found = 0;
        PRINT("Scanning...\n");
        for (uint8_t addr = 1; addr < 127; addr++) {
            uint8_t st = twi_start();
            if (st == TW_START || st == TW_REP_START) {
                st = twi_write((addr << 1) | TW_WRITE);     // SLA+W
                if (st == TW_MT_SLA_ACK) {                  // 0x18 : 장치가 ACK 했다
                    PRINT("I2C device found at address 0x%02X\n", addr);
                    found++;
                }
            }
            twi_stop();
        }
        if (found == 0)
            PRINT("No I2C devices found\n");
        else
            PRINT("done (%u device%s)\n", found, found > 1 ? "s" : "");
        _delay_ms(5000);
    }
}
