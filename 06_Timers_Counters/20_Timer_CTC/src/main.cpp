/*=======================================================*/
// Timer_CTC : CTC 모드로 8 ms 를 만들고, 비교 일치를 핀으로 직접 내보낸다
//
// 교재 16장 실습 16-3.  원천: 14주차A §3~4, 14주차B §3~4
// 검증 : PIO 6.2.0 / avr-gcc 7.3.0
//        OC0A(PD6) 가 8 ms 마다 토글, 7세그먼트가 1 초마다 0~9
//
// Normal 과의 차이 둘
//
//  1) TOP 을 내가 정한다
//     Normal 은 0xFF 가 고정 TOP 이라 매 주기 TCNT0 을 다시 넣어야 했다.
//     CTC 는 OCR0A 가 TOP 이고, 일치하면 하드웨어가 TCNT0 을 0 으로 되돌린다.
//     그래서 OCR0A 를 **한 번만** 설정하면 주기가 유지된다.
//
//  2) 비교 일치를 핀으로 바로 낼 수 있다
//     COM0A1:0 을 01 로 두면 일치할 때마다 OC0A(PD6) 를 하드웨어가 토글한다.
//     소프트웨어가 PORT 에 쓰지 않는다. 이것이 PWM 의 기반이다(17장).
//     단, 그 핀의 DDR 을 출력으로 해 두어야 트라이스테이트 버퍼가 열린다.
//
// OCR0A 값에 주의한다 — 한 주기는 **OCR0A + 1** 카운트다.
//   0 부터 세어 OCR0A 와 같아지는 순간까지이므로 칸 수는 OCR0A + 1 이다.
//   64 us x 125 칸 = 8 ms 를 정확히 만들려면 OCR0A = **124** 다.
//   125 로 두면 한 주기가 126 칸 = 8.064 ms 가 되어 1 초가 약 1.008 초가 된다.
//   수업에서 125 로 두었던 것을 여기서 바로잡았다.
//
// COM0A1:0 (Normal·CTC 에서)
//   00 일반 포트   01 토글   10 일치 시 Low   11 일치 시 High
//
// 선행 학습 : 12_Timer0_OVF_ISR      다음 단계 : 07_PWM/30_Timer0_FastPWM
/*=======================================================*/
#include <avr/io.h>

// 한 주기 = OCR0A + 1 칸. 125 칸(8 ms)을 원하므로 124 다.
#define cDelay (125 - 1)

volatile int sec = 0;
volatile int msec8 = 0;

void dispSeg(unsigned char ch) {
    PORTB &= 0xF0;                      // 하위 4비트만 지우고
    PORTB |= ch;                        // 새 값을 얹는다 (상위는 건드리지 않는다)
}

int main(void) {
    DDRB |= 0x0F;                       // PB3~PB0 출력 (7세그먼트)

    TCCR0A |= (1 << WGM01);             // WGM02:00 = 010 -> CTC
                                        // WGM01 은 TCCR0A, WGM02 는 TCCR0B 에 있다

    TCCR0B |= (1 << CS02);              // CS02:00 = 101
    TCCR0B |= (1 << CS00);              //   -> clk/1024

    DDRD |= _BV(PD6);                   // OC0A 핀을 출력으로 (안 하면 파형이 안 나간다)
    TCCR0A |= _BV(COM0A0);              // COM0A1:0 = 01 -> 일치할 때마다 토글

    OCR0A = cDelay;                     // TOP. 한 번만 쓰면 된다
    while (1) {
        if (bit_is_set(TIFR0, OCF0A)) { // 비교 일치 플래그. Normal 의 TOV0 자리다
            TIFR0 |= _BV(OCF0A);        // 1 을 써서 지운다
                                        // TCNT0 재설정은 필요 없다 — 하드웨어가 0 으로 되돌린다

            msec8++;
            if (msec8 == 125) {         // 8 ms x 125 = 1 s
                msec8 = 0;
                dispSeg(sec++);
            }
            if (sec == 10) {
                sec = 0;
            }
        }
    }
}
