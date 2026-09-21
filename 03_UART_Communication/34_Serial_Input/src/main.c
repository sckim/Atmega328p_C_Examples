/*=======================================================*/
// Serial_Input : 컴퓨터가 보낸 명령을 UART 로 받아서 반응한다 (폴링 방식)
//
// 01_Arduino_Examples/03_UART_Communication/30_Serial_Input 의 AVR C 버전이다.
//
// 명령 형식 :  '#' 다음에 한 글자
//   #i  -> count 를 1 증가        #d  -> count 를 1 감소
// 100ms 마다 "Count1 = n,  Count = m" 을 출력한다.
//
// 시간은 Timer0 를 CTC 모드로 1ms 마다 인터럽트를 발생시켜 만든다 (millis() 와 같은 원리).
//   16MHz / 64(분주) / 250(OCR0A+1) = 1000Hz
//
// 선행 학습 : 30_UART_Reg, 20_Print      다음 단계 : 38_SerialEvent
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
    return 0;
}
static FILE uart_out = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static void uart_init(void)
{
    UBRR0 = F_CPU / 16 / 9600 - 1;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart_out;
}
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

static uint8_t rx_available(void)               // 받은 글자가 있는가?
{
    return UCSR0A & (1 << RXC0);
}

static char read_char(void)                     // 글자가 올 때까지 기다렸다가 읽는다 (blocking)
{
    while (!rx_available())
        ;
    return UDR0;
}

/* ---- 1ms 시간 기준 (Timer0 CTC) ---- */
static volatile uint32_t ms_ticks = 0;          // ISR 과 main 이 함께 쓰므로 volatile

ISR(TIMER0_COMPA_vect)
{
    ms_ticks++;
}

static void timer0_init(void)
{
    TCCR0A = (1 << WGM01);                      // CTC 모드
    OCR0A = 249;                                // 250 카운트마다 비교 일치
    TIMSK0 = (1 << OCIE0A);                     // 비교 일치 인터럽트 허용
    TCCR0B = (1 << CS01) | (1 << CS00);         // 분주 64, 타이머 시작
}

static uint32_t millis(void)
{
    uint32_t t;
    cli();                                      // 32비트 값을 읽는 동안 ISR 이 끼어들지 못하게 한다
    t = ms_ticks;
    sei();
    return t;
}

int main(void)
{
    int16_t count = 0;
    int16_t count1 = 0;
    uint32_t timestamp = 0;

    uart_init();
    timer0_init();
    sei();

    while (1) {
        if (rx_available()) {
            if (read_char() == '#') {           // 새 명령의 시작
                char command = read_char();
                if (command == 'i')
                    count++;
                else if (command == 'd')
                    count--;
            }
        }

        if (millis() - timestamp >= 100) {      // 100ms 마다 출력
            timestamp = millis();
            PRINT("Count1 = %d,  Count = %d\n", count1++, count);
        }
    }
}
