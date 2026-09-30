/*=======================================================*/
// DS18B20_Offline : 센서 없이 스크래치패드 9바이트를 검사하고 온도로 바꾼다
//
// 교재 19장 실습 19-2. 20_DS18B20 이 버스에서 읽은 뒤에 하는 일만 떼어 냈다.
// 보드도 센서도 없이 simavr 로 돌릴 수 있다(9장).
//
// 스크래치패드 (DS18B20 이 돌려주는 9바이트)
//   0 온도 LSB  1 온도 MSB  2 TH  3 TL  4 설정  5 예약  6 예약  7 예약  8 CRC
//   온도 = 16비트 2의 보수 x 0.0625 도 (12비트 분해능)
//   CRC  = 앞 8바이트의 Dallas CRC-8. 9바이트 전체의 CRC 는 0 이 된다
//
// 아래 표의 첫 줄은 전원을 켠 직후의 값(85 도)이다. 변환을 기다리지 않고 읽으면 이 값이 온다.
// 6번 바이트는 예약값이라 예시로 적었다. CRC 는 그 값을 넣어 계산했다.
//
// 출력 : 줄마다 CRC 검사 결과와 온도를 두 가지 방식으로 찍는다.
//   old : 20_DS18B20 처음 판의 방식. -1 도와 0 도 사이에서 부호가 사라진다
//   new : 부호를 따로 찍는 방식
//
// 선행 학습 : 20_DS18B20
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

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
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

// Dallas/Maxim CRC-8, 다항식 X^8 + X^5 + X^4 + 1 (20_DS18B20/onewire.c 와 같다)
static uint8_t ow_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;
    while (len--) {
        uint8_t inbyte = *data++;
        for (uint8_t i = 0; i < 8; i++) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

static const uint8_t samples[][9] = {
    { 0x50, 0x05, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x10, 0x1C },  // 전원 직후 85도
    { 0x91, 0x01, 0x4B, 0x46, 0x7F, 0xFF, 0x0F, 0x10, 0x25 },  // 25.0625도
    { 0x90, 0x01, 0x4B, 0x46, 0x7F, 0xFF, 0x0F, 0x10, 0x25 },  // 한 비트 깨짐
    { 0xF8, 0xFF, 0x4B, 0x46, 0x7F, 0xFF, 0x08, 0x10, 0xF8 },  // -0.5도
    { 0x5E, 0xFF, 0x4B, 0x46, 0x7F, 0xFF, 0x02, 0x10, 0xB6 },  // -10.125도
};

static void print_old(int16_t raw)            // 처음 판 : -0.5 가 0.500 이 된다
{
    int32_t  milli = (int32_t)raw * 625 / 10;
    int16_t  ip    = (int16_t)(milli / 1000);
    uint16_t fp    = (uint16_t)((milli < 0 ? -milli : milli) % 1000);
    PRINT("old %d.%03u", ip, fp);
}

static void print_new(int16_t raw)            // 부호를 먼저 찍고 크기만 나눈다
{
    int32_t milli = (int32_t)raw * 625 / 10;
    char sign = ' ';
    if (milli < 0) { sign = '-'; milli = -milli; }
    PRINT("new %c%ld.%03ld", sign, milli / 1000, milli % 1000);
}

int main(void)
{
    UBRR0  = F_CPU / 16 / 9600 - 1;
    UCSR0B = (1 << TXEN0);
    stdout = &uart_out;

    while (1) {
        for (uint8_t k = 0; k < sizeof samples / sizeof samples[0]; k++) {
            const uint8_t *sp = samples[k];
            uint8_t crc = ow_crc8(sp, 8);
            int16_t raw = (int16_t)((uint16_t)sp[1] << 8 | sp[0]);
            PRINT("%02X %02X ... CRC %02X/%02X %s  raw %6d  ", sp[0], sp[1], crc, sp[8],
                  crc == sp[8] ? "ok " : "BAD", raw);
            print_old(raw);
            PRINT("  ");
            print_new(raw);
            PRINT("\n");
        }
        PRINT("\n");
        _delay_ms(1000);
    }
}
