/*=======================================================*/
// Blink : 온보드 LED(PB5, 아두이노 13번 핀)를 1초 간격으로 깜빡인다
//
// 01_Arduino_Examples/01_Digital_IO/10_Blink 의 AVR C 버전이다.
//
// 아두이노 함수와 레지스터의 대응
//   pinMode(13, OUTPUT)        ->  DDRB  |= (1 << DDB5);    // DDRx  : 방향 (1 = 출력)
//   digitalWrite(13, HIGH)     ->  PORTB |= (1 << PB5);     // PORTx : 출력값
//   digitalWrite(13, LOW)      ->  PORTB &= ~(1 << PB5);
//   (토글)                     ->  PORTB ^= (1 << PB5);
//   delay(1000)                ->  _delay_ms(1000);
//
// ATmega328P 의 핀 이름은 포트 + 번호이다. 아두이노 13번 핀 = PORTB 의 5번 비트 = PB5.
// 비트 조작(|= , &= ~, ^=)은 72_Operators 에서 설명한다.
//
// 다음 단계 : 20_Button
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << DDB5);                // PB5 를 출력으로 설정

    while (1) {
        PORTB |= (1 << PB5);            // LED ON
        _delay_ms(1000);
        PORTB &= ~(1 << PB5);           // LED OFF
        _delay_ms(1000);
    }
}
