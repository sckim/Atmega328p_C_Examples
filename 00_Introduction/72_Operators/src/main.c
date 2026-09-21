/*=======================================================*/
// Operators : 산술·비교·논리 연산자와 비트 연산자
//
// 01_Arduino_Examples/00_Introduction/20_Operators 의 AVR C 버전이다.
// 결과는 UART(9600bps)로 출력된다.
//
// 레지스터 코드의 대부분은 아래 비트 연산 조합이다.
//   TCCR0A |= (1 << WGM01);      // 비트 SET
//   PORTB  &= ~(1 << PB5);       // 비트 CLEAR
//   PORTB  ^= (1 << PB5);        // 비트 TOGGLE
//   if (PINB & (1 << PB0)) ...   // 비트 TEST
//
// 선행 학습 : 70_Data_Types      다음 단계 : 74_Control_Flow
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
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
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart_out;
}
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

// 8비트 값을 2진수 8자리로 출력한다 (printf 에는 %b 가 없다)
static void print_bin(uint8_t v)
{
    for (int8_t i = 7; i >= 0; i--)
        putchar((v & (1 << i)) ? '1' : '0');
}

int main(void)
{
    uart_init();

    PRINT("=== 산술 연산자 ===\n");
    volatile int a = 7, b = 2;
    PRINT("a=7, b=2 -> a+b=%d, a-b=%d, a*b=%d, a/b=%d, a%%b=%d\n", a + b, a - b, a * b, a / b, a % b);
    PRINT("(정수 나눗셈은 소수점을 버린다: 7/2 = 3)\n\n");

    PRINT("=== 비트 연산자 (임베디드의 핵심!) ===\n");
    uint8_t reg = 0b00000000;          // 8개 핀 상태를 흉내 낸 가상의 "레지스터"
    PRINT("초기값          : 0b"); print_bin(reg); putchar('\n');

    reg |= (1 << 3);                   // 3번 비트를 켠다 (다른 비트는 그대로)
    PRINT("3번 비트 SET    : 0b"); print_bin(reg); putchar('\n');

    reg &= ~(1 << 3);                  // 3번 비트를 끈다
    PRINT("3번 비트 CLEAR  : 0b"); print_bin(reg); putchar('\n');

    reg ^= (1 << 5);                   // 토글: 꺼져 있으면 켜고
    PRINT("5번 비트 TOGGLE : 0b"); print_bin(reg); putchar('\n');
    reg ^= (1 << 5);                   // 켜져 있으면 끈다
    PRINT("5번 비트 TOGGLE : 0b"); print_bin(reg); putchar('\n');

    PRINT("1 << 4          : 0b"); print_bin(1 << 4); putchar('\n');
    PRINT("0b00010000 >> 2 : 0b"); print_bin(0b00010000 >> 2); putchar('\n');

    PRINT("\n=== 특정 비트가 켜져 있는지 검사 ===\n");
    reg = 0b00101000;
    PRINT("reg = 0b00101000, 3번 비트는 %s\n", (reg & (1 << 3)) ? "켜짐(1)" : "꺼짐(0)");
    PRINT("reg = 0b00101000, 4번 비트는 %s\n", (reg & (1 << 4)) ? "켜짐(1)" : "꺼짐(0)");

    PRINT("\n=== 진짜 레지스터에 적용 : PORTB 의 PB5(온보드 LED) ===\n");
    DDRB |= (1 << DDB5);               // PB5 를 출력으로
    PORTB |= (1 << PB5);               // PB5 를 High
    PRINT("PORTB |=  (1<<PB5) 후 PORTB = 0b"); print_bin(PORTB); putchar('\n');
    PORTB &= ~(1 << PB5);              // PB5 를 Low
    PRINT("PORTB &= ~(1<<PB5) 후 PORTB = 0b"); print_bin(PORTB); putchar('\n');

    while (1)
        ;
}
