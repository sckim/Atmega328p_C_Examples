/*=======================================================*/
// ISR_Stack : 인터럽트가 걸리면 스택에 무엇이 쌓이는가. ISR 앞뒤에 붙는 코드
//
// 교재 9장 실습 9-3. 보드 없이 시뮬레이터(simavr)에서 돈다.
//
// INT0 핀(PD2)을 출력으로 두고 소프트웨어로 뒤집는다. 출력 핀이어도
// 외부 인터럽트는 걸린다(데이터시트 External Interrupts). 그래서 버튼 없이
// 상승 에지마다 ISR 이 불린다.
//
// ISR 은 count++ 한 줄인데 역어셈블하면 명령이 열다섯이다.
//   push r1 / push r0 / in r0,SREG / push r0 / eor r1,r1 / push r24   <- 저장
//   lds / subi / sts                                                   <- count++
//   pop r24 / pop r0 / out SREG,r0 / pop r0 / pop r1                   <- 복원
//   reti                                                               <- 복귀 + I = 1
// 저장·복원은 컴파일러가 넣는다(5장 표 5-3 의 4·6번). 복귀 주소는 하드웨어가 넣는다.
//
// 선행 학습 : 00_Introduction/92_Call_Stack, 10_Volatile
/*=======================================================*/
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char count;

ISR(INT0_vect)
{
    count++;
}

int main(void)
{
    DDRD  |= (1 << DDD2);                   // INT0 핀(PD2)을 출력으로
    EICRA  = (1 << ISC01) | (1 << ISC00);   // 상승 에지에서
    EIMSK  = (1 << INT0);                   // INT0 개별 허용
    sei();                                  // 전체 허용

    while (1) {
        PORTD ^= (1 << PD2);                // 핀을 뒤집는다. 올라갈 때마다 INT0
    }
}
