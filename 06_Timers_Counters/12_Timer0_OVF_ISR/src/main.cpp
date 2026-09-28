/*=======================================================*/
// Timer0_OVF_ISR : 10_Timer_Overflow 와 같은 동작을 인터럽트로
//
// 교재 16장 실습 16-2.  원천: 13주차B §6, 14주차A §2.5, 14주차B §1.4
// 검증 : PIO 6.2.0 / avr-gcc 7.3.0
//        PB5 가 8 ms 마다 토글, 7세그먼트가 1 초마다 0~9 (폴링판과 같다)
//
// 폴링과 달라진 것은 셋뿐이다.
//   1) #include <avr/interrupt.h>
//   2) while 안의 "할 일"을 ISR(TIMER0_OVF_vect) 로 옮긴다
//   3) 허용 비트 두 개를 켠다
//        TIMSK0 의 TOIE0  <- 개별 허용
//        sei()            <- 전체 허용 (SREG 의 I 비트, 7번)
//
// 둘 중 하나만 빠져도 ISR 이 호출되지 않는다. AND 조건이다.
// 데이터시트도 "TOIE0 를 1 로 쓰고, **그리고** SREG 의 I 비트가 세트되어 있으면"
// 이라고 적어 두었다.
//
// 플래그(TOV0)는 ISR 이 실행되면 하드웨어가 자동으로 지운다.
// 폴링처럼 TIFR0 에 1 을 쓸 필요가 없다.
//
// 설정 순서 : 타이머 설정 -> 개별 허용 -> 마지막에 sei().
// 애매하면 cli() 로 먼저 막고 설정한 뒤 sei() 한다.
//
// ISR 과 main 이 함께 보는 변수는 반드시 volatile 이다.
// 없으면 컴파일러가 값을 레지스터에 잡아 두고 메모리를 다시 읽지 않는다.
//
// 선행 학습 : 10_Timer_Overflow      다음 단계 : 20_Timer_CTC
/*=======================================================*/
#include <avr/io.h>
#include <avr/interrupt.h>

#define cDelay (256 - 125)              // 125 칸 = 8 ms

volatile char sec = 0;
volatile char msec8 = 0;

ISR(TIMER0_OVF_vect) {                  // 이름이 정해져 있다. 벡터 테이블이 부른다
    TCNT0 = cDelay;                     // 시작점 재설정은 여전히 필요하다
                                        // TOV0 클리어는 하드웨어가 해 준다

    msec8++;
    if (msec8 == 125) {                 // 8 ms x 125 = 1 s
        sec++;
        msec8 = 0;
        PORTD = (sec % 10) << 4;
    }
    if (sec == 99) {
        sec = 0;
    }
    PORTB ^= _BV(PB5);                  // 8 ms 마다 토글
}

int main(void) {
    DDRD |= 0xF0;                       // PD7~PD4 출력
    DDRB |= _BV(PB5);                   // PB5 = 아두이노 13번

    PORTD = 0;

    TCCR0A = 0;                         // Normal
    TCCR0B |= (1 << CS02) | (1 << CS00);// clk/1024
    TCNT0 = cDelay;

    TIMSK0 |= _BV(TOIE0);               // 개별 허용 — 이것만으로는 안 된다
    sei();                              // 전체 허용 — 이것만으로도 안 된다

    while (1) {
        // 할 일이 없다. 타이머가 알아서 부른다.
        // 폴링판은 이 자리에서 쉬지 않고 플래그를 확인했다.
    }
}
