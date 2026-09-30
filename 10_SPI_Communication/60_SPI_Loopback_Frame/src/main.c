/*=======================================================*/
// SPI_Loopback_Frame : MOSI 와 MISO 를 이어 5바이트 프레임을 주고받고, 인터럽트로 조립한다
//
// 교재 18장 실습 18-2. 원천은 적외선 온도 모듈 DTM-M300 을 SPI 로 읽는 옛 강의 영상이다.
// 모듈은 640 ms 마다 5바이트를 보낸다.
//   0xA1   OH   OL   AH   AL        (0xA1 = 머리, O = 물체 온도, A = 주변 온도, 0.1 도 단위)
// 모듈이 없으므로 MOSI(11번)와 MISO(12번)를 점퍼로 이어 "보낸 것이 그대로 돌아오게" 한다.
// 보내는 쪽은 가짜 센서, 받는 쪽은 영상의 코드와 같은 ISR(SPI_STC_vect) 이다.
//
// 결과 : UART(9600bps)로 프레임마다 한 줄을 찍는다.
//
// FIXED_PARSER
//   0 : 영상의 파서. 0xA1 을 받으면 언제든 처음부터 다시 센다.
//       물체 온도가 16.1 도(= 0x00A1)이면 OL 바이트가 0xA1 이라 조립이 어긋난다.
//   1 : 머리를 기다리는 동안에만 0xA1 을 머리로 본다.
//
// 선행 학습 : 24_SPI_595_Hardware, 05_Interrupts/12_ISR_Stack
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#define FIXED_PARSER 0

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

volatile int     temp_obj, temp_air;   // 0.1 도 단위
volatile uint8_t req_spi;              // 남은 바이트 수. 0 이면 프레임 완성
volatile uint8_t busy;                 // 한 바이트 전송 중

ISR(SPI_STC_vect)
{
    uint8_t d = SPDR;                  // 보낸 것과 동시에 받은 바이트
    busy = 0;
#if FIXED_PARSER == 0
    req_spi = req_spi > 0 ? req_spi - 1 : 0;   // 영상의 코드 그대로
    if (d == 0xA1) {                           // 0xA1 이면 언제든 머리로 본다
        req_spi = 4;
        return;
    }
#else
    if (req_spi == 5) {                        // 머리를 기다리는 중일 때만
        if (d == 0xA1) req_spi = 4;
        return;
    }
    req_spi--;
#endif
    switch (req_spi) {
    case 3: temp_obj  = (unsigned int)d << 8; break;
    case 2: temp_obj |= (unsigned int)d;      break;
    case 1: temp_air  = (unsigned int)d << 8; break;
    case 0: temp_air |= (unsigned int)d;      break;
    }
}

// 가짜 센서 : 5바이트를 하나씩 보낸다. 받는 일은 ISR 이 한다
static void send_frame(int obj, int air)
{
    const uint8_t f[5] = { 0xA1, obj >> 8, obj & 0xFF, air >> 8, air & 0xFF };
    for (uint8_t i = 0; i < 5; i++) {
        busy = 1;
        SPDR = f[i];
        while (busy)
            ;
    }
}

int main(void)
{
    UBRR0  = F_CPU / 16 / 9600 - 1;
    UCSR0B = (1 << TXEN0);
    stdout = &uart_out;

    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);   // SS·MOSI·SCK 출력
    SPCR  = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << SPR0);  // f/16
    sei();

    // 물체 25.3·16.1·25.3 도, 주변 19.8 도
    static const int test[][2] = { {253, 198}, {161, 198}, {253, 198} };
    while (1) {
        for (uint8_t k = 0; k < 3; k++) {
            if (req_spi == 0) req_spi = 5;      // 새 프레임을 기다린다
            send_frame(test[k][0], test[k][1]);
            if (req_spi == 0)
                PRINT("sent %d.%d -> obj %d.%d air %d.%d\n",
                      test[k][0] / 10, test[k][0] % 10,
                      temp_obj / 10, temp_obj % 10, temp_air / 10, temp_air % 10);
            else
                PRINT("sent %d.%d -> incomplete (req_spi = %u)\n",
                      test[k][0] / 10, test[k][0] % 10, req_spi);
            _delay_ms(640);
        }
    }
}
