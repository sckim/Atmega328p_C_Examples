/*=======================================================*/
// Control_Flow : if/else, for, while, do-while, switch
//
// 01_Arduino_Examples/00_Introduction/30_Control_Flow 의 AVR C 버전이다.
// 결과는 UART(9600bps)로 출력된다.
//
// 아두이노의 loop() 는 결국 "무한 while 문" 이다. AVR C 에서는 main() 안의
// while (1) { ... } 이 그 역할을 직접 한다. 이후의 모든 예제가 이 제어문들의 조합이다.
//
// 선행 학습 : 72_Operators      다음 단계 : 76_Functions
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

int main(void)
{
    uart_init();

    PRINT("=== if / else if / else ===\n");
    for (int score = 0; score <= 100; score += 40) {
        PRINT("score=%d -> ", score);
        if (score >= 90) {
            PRINT("A\n");
        } else if (score >= 70) {
            PRINT("B\n");
        } else {
            PRINT("C\n");
        }
    }

    PRINT("\n=== for 문: 0부터 4까지 ===\n");
    for (int i = 0; i < 5; i++)
        PRINT("%d ", i);
    PRINT("\n");

    PRINT("\n=== while 문: 조건이 참인 동안 반복 (2의 거듭제곱, 100을 넘으면 종료) ===\n");
    int v = 1;
    while (v <= 100) {
        PRINT("%d ", v);
        v *= 2;
    }
    PRINT("\n");

    PRINT("\n=== do-while 문: 최소 한 번은 실행된다 ===\n");
    int n = 0;
    do {
        PRINT("실행됨 (n=%d)\n", n);
        n++;
    } while (n < 3);

    PRINT("\n=== switch-case: 값에 따라 분기 ===\n");
    for (int mode = 0; mode < 4; mode++) {
        PRINT("mode=%d -> ", mode);
        switch (mode) {
        case 0:
            PRINT("정지\n");
            break;
        case 1:
            PRINT("느림\n");
            break;
        case 2:
            PRINT("보통\n");
            break;
        default:            // case 에 없는 나머지 모든 값
            PRINT("빠름\n");
            break;          // break 를 빼먹으면 아래 case 까지 계속 실행된다
        }
    }

    while (1)               // 임베디드의 main() 은 끝나지 않는다
        ;
}
