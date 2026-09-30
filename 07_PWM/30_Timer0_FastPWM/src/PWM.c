/*=======================================================*/
// Timer0_FastPWM : Timer0 Fast PWM 으로 OC0A(PD6)·OC0B(PD5)에 PWM 두 개를 낸다
//
// 교재 17장 실습 17-1. 강의(14주차)에서 시연한 코드다. 레지스터 설정은 Timer0.h 의
// 함수 넷이 맡는다. 모드·분주·출력 모드를 이름으로 부르면 된다.
//
// 모드 3 (WGM02:00 = 011) : TOP = 0xFF. 카운터가 0 -> 255 를 되풀이하는 톱니파다
//   주기 = 256 x 분주 / 16 MHz          1024분주면 256 x 64 us = 16.384 ms
//   듀티 = OCR 이 정한다. 채널 A 는 OCR0A, 채널 B 는 OCR0B
//   High 구간 = OCR + 1 칸 (비반전일 때)
//
// 이 코드의 값
//   OCR0A = 51  (20 %)   채널 A 는 반전  -> High 가 256 - 52 = 204칸, 약 80 %
//   OCR0B = 102 (40 %)   채널 B 는 비반전 -> High 가 103칸, 약 40 %
//
// 연결 : PD6 = 아두이노 6번, PD5 = 아두이노 5번. 로직 애널라이저로 두 핀을 본다.
//
// 선행 학습 : 06_Timers_Counters/20_Timer_CTC      다음 단계 : 38_FastPWM_Tone
/*=======================================================*/
#define F_CPU	16000000L

#include <avr/io.h>
#include "Timer0.h"

#define DutyRatio	20
#define DutyValue (DutyRatio / 100.0 * 256)

int main(void)
{
	OCR0A = DutyValue;               // 51.2 -> 51
	OCR0B = 2*DutyValue;             // 102.4 -> 102

	Timer0Mode(FPWM);                // Fast PWM, TOP = 0xFF
	Timer0Prescaler(1024);           // 1칸 = 64 us, 주기 16.384 ms
	Timer0OutputA(Invert);           // 일치 시 High, BOTTOM 에서 Low
	Timer0OutputB(NonInvert);        // 일치 시 Low,  BOTTOM 에서 High

	while (1)
	{
		// CPU 는 아무것도 하지 않는다. 파형은 타이머가 만든다
	}
}
