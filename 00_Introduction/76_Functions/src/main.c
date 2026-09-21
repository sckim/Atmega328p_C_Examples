/*=======================================================*/
// Functions : 함수(매개변수, 반환값)와 매크로(#define)의 차이
//
// 01_Arduino_Examples/00_Introduction/40_Functions 의 AVR C 버전이다.
// 결과는 UART(9600bps)로 출력된다.
//
// C 언어에는 함수 오버로딩이 없다(C++ 기능). 같은 이름의 함수를 여러 개 만들 수 없으므로
// square_i()/square_f() 처럼 이름으로 구분한다. (01 의 예제는 C++ 이라 오버로딩을 썼다)
//
// 선행 학습 : 74_Control_Flow      다음 단계 : 78_Arrays_Pointers
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>          // dtostrf()

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

// 매개변수 2개를 받아 합을 반환(return)하는 함수
static int add(int a, int b)
{
    return a + b;
}

// 반환값이 없는 함수는 void
static void print_line(char ch, int count)
{
    for (int i = 0; i < count; i++)
        putchar(ch);
    putchar('\n');
}

static int square_i(int x)
{
    return x * x;
}
static float square_f(float x)
{
    return x * x;
}

// 매크로: 함수처럼 보이지만 컴파일 전에 "텍스트를 그대로 치환"할 뿐이다.
#define SQUARE_MACRO(x) (x * x)          // 위험한 매크로 (괄호 부족)
#define SQUARE_SAFE(x)  ((x) * (x))      // 올바른 매크로

int main(void)
{
    uart_init();

    PRINT("=== 함수 호출: add(3, 4) ===\n");
    int result = add(3, 4);
    PRINT("%d\n\n", result);

    PRINT("=== void 함수: print_line('-', 10) ===\n");
    print_line('-', 10);

    PRINT("\n=== 자료형별 함수: square_i(5), square_f(2.5) ===\n");
    char buf[16];
    PRINT("%d\n", square_i(5));                          // 25
    dtostrf(square_f(2.5f), 1, 2, buf);                  // printf 는 기본적으로 float 를 출력하지 못한다
    PRINT("%s\n", buf);                                  // 6.25

    PRINT("\n=== 매크로의 함정: 텍스트 치환일 뿐! ===\n");
    volatile int n = 3;
    PRINT("SQUARE_MACRO(n+1) = %d\n", SQUARE_MACRO(n + 1));
    // (n+1 * n+1) 로 치환되어 * 가 먼저 계산된다: 3 + (1*3) + 1 = 7
    PRINT("괄호가 없어서 3+(1*3)+1 = 7 이 된다. 기대한 값은 16\n");
    PRINT("SQUARE_SAFE(n+1)  = %d   (괄호를 친 매크로)\n", SQUARE_SAFE(n + 1));
    PRINT("square_i(n+1)     = %d   (진짜 함수)\n", square_i(n + 1));

    while (1)
        ;
}
