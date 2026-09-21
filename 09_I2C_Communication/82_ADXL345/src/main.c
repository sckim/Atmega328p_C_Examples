/*=======================================================*/
// ADXL345 : 3축 가속도 센서를 TWI 레지스터로 직접 읽는다
//
// 01_Arduino_Examples/09_I2C_Communication/80_ADXL345 의 AVR C 버전이다.
// (02 의 80_ADXL_ITG 는 ADXL345 + ITG3200 을 함께 쓰는 옛 통합 프로젝트이다)
//
// ADXL345 (7비트 주소 0x53, ALT ADDRESS 핀이 GND 일 때)
//   0x00 DEVID       : 항상 0xE5
//   0x2D POWER_CTL   : bit3(Measure)=1 이면 측정 시작 (리셋 후에는 대기 모드)
//   0x31 DATA_FORMAT : bit3(FULL_RES)=1 이면 전 범위에서 해상도 4mg/LSB 유지, bit1:0 = 범위(00 = ±2g)
//   0x32~0x37 DATAX0~DATAZ1 : X, Y, Z 각각 16비트(하위 바이트가 먼저, little-endian)
//
// 6바이트를 한 번에 읽는 이유 : 각 축의 상·하위 바이트가 서로 다른 시점의 값이 되는 것을 막기 위해서다.
//
// 연결 : SDA = PC4, SCL = PC5, VCC = 3.3V (모듈에 레귤레이터가 있으면 5V 도 가능)
// 결과 : UART(9600bps)로 X, Y, Z 를 mg 단위로 출력
//
// 선행 학습 : 20_I2C_write      다음 단계 : 70_MAX30105
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
        twi_start() == TW_REP_START &&
        twi_write((addr << 1) | TW_READ) == TW_MR_SLA_ACK) {
        for (uint8_t i = 0; i < n; i++)
            buf[i] = twi_read(i < n - 1);
        ok = true;
    }
    twi_stop();
    return ok;
}

#define ADXL345_ADDR        0x53
#define ADXL345_DEVID       0x00
#define ADXL345_POWER_CTL   0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0      0x32

int main(void)
{
    uart_init();
    twi_init();

    uint8_t id = 0;
    if (!i2c_read_regs(ADXL345_ADDR, ADXL345_DEVID, &id, 1) || id != 0xE5) {
        PRINT("ADXL345 를 찾지 못했다 (DEVID = 0x%02X, 정상값 0xE5)\n", id);
        while (1)
            ;
    }
    PRINT("ADXL345 발견 (DEVID = 0x%02X)\n", id);

    i2c_write_reg(ADXL345_ADDR, ADXL345_DATA_FORMAT, 0x08);   // FULL_RES, ±2g
    i2c_write_reg(ADXL345_ADDR, ADXL345_POWER_CTL, 0x08);     // 측정 시작

    while (1) {
        uint8_t raw[6];
        if (i2c_read_regs(ADXL345_ADDR, ADXL345_DATAX0, raw, 6)) {
            int16_t x = (int16_t)((raw[1] << 8) | raw[0]);
            int16_t y = (int16_t)((raw[3] << 8) | raw[2]);
            int16_t z = (int16_t)((raw[5] << 8) | raw[4]);
            // FULL_RES 에서 1 LSB = 3.9mg  ->  mg = raw x 39 / 10
            PRINT("X=%6d mg  Y=%6d mg  Z=%6d mg\n", (int)((int32_t)x * 39 / 10), (int)((int32_t)y * 39 / 10), (int)((int32_t)z * 39 / 10));
        } else {
            PRINT("읽기 실패\n");
        }
        _delay_ms(200);
    }
}
