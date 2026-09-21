/*=======================================================*/
// BCD_4511 : BCD-7세그먼트 디코더(CD4511, 래치 내장) 2개로 두 자리 표시
//
// 01_Arduino_Examples/02_Segment_Display/20_BCD_4511 의 AVR C 버전이다.
//
// 연결
//   BCD 입력 A,B,C,D : PB2, PB3, PB4, PB5 (아두이노 핀 10~13) - 두 CD4511 이 공유
//   래치(/LE)        : PB0 (1의 자리), PB1 (10의 자리)
//
// 원리: 4비트 BCD 값(0~9)만 보내면 CD4511 이 7세그먼트 패턴으로 바꿔 준다.
//       /LE 를 Low 로 내려 두면 입력을 따라가고, High 로 올리는 순간 그 값을 래치(고정)한다.
//       그러므로 "데이터를 놓고 -> /LE 펄스" 를 자리마다 반복하면 MCU 가 계속 구동하지 않아도 된다.
//       (74LS47 등 다른 디코더와 달리 표시가 한 번 저장되면 MCU 의 부담이 없다)
//
// 선행 학습 : 10_7Segments      다음 단계 : 30_Two_7Segments
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

#define LATCH_ONES  PB0
#define LATCH_TENS  PB1
#define BCD_MASK    0x3C            // PB2 ~ PB5

// value : 0~9, digit : 0 = 1의 자리, 1 = 10의 자리
static void disp_num(unsigned char value, unsigned char digit)
{
    PORTB = (PORTB & ~BCD_MASK) | ((value & 0x0F) << 2);     // BCD 4비트를 PB2~PB5 에 출력

    unsigned char latch = (digit == 0) ? LATCH_ONES : LATCH_TENS;
    PORTB &= ~(1 << latch);         // /LE = Low  : 디코더가 입력을 받아들인다
    _delay_ms(10);
    PORTB |= (1 << latch);          // /LE = High : 값을 래치(고정)
}

int main(void)
{
    DDRB |= BCD_MASK | (1 << LATCH_ONES) | (1 << LATCH_TENS);
    PORTB |= BCD_MASK | (1 << LATCH_ONES) | (1 << LATCH_TENS);    // 래치는 High(비활성)로 시작

    unsigned char num = 0;
    while (1) {
        disp_num(num % 10, 0);      // 1의 자리
        disp_num(num / 10, 1);      // 10의 자리
        if (++num > 99)
            num = 0;
        _delay_ms(1000);            // 1초마다 증가
    }
}
