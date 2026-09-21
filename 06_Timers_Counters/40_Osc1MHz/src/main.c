/*=======================================================*/
// Osc1MHz : Timer1 CTC 모드로 OC1A(PB1) 에 1MHz 구형파 출력
//
// 01_Arduino_Examples/06_Timers_Counters/40_Osc1MHz 의 AVR C 버전이다.
// (01 에서는 TimerOne 라이브러리로 주기 1us, 펄스 폭 0.5us 를 만들었다)
//
// 원리 : CTC 모드에서 TCNT1 이 OCR1A 까지 세고 0 으로 돌아오는 순간 OC1A 핀을 토글한다.
//        토글이므로 한 주기 = 비교 일치 두 번이다.
//
//        f_out = F_CPU / ( 2 x N x (1 + OCR1A) )       N : 분주비(1, 8, 64, 256, 1024)
//
//   16MHz, N=1 :  OCR1A = 7  ->  16MHz / (2 x 8) = 1MHz  (duty 50%)
//                 OCR1A = 3  ->  2MHz,  OCR1A = 1  ->  4MHz,  OCR1A = 0  ->  8MHz
//   ※ 이 값들은 CPU 가 명령 몇 개를 실행하는 동안의 시간이다. 이 속도에서는
//      MCU 가 매 클럭마다 핀을 직접 토글할 수 없지만, 타이머 하드웨어는 CPU 없이 해낸다.
//
// 측정 : PB1(아두이노 9번 핀)을 오실로스코프로 관찰한다.
//
// 선행 학습 : 20_Timer_CTC
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << DDB1);                        // OC1A(PB1) 출력

    TCCR1A = (1 << COM1A0);                     // 비교 일치 때 OC1A 토글
    TCCR1B = (1 << WGM12)                       // CTC 모드 (TOP = OCR1A)
           | (1 << CS10);                       // 분주 1, 타이머 시작
    OCR1A = 7;                                  // 16MHz / (2 x 8) = 1MHz

    while (1) {
        // 출력은 하드웨어(타이머)가 만든다. CPU 는 할 일이 없다.
    }
}
