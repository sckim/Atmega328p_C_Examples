/*=======================================================*/
// I2C_write : I2C 장치(초음파 거리센서 SRF10/SRF08)에 명령을 쓰고 결과를 읽는다
//
// 01_Arduino_Examples/09_I2C_Communication/20_I2C_write 의 AVR C 버전이다.
//
// SRF10 (7비트 주소 0x70, 데이터시트의 8비트 주소는 0xE0)
//   레지스터 0 : 명령(쓰기)   0x50 = 인치, 0x51 = 센티미터, 0x52 = 마이크로초 단위로 측정
//   레지스터 2, 3 : 에코 결과 상위·하위 바이트(읽기)
//
// I2C 의 "레지스터 쓰기" 순서
//   START -> SLA+W -> 레지스터 번호 -> 데이터 -> STOP
// "레지스터 읽기" 순서 (반복 시작 조건 사용)
//   START -> SLA+W -> 레지스터 번호 -> REPEATED START -> SLA+R -> 데이터(마지막에 NACK) -> STOP
//
// 연결 : SDA = PC4, SCL = PC5 (풀업 필요)   결과 : UART(9600bps)
//
// 선행 학습 : 10_i2c_scanner      다음 단계 : 30_LCD_I2C
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>
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
    TWSR = 0;
    TWBR = (F_CPU / 100000UL - 16) / 2;         // 100kHz
    TWCR = (1 << TWEN);
}

static uint8_t twi_wait(void)
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
    while ((TWCR & (1 << TWSTO)) && --timeout)
        ;
}

static uint8_t twi_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    return twi_wait();
}

static uint8_t twi_read(bool ack)               // ack = true : 계속 읽겠다, false : 마지막 바이트(NACK)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);
    twi_wait();
    return TWDR;
}

/* ---- 레지스터 단위 읽기/쓰기 ---- */
static bool i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t value)
{
    bool ok = false;
    if (twi_start() == TW_START &&
        twi_write((addr << 1) | TW_WRITE) == TW_MT_SLA_ACK &&
        twi_write(reg) == TW_MT_DATA_ACK &&
        twi_write(value) == TW_MT_DATA_ACK)
        ok = true;
    twi_stop();
    return ok;
}

static bool i2c_read_regs(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t n)
{
    bool ok = false;
    if (twi_start() == TW_START &&
        twi_write((addr << 1) | TW_WRITE) == TW_MT_SLA_ACK &&
        twi_write(reg) == TW_MT_DATA_ACK &&
        twi_start() == TW_REP_START &&                          // 반복 시작 조건
        twi_write((addr << 1) | TW_READ) == TW_MR_SLA_ACK) {
        for (uint8_t i = 0; i < n; i++)
            buf[i] = twi_read(i < n - 1);                       // 마지막 바이트만 NACK
        ok = true;
    }
    twi_stop();
    return ok;
}

#define SRF10_ADDR   0x70
#define SRF10_CMD    0x00
#define SRF10_RANGE  0x02

int main(void)
{
    uart_init();
    twi_init();

    while (1) {
        // 1단계 : 센서에 "인치 단위로 측정하라"고 명령한다
        if (!i2c_write_reg(SRF10_ADDR, SRF10_CMD, 0x50)) {
            PRINT("SRF10 응답 없음 (주소 0x%02X)\n", SRF10_ADDR);
            _delay_ms(1000);
            continue;
        }

        // 2단계 : 측정이 끝나기를 기다린다 (데이터시트 : 65ms 이상)
        _delay_ms(70);

        // 3단계 : 결과(레지스터 2, 3)를 읽는다
        uint8_t buf[2];
        if (i2c_read_regs(SRF10_ADDR, SRF10_RANGE, buf, 2)) {
            uint16_t reading = ((uint16_t)buf[0] << 8) | buf[1];
            PRINT("range = %u inches\n", reading);
        }
        _delay_ms(100);
    }
}
