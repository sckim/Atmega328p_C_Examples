/*=======================================================*/
// SerialShift_595 : 시프트 레지스터 74HC595 로 출력 핀 8개를 3개로 늘린다
//
// 01_Arduino_Examples 의 shiftOut() 을 레지스터로 직접 구현한 AVR C 버전이다.
//   shiftOut(dataPin, clockPin, bitOrder, value)  ->  아래 shift_out() 함수
//
// 연결 (74HC595)
//   SER   (DS,    14번 핀) <- PD3 : 직렬 데이터
//   RCLK  (ST_CP, 12번 핀) <- PD4 : 래치 클럭 (Low -> High 순간 내부 값이 출력에 반영된다)
//   SRCLK (SH_CP, 11번 핀) <- PD5 : 시프트 클럭 (Low -> High 마다 SER 값이 한 칸씩 밀려 들어간다)
//   /OE 는 GND, /SRCLR 는 5V, Q0 ~ Q7 에 LED
//
// 동작 : 8개 LED 가 0 -> 1 -> ... -> 8 개씩 차례로 켜지는 막대(lookup 표)를 100ms 마다 보여 준다.
//
// 이 예제는 소프트웨어로 클럭을 흔드는 "bit-bang" 방식이다. 하드웨어 SPI 를 쓰면 (10_Comm_SPI)
// 같은 일을 SPDR 에 쓰기만 하면 되고 훨씬 빠르다.
//
// 선행 학습 : 10_Comm_SPI      다음 단계 : 30_MAX7219
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SER    PD3
#define RCLK   PD4
#define SRCLK  PD5
#define CTRL_PORT PORTD
#define CTRL_DDR  DDRD

static const uint8_t lookup[8] = {
    0b00000001, 0b00000011, 0b00000111, 0b00001111,
    0b00011111, 0b00111111, 0b01111111, 0b11111111};

// value 를 MSB 부터 한 비트씩 SER 에 놓고 SRCLK 펄스를 만든다
static void shift_out(uint8_t value)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (value & (1 << (7 - i)))
            CTRL_PORT |= (1 << SER);
        else
            CTRL_PORT &= ~(1 << SER);

        CTRL_PORT |= (1 << SRCLK);          // 상승 에지 : 한 칸 밀어 넣는다
        CTRL_PORT &= ~(1 << SRCLK);
    }
}

int main(void)
{
    CTRL_DDR |= (1 << SER) | (1 << RCLK) | (1 << SRCLK);

    uint8_t pos = 0;
    while (1) {
        shift_out(lookup[pos++ % 8]);

        CTRL_PORT &= ~(1 << RCLK);          // 래치 펄스 : 시프트한 값을 출력에 반영
        CTRL_PORT |= (1 << RCLK);

        _delay_ms(100);
    }
}
