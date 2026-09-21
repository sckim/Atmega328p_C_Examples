/*=======================================================*/
// eeprom_24c02 : 외부 I2C EEPROM(24C02, 256바이트)에 쓰고 읽기
//
// 01_Arduino_Examples/12_EEPROM/30_eeprom_24c02 의 AVR C 버전이다.
// (ATmega328P 내장 EEPROM 은 20_eeprom_write)
//
// 24C02 (7비트 주소 0x50 ~ 0x57, A2 A1 A0 를 모두 GND 에 연결하면 0x50)
//   바이트 쓰기 : START -> SLA+W -> 워드 주소 -> 데이터 -> STOP
//   바이트 읽기 : START -> SLA+W -> 워드 주소 -> REPEATED START -> SLA+R -> 데이터(NACK) -> STOP
//   쓰기 사이클(약 5ms) : STOP 후 칩이 내부에서 기록하는 동안에는 SLA+W 에 응답하지 않는다.
//                         그래서 ACK 가 올 때까지 SLA+W 를 반복해서 보낸다 (ACK polling).
//                         01 예제는 이 시간을 delay 로 통째로 기다린다.
//
// 동작 : 주소 1 부터 5개에 값 1..5 를 쓰고, 읽어서 결과를 출력한다.
// 연결 : SDA = PC4, SCL = PC5, WP 는 GND
//
// 선행 학습 : 20_I2C_write, 20_eeprom_write
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
    TWBR = (F_CPU / 100000UL - 16) / 2;
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

static uint8_t twi_read(bool ack)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);
    twi_wait();
    return TWDR;
}

/* ---- 24C02 ---- */
#define EEPROM_ADDR 0x50

static bool device_write(uint8_t wordaddr, uint8_t data)
{
    bool ok = false;
    if (twi_start() == TW_START &&
        twi_write((EEPROM_ADDR << 1) | TW_WRITE) == TW_MT_SLA_ACK &&
        twi_write(wordaddr) == TW_MT_DATA_ACK &&
        twi_write(data) == TW_MT_DATA_ACK)
        ok = true;
    twi_stop();
    if (!ok)
        return false;

    // ACK polling : 내부 쓰기 사이클(최대 5ms)이 끝나 SLA+W 에 ACK 할 때까지 반복
    for (uint8_t i = 0; i < 100; i++) {
        bool ready = (twi_start() == TW_START) &&
                     (twi_write((EEPROM_ADDR << 1) | TW_WRITE) == TW_MT_SLA_ACK);
        twi_stop();
        if (ready)
            return true;
        _delay_us(100);
    }
    return false;
}

static bool device_read(uint8_t wordaddr, uint8_t *data)
{
    bool ok = false;
    if (twi_start() == TW_START &&
        twi_write((EEPROM_ADDR << 1) | TW_WRITE) == TW_MT_SLA_ACK &&
        twi_write(wordaddr) == TW_MT_DATA_ACK &&
        twi_start() == TW_REP_START &&
        twi_write((EEPROM_ADDR << 1) | TW_READ) == TW_MR_SLA_ACK) {
        *data = twi_read(false);            // 1바이트만 읽고 NACK
        ok = true;
    }
    twi_stop();
    return ok;
}

int main(void)
{
    uart_init();
    twi_init();
    _delay_ms(500);

    uint8_t data = 0x01;
    uint8_t wordaddress = 0x01;

    for (uint8_t i = 0; i < 5; i++) {
        PRINT("Write data: %X to: %X\n", data, wordaddress);
        if (!device_write(wordaddress, data))
            PRINT("  -> 쓰기 실패 (24C02 연결과 주소를 확인)\n");
        wordaddress++;
        data++;
    }

    PRINT("\n");
    wordaddress = 0x01;
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t v = 0xFF;
        if (device_read(wordaddress, &v))
            PRINT("Read data: %X from: %X\n", v, wordaddress);
        else
            PRINT("  -> 읽기 실패\n");
        wordaddress++;
    }

    while (1)
        ;
}
