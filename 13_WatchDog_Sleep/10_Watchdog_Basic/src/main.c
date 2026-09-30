/*=======================================================*/
// Watchdog_Basic : 워치독을 켜고 제때 wdt_reset() 으로 먹이를 준다. 원래 Microchip Studio 예제다
//
// 교재 21장 §21.3 에서 읽는다. 코드는 원래 main.c 그대로다(F_CPU 만 PlatformIO 에 맞췄다).
//
// 하는 일
//   WDT_Prescaler_Change(WDTO_500MS) : 0.5초 워치독을 켠다
//   while 안에서 400ms 마다 PD6 을 뒤집고 wdt_reset() 을 한다. 0.5초 안에 먹이를 주므로 리셋되지 않는다
//   ISR(WDT_vect) 는 PB5 를 뒤집는다 (먹이를 끊으면 한 번 불린다)
//
// 눈여겨볼 줄 : wdt_enable(time) 다음의  WDTCSR = (1 << WDIE);
//   WDCE 시퀀스 밖의 쓰기라 WDE 를 끄고 분주를 바꾸는 부분은 무시되고 WDIE 만 선다.
//   simavr 에서 읽으면 WDTCSR = 0x4D (WDIE + WDE + 0.5s) — "인터럽트 후 리셋" 모드다(교재 표 21-2).
//
// 연결 : PD6(아두이노 6번)에 LED, PB5 는 온보드 LED
//
// 다음 단계 : 12_Watchdog_Count
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

void WDT_off(void) {
    cli();
    wdt_reset();
    /* Clear WDRF in MCUSR */
    MCUSR &= ~(1 << WDRF);
    /* Write logical one to WDCE and WDE */
    /* Keep old prescaler setting to prevent unintentional time-out */
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    /* Turn off WDT */
    WDTCSR = 0x00;
    sei();
}

void WDT_Prescaler_Change(int time) {
    // Turn off global interrupt
    cli();

    wdt_reset();

    /* Start timed sequence */
    WDTCSR |= (1 << WDCE) | (1 << WDE);

    /* Set new prescaler(time-out) value = 64K cycles (~0.5 s) */
    //    WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP0);
    //or
    wdt_enable(time);
    WDTCSR = (1 << WDIE);

    // Turn on global interrupt
    sei();
}


int main(void) {
    WDT_Prescaler_Change(WDTO_500MS);

    DDRB |= _BV(5);  // watchdog timer 인터럽트
    DDRD |= _BV(6);  // timer reset 상태
    PORTD |= 0b00000000;

    PORTD |= _BV(6);
    _delay_ms(10);
    PORTD = 0;
    _delay_ms(10);
    PORTD |= _BV(6);
    _delay_ms(10);
    PORTD = 0;
    _delay_ms(10);

    while (1){
        _delay_ms(400);
        PORTD ^= _BV(6);
        wdt_reset();
    }
}

ISR(WDT_vect) {
    PORTB ^= _BV(5);
}