/*=======================================================*/
// Volatile : ISR 과 main 이 함께 쓰는 변수에는 volatile 이 필요하다
//
// 01_Arduino_Examples/05_Interrupts/10_Volatile 의 AVR C 버전이다.
//
// 동작 : INT0(PD2)의 신호가 바뀔 때마다(상승·하강 모두) 인터럽트가 걸리고,
//        ISR 은 changed 를 1 로 만든다. main 은 changed 를 발견하면
//        온보드 LED(PB5)를 200ms 동안 켰다 끈다.
//
// 왜 volatile 인가?
//   컴파일러는 main() 의 while 문 안에서 changed 를 바꾸는 코드가 없다고 보고
//   "항상 0" 이라고 가정한 채 최적화(-Os)해 버릴 수 있다. 그러면 ISR 이 값을 바꿔도
//   main 은 레지스터에 남아 있는 옛 값만 보게 된다.
//   volatile 은 "이 변수는 프로그램 흐름 밖에서 바뀔 수 있으니 매번 메모리에서 읽어라"는 뜻이다.
//   (직접 volatile 을 지우고 빌드된 어셈블리를 비교해 보면 차이를 확인할 수 있다)
//
// 연결 : PD2(INT0)에 스위치 - 누르면 GND. 내부 풀업을 사용한다.
//
// 선행 학습 : 10_Blink, 20_Button      다음 단계 : 20_External_Interrupt
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

static volatile uint8_t changed = 0;        // ISR 과 main 이 공유 -> volatile

ISR(INT0_vect)
{
    changed = 1;
}

int main(void)
{
    DDRB |= (1 << DDB5);                    // PB5(LED) 출력
    DDRD &= ~(1 << DDD2);                   // PD2 입력
    PORTD |= (1 << PD2);                    // 내부 풀업

    EICRA = (1 << ISC00);                   // INT0 : 신호가 바뀔 때마다(any logical change)
    EIMSK = (1 << INT0);                    // INT0 허용
    sei();

    while (1) {
        if (changed == 1) {
            changed = 0;                    // 다시 기다린다
            PORTB |= (1 << PB5);            // LED 200ms
            _delay_ms(200);
            PORTB &= ~(1 << PB5);
        }
    }
}
