/*=======================================================*/
// IDLE_Sleep_ExtInterrupt : Idle 슬립에 들었다가 외부 인터럽트(INT0)로 깨어난다
//
// 01_Arduino_Examples/13_WatchDog_Sleep/30_IDLE_Sleep_ExtInterrupt 의 AVR C 버전이다.
//
// 슬립 모드 (SMCR 의 SM2:0, SE)
//   Idle       : CPU 클럭만 멈춘다. 타이머·UART·ADC·외부 인터럽트 등 모든 주변장치가 계속 동작하고
//                어떤 인터럽트로도 깨어난다. 전류 절감은 작지만(약 1/4) 깨어나는 즉시 계속 실행한다.
//   Power-down : 거의 모든 클럭 정지. 외부 인터럽트(레벨)·WDT 등 소수만 깨울 수 있다. (40_Deep_Sleep)
//
// avr/sleep.h 의 사용법
//   set_sleep_mode(SLEEP_MODE_IDLE);   모드 선택
//   sleep_enable();  sleep_cpu();      SE 를 켜고 SLEEP 명령 실행 (인터럽트가 걸리면 다음 줄로 깨어난다)
//   sleep_disable();                   깨어난 뒤 SE 를 끈다 (실수로 슬립하는 것을 막는다)
//
// 동작
//   LED(PB5)를 켠 채로 1초 동안 깨어 있다가 LED 를 끄고 Idle 슬립에 든다.
//   PD2(INT0)를 Low 로 만들면(스위치를 누르면 GND) 깨어나서 LED 가 켜진다.
//   INT0 는 "Low 레벨" 로 설정한다. 이 예제의 ISR 이 하는 일은 인터럽트를 다시 끄는 것뿐이다.
//   (Low 레벨 인터럽트는 핀이 Low 인 동안 계속 걸리므로, 깨어난 즉시 끄지 않으면 ISR 이 반복 호출된다)
//
// 연결 : PD2 에 스위치(누르면 GND), 내부 풀업 사용. LED 는 온보드(PB5)
//
// 선행 학습 : 20_Sleep_delay, 05_Interrupts      다음 단계 : 40_Deep_Sleep_ExtInterrupt
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

ISR(INT0_vect)
{
    EIMSK &= ~(1 << INT0);                 // Low 레벨 인터럽트가 반복되지 않게 바로 끈다
}

static void sleep_now(void)
{
    EICRA &= ~((1 << ISC01) | (1 << ISC00));   // ISC0 = 00 : INT0 핀이 Low 레벨일 때 인터럽트
    EIFR = (1 << INTF0);                       // 이미 걸려 있던 플래그 지움
    EIMSK |= (1 << INT0);                      // INT0 허용

    set_sleep_mode(SLEEP_MODE_IDLE);
    cli();                                     // 준비하는 동안 인터럽트 금지
    sleep_enable();
    PORTB &= ~(1 << PB5);                      // LED 끄기 : 슬립 중임을 표시
    sei();                                     // sei 다음 한 명령 뒤에 인터럽트가 허용되므로
    sleep_cpu();                               // 인터럽트를 놓치고 영원히 자는 일이 없다
    sleep_disable();
    PORTB |= (1 << PB5);                       // 깨어남 : LED 켜기
}

int main(void)
{
    // 사용하지 않는 핀은 입력 + 풀업으로 두어 떠 있는 핀이 전류를 만들지 않게 한다
    DDRB = (1 << DDB5);                        // PB5 만 출력(LED)
    PORTB = ~(1 << PB5);                       // 나머지는 풀업
    DDRD &= 0x03;                              // PD2~PD7 입력 (PD0, PD1 은 UART 이므로 그대로)
    PORTD |= 0xFC;                             // PD2~PD7 풀업 (PD2 가 INT0 스위치 입력)

    PORTB |= (1 << PB5);                       // LED 켜기
    sei();

    while (1) {
        _delay_ms(1000);                       // 1초 동안은 깨어 있다
        sleep_now();                           // 슬립 -> INT0 로 깨어나면 여기로 돌아온다
    }
}
