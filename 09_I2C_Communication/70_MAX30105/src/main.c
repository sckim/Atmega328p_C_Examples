/*=======================================================*/
// MAX30105 : 입자·맥박 센서(MAX30105 / MAX30102 호환)의 Red·IR 값을 TWI 로 직접 읽는다
//
// 01_Arduino_Examples/09_I2C_Communication/70_MAX30105 (SparkFun 라이브러리 2500줄)의
// "Example1_Basic_Readings" 에 해당하는 동작을 레지스터 수준의 최소 코드로 구현했다.
//
// MAX30105 (7비트 주소 0x57)
//   0xFF PART_ID   : 0x15
//   0x09 MODE_CONFIG  : bit6 = RESET, bit2:0 = 모드 (010 = Red, 011 = Red+IR, 111 = 다중 LED)
//   0x08 FIFO_CONFIG  : bit7:5 = 평균 샘플 수, bit4 = FIFO 롤오버
//   0x0A SPO2_CONFIG  : bit6:5 = ADC 범위, bit4:2 = 샘플 속도, bit1:0 = 펄스 폭(= ADC 해상도)
//   0x0C / 0x0D       : Red / IR LED 전류
//   0x04 / 0x06       : FIFO 쓰기/읽기 포인터 (32 칸의 원형 큐)
//   0x07 FIFO_DATA    : 샘플 1개 = Red 3바이트 + IR 3바이트 (18비트 값, 상위 바이트가 먼저)
//   0x1F / 0x20       : 칩 내부 온도 정수부/소수부(1/16 도), 측정 시작은 0x21 <- 1
//
// 순서 : 리셋 -> 설정 -> 반복(FIFO 에 쌓인 샘플 수 = (WR - RD) & 0x1F, 그 수만큼 읽기)
//
// 연결 : SDA = PC4, SCL = PC5, VIN = 3.3V   결과 : UART(9600bps)
// 손가락을 센서 위에 올리면 IR 값이 크게 올라간다.
//
// 선행 학습 : 82_ADXL345
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

/* ---- MAX30105 ---- */
#define MAX_ADDR        0x57
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA   0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA     0x0C     // Red
#define REG_LED2_PA     0x0D     // IR
#define REG_TEMP_INT    0x1F
#define REG_TEMP_FRAC   0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PART_ID     0xFF

static bool max_init(void)
{
    uint8_t id = 0;
    if (!i2c_read_regs(MAX_ADDR, REG_PART_ID, &id, 1) || id != 0x15)
        return false;

    i2c_write_reg(MAX_ADDR, REG_MODE_CONFIG, 0x40);          // 소프트 리셋
    uint8_t mode = 0x40;
    for (uint8_t i = 0; i < 100 && (mode & 0x40); i++) {     // RESET 비트가 0 이 될 때까지
        _delay_ms(1);
        i2c_read_regs(MAX_ADDR, REG_MODE_CONFIG, &mode, 1);
    }

    i2c_write_reg(MAX_ADDR, REG_FIFO_CONFIG, 0x50);          // 4개 평균, FIFO 롤오버 허용
    i2c_write_reg(MAX_ADDR, REG_SPO2_CONFIG, 0x27);          // ADC 4096nA, 100sps, 411us(18비트)
    i2c_write_reg(MAX_ADDR, REG_LED1_PA, 0x1F);              // Red 약 6.4mA
    i2c_write_reg(MAX_ADDR, REG_LED2_PA, 0x1F);              // IR  약 6.4mA
    i2c_write_reg(MAX_ADDR, REG_FIFO_WR_PTR, 0);             // FIFO 포인터 초기화
    i2c_write_reg(MAX_ADDR, REG_OVF_COUNTER, 0);
    i2c_write_reg(MAX_ADDR, REG_FIFO_RD_PTR, 0);
    i2c_write_reg(MAX_ADDR, REG_MODE_CONFIG, 0x03);          // Red + IR 모드 시작
    return true;
}

static void max_print_temperature(void)                      // 칩 내부 온도 (주변 온도 근사)
{
    uint8_t t[2];
    i2c_write_reg(MAX_ADDR, REG_TEMP_CONFIG, 0x01);          // 측정 시작 (끝나면 비트가 0 이 된다)
    _delay_ms(100);
    if (i2c_read_regs(MAX_ADDR, REG_TEMP_INT, t, 2)) {
        int8_t whole = (int8_t)t[0];
        uint8_t hundredths = (uint8_t)((uint16_t)(t[1] & 0x0F) * 625 / 100);   // 1/16 = 0.0625
        PRINT("칩 온도 = %d.%02u C\n", whole, hundredths);
    }
}

int main(void)
{
    uart_init();
    twi_init();

    if (!max_init()) {
        PRINT("MAX30105 를 찾지 못했다 (주소 0x%02X, PART_ID 0x15)\n", MAX_ADDR);
        while (1)
            ;
    }
    PRINT("MAX30105 초기화 완료\n");
    max_print_temperature();

    while (1) {
        uint8_t ptr[2];
        // WR 과 RD 포인터를 연속으로 읽을 수 없으므로 각각 읽는다
        i2c_read_regs(MAX_ADDR, REG_FIFO_WR_PTR, &ptr[0], 1);
        i2c_read_regs(MAX_ADDR, REG_FIFO_RD_PTR, &ptr[1], 1);
        uint8_t samples = (ptr[0] - ptr[1]) & 0x1F;            // 큐에 쌓인 샘플 수

        while (samples--) {
            uint8_t d[6];
            if (i2c_read_regs(MAX_ADDR, REG_FIFO_DATA, d, 6)) {
                uint32_t red = ((uint32_t)(d[0] & 0x03) << 16) | ((uint32_t)d[1] << 8) | d[2];   // 18비트
                uint32_t ir  = ((uint32_t)(d[3] & 0x03) << 16) | ((uint32_t)d[4] << 8) | d[5];
                PRINT("R=%lu IR=%lu%s\n", (unsigned long)red, (unsigned long)ir, ir < 50000UL ? "  (손가락 없음?)" : "");
            }
        }
        _delay_ms(40);                                          // 100sps/4 = 25샘플/초 -> 40ms 마다 1개
    }
}
