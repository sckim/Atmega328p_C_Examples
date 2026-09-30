/*=======================================================*/
// Timer1_Servo : 서보 펄스를 Timer1 하드웨어 PWM 으로 만든다. CPU 는 각도만 바꾼다
//
// 교재 17장 실습 17-3. 7장에서는 핀을 손으로 올리고 내려 서보 펄스를 만들었다.
// 여기서는 타이머가 20 ms 마다 펄스를 알아서 낸다.
//
// Timer1 모드 14 (WGM13:10 = 1110) : Fast PWM, TOP = ICR1
//   8분주면 1칸 = 0.5 us
//   ICR1  = 39999            -> 40000칸 x 0.5 us = 20 ms (50 Hz)
//   OCR1A = 펄스 폭(us) x 2 - 1  -> 1500 us 면 2999
// 16비트 타이머라 TOP 을 넉넉히 잡을 수 있다. 8비트 Timer0 으로는 20 ms 에
// 0.5 us 눈금을 줄 수 없다.
//
// 서보 신호 규격(7장) : 약 0.5 ms -> 0도, 1.5 ms -> 90도, 2.5 ms -> 180도
// 제품마다 다르다. 각도가 안 맞으면 PULSE_MIN / PULSE_MAX 를 보정한다.
//
// 연결 : 서보 신호선(노랑)을 PB1(아두이노 9번, OC1A)에. 빨강 5V, 갈색 GND.
//
// 선행 학습 : 38_FastPWM_Tone, 01_Arduino_Examples/08_Motors/12_Servo_Bitbang
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

#define PULSE_MIN  500     // us, 0도
#define PULSE_MAX  2500    // us, 180도

static void servo_write(uint8_t angle)
{
    uint16_t us = PULSE_MIN + (uint32_t)(PULSE_MAX - PULSE_MIN) * angle / 180;
    OCR1A = us * 2 - 1;
}

int main(void)
{
    DDRB  |= (1 << DDB1);                                  // OC1A 핀을 출력으로
    ICR1   = 39999;                                        // TOP -> 20 ms
    TCCR1A = (1 << COM1A1) | (1 << WGM11);                 // OC1A 비반전, WGM11
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);    // 모드 14, 8분주
    servo_write(90);

    while (1) {
        servo_write(0);    _delay_ms(1000);
        servo_write(90);   _delay_ms(1000);
        servo_write(180);  _delay_ms(1000);
        servo_write(90);   _delay_ms(1000);
    }
}
