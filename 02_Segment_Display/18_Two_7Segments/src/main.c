/*=======================================================*/
// Two_7Segments : 7-세그먼트 2자리를 다이나믹(멀티플렉싱) 구동으로 0~99 카운트
//
// 01_Arduino_Examples/02_Segment_Display/30_Two_7Segments 의 AVR C 버전이다.
//
// 연결
//   세그먼트 a,b,c,d,e,f,g,dp : PD0 ~ PD7 (두 자리가 같은 선을 공유한다)
//   1의 자리 선택 : PB0,  10의 자리 선택 : PB1  (High 이면 해당 자리가 켜진다)
//   (PD0, PD1 은 UART 핀이지만 이 예제는 UART 를 쓰지 않으므로 세그먼트에 사용한다)
//
// 원리: 한 번에 한 자리만 켜고, 10ms 마다 자리를 바꾼다. 사람 눈에는 잔상 때문에
//       두 자리가 동시에 켜진 것처럼 보인다. (한 자리당 갱신 주기 20ms = 50Hz)
//
// 세그먼트 코드는 active-low(공통 애노드형 값) 이다 : 0 이면 해당 세그먼트가 켜진다.
//
// 선행 학습 : 10_7Segments      다음 단계 : 20_Four7Segments
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

static const unsigned char SEG[10] = {
    0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
    0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};

// digit : 0 = 1의 자리, 1 = 10의 자리
static void disp_seg(unsigned char ch, unsigned char digit)
{
    PORTB &= ~((1 << PB0) | (1 << PB1));       // 먼저 두 자리를 모두 끈다 (잔상/번짐 방지)
    PORTD = SEG[ch];                           // 세그먼트 패턴 출력
    PORTB |= (digit == 0) ? (1 << PB0) : (1 << PB1);   // 해당 자리만 켠다
}

int main(void)
{
    DDRD = 0xFF;                               // 세그먼트 8개 출력
    DDRB |= (1 << PB0) | (1 << PB1);           // 자리 선택 출력
    PORTD = 0xFF;

    unsigned char num = 0;
    unsigned int duration = 0;

    while (1) {
        disp_seg(num % 10, 0);                 // 1의 자리
        _delay_ms(10);
        disp_seg(num / 10, 1);                 // 10의 자리
        _delay_ms(10);

        if (++duration % 50 == 0) {            // 약 1초마다 1 증가 (20ms x 50)
            if (++num > 99)
                num = 0;
        }
    }
}
