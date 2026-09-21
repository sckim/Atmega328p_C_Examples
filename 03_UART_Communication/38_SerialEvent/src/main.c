/*=======================================================*/
// SerialEvent : 수신 인터럽트(RXC)로 한 줄을 받아 처리한다
//
// 01_Arduino_Examples/03_UART_Communication/40_SerialEvent 의 AVR C 버전이다.
//
// 아두이노의 serialEvent() 는 "글자가 들어올 때마다 loop 사이에 호출되는 함수"이다.
// AVR C 에서는 그 정체가 USART_RX_vect 인터럽트 서비스 루틴(ISR)이다.
//
//   ISR  : 글자가 도착하는 즉시 실행되어 버퍼에 저장한다. 줄바꿈('\n')이 오면 완료 플래그를 세운다.
//   main : 완료 플래그를 보고 한 줄을 처리(여기서는 그대로 되돌려 출력)한다.
//
// ISR 과 main 이 함께 쓰는 변수는 반드시 volatile 로 선언해야 한다 (05_Interrupts/10_Volatile).
//
// 테스트 : 터미널에서 문자열을 입력하고 Enter(줄바꿈)를 누른다.
//
// 선행 학습 : 34_Serial_Input, 05_Interrupts      다음 단계 : 50_Comm_UART
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>
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
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);   // 수신 완료 인터럽트 허용
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart_out;
}
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

#define BUF_SIZE 64
static volatile char input_string[BUF_SIZE];    // 받은 글자를 모으는 버퍼
static volatile uint8_t input_len = 0;
static volatile bool string_complete = false;   // 한 줄이 완성되었는가

ISR(USART_RX_vect)
{
    char c = UDR0;                              // 읽으면 수신 플래그가 자동으로 지워진다
    if (string_complete)                        // 아직 이전 줄을 처리하지 않았다면 버린다
        return;
    if (c == '\r')
        return;                                 // 윈도우 터미널의 CR 은 무시
    if (c == '\n') {
        input_string[input_len] = '\0';
        string_complete = true;
    } else if (input_len < BUF_SIZE - 1) {
        input_string[input_len++] = c;
    }
}

int main(void)
{
    uart_init();
    sei();                                      // 전역 인터럽트 허용
    PRINT("SerialEvent : 문자열을 입력하고 Enter 를 누르세요\n");

    while (1) {
        if (string_complete) {
            PRINT("받은 문자열: %s\n", (const char *)input_string);
            input_len = 0;                      // 버퍼를 비운다
            string_complete = false;
        }
        // 여기서 다른 일을 해도 수신은 ISR 이 놓치지 않고 처리한다
    }
}
