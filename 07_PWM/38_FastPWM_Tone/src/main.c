/*=======================================================*/
// FastPWM_Tone : TOP 을 OCR0A 로 옮겨 PWM 의 주파수를 바꾼다. 부저로 음계를 낸다
//
// 교재 17장 실습 17-2. analogWrite 로는 PWM 주기를 바꿀 수 없다(7장).
// 레지스터로는 바꿀 수 있다. 모드 7 을 쓰면 된다.
//
// 모드 7 (WGM02:00 = 111) : TOP = OCR0A
//   OCR0A 가 주기를 정한다   주기 = (OCR0A + 1) x 분주 / 16 MHz
//   OCR0B 가 듀티를 정한다   OCR0B = OCR0A / 2 이면 약 50 %
//   대신 OC0A 핀은 PWM 으로 쓸 수 없다. OCR0A 가 TOP 이 되었기 때문이다
//
// 256분주면 1칸 = 16 us(62,500 Hz). 음 f 를 내려면
//   OCR0A = 62500 / f - 1          도(C4, 261.6 Hz) -> 238,  높은 도(C5) -> 118
//
// 연결 : 부저(+)를 PD5(아두이노 5번, OC0B)에, (-)를 GND 에.
//
// 선행 학습 : 30_Timer0_FastPWM      다음 단계 : 44_Timer1_Servo
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

// 도레미파솔라시도 (Hz)
static const uint16_t scale[] = { 262, 294, 330, 349, 392, 440, 494, 523 };

static void tone(uint16_t f)
{
    OCR0A = (uint8_t)(62500UL / f - 1);     // 주기
    OCR0B = OCR0A / 2;                      // 듀티 약 50 %
}

int main(void)
{
    DDRD  |= (1 << DDD5);                                  // OC0B 출력
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // 비반전, WGM01:00
    TCCR0B = (1 << WGM02) | (1 << CS02);                   // 모드 7, 256분주

    while (1) {
        for (uint8_t i = 0; i < 8; i++) {
            tone(scale[i]);
            _delay_ms(400);
        }
    }
}
