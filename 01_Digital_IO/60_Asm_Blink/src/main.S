;=======================================================
; asmBlink : 같은 Blink 를 어셈블리로. 명령 넷이 전부다.
;
; 교재 8장. C 로 쓴 10_Blink 와 같은 일을 한다. 크기를 비교해 보라.
;
; 아두이노 -> C(레지스터) -> 어셈블리 로 내려온 모습이다.
;   pinMode(13, OUTPUT)     ->  DDRB  |= (1 << DDB5);   ->  sbi DDRB, DDB5
;   digitalWrite(13, LOW)   ->  PORTB &= ~(1 << PB5);   ->  cbi PORTB, PB5
;   digitalWrite(13, HIGH)  ->  PORTB |= (1 << PB5);    ->  sbi PORTB, PB5
;   loop() 반복             ->  while (1)               ->  rjmp Loop
;
; 지연이 없으므로 LED 는 눈에 보이지 않을 만큼 빠르게 깜빡인다.
; 계측기로 PB5(아두이노 13번 핀)를 보면 사각파가 보인다.
;
; 주소에 주의한다. 같은 레지스터에 주소가 둘 있다.
;   sbi/cbi 는 I/O 주소를 쓴다      : DDRB = 0x04,  PORTB = 0x05
;   데이터 메모리에서 본 주소는     : DDRB = 0x24,  PORTB = 0x25
;   차이는 언제나 0x20 이다. I/O 레지스터가 데이터 메모리의 0x20 부터 놓여 있기 때문이다.
;
; 빌드한 뒤 역어셈블해 기계어를 확인한다.
;   avr-objdump -d .pio/build/uno/firmware.elf
;
; 선행 학습 : 10_Blink (같은 동작의 C 판)
;=======================================================
#include <avr/io.h>

.section .text
.global main

main:
Setup:
    sbi  _SFR_IO_ADDR(DDRB), DDB5     ; PB5 를 출력으로
Loop:
    cbi  _SFR_IO_ADDR(PORTB), PB5     ; LED OFF
    sbi  _SFR_IO_ADDR(PORTB), PB5     ; LED ON
    rjmp Loop                         ; 되돌아간다. PC 를 바꾸는 것이 점프다
