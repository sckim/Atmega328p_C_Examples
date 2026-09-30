/*=======================================================*/
// SPI_595_Hardware : 74HC595 를 하드웨어 SPI 로 구동한다. SPDR 에 쓰면 끝이다
//
// 교재 18장 실습 18-1. 20_SerialShift_595 와 같은 동작(LED 막대)을 한다.
// 20 은 핀을 소프트웨어로 흔들었다(bit-bang). 여기서는 SPI 모듈이 클럭 8개를 만든다.
//
// 74HC595 는 SPI 장치로 만들어진 칩이 아니다. 그래도 SPI 로 구동된다.
// "클럭 상승 에지마다 데이터 한 비트를 밀어 넣는다"는 규칙이 SPI 모드 0 과 같기 때문이다.
//
// 연결 (74HC595)
//   SER   (DS,    14번 핀) <- PB3 (MOSI, 아두이노 11)
//   SRCLK (SH_CP, 11번 핀) <- PB5 (SCK,  아두이노 13)
//   RCLK  (ST_CP, 12번 핀) <- PB2 (SS,   아두이노 10)  래치. 한 바이트를 다 보낸 뒤 올린다
//   /OE 는 GND, /SRCLR 는 5V, Q0 ~ Q7 에 LED
//   MISO(PB4) 는 쓰지 않는다. 595 는 돌려주는 값이 없다
//
// SPI 설정 : 마스터, 모드 0(CPOL = 0, CPHA = 0), MSB 먼저, f/2 (SPI2X) = 8 MHz
//
// 선행 학습 : 20_SerialShift_595      다음 단계 : 60_SPI_Loopback_Frame
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

static const uint8_t lookup[8] = {
    0b00000001, 0b00000011, 0b00000111, 0b00001111,
    0b00011111, 0b00111111, 0b01111111, 0b11111111};

static void spi_init(void)
{
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);    // SS·MOSI·SCK 출력
    SPCR  = (1 << SPE) | (1 << MSTR);                   // 허용, 마스터, 모드 0
    SPSR  = (1 << SPI2X);                               // f/4 x 2 = f/2
}

static void spi_write(uint8_t data)
{
    SPDR = data;                          // 쓰는 순간 SCK 8개가 나간다
    while (!(SPSR & (1 << SPIF)))         // 다 나갈 때까지 기다린다
        ;
}

int main(void)
{
    spi_init();
    uint8_t pos = 0;
    while (1) {
        PORTB &= ~(1 << PB2);             // 래치 Low
        spi_write(lookup[pos++ % 8]);
        PORTB |= (1 << PB2);              // 래치 High : 출력에 반영
        _delay_ms(100);
    }
}
