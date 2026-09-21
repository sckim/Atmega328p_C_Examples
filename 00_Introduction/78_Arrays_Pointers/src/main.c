/*=======================================================*/
// Arrays_Pointers : 배열과 포인터의 기초, 그리고 "레지스터는 주소다"
//
// 01_Arduino_Examples/00_Introduction/50_Arrays_Pointers 의 AVR C 버전이다.
// 결과는 UART(9600bps)로 출력된다.
//
// 레지스터(PORTB, DDRB ...)는 사실 특정 주소의 메모리이다. <avr/io.h> 는
//   #define PORTB (*(volatile uint8_t *)0x25)
// 같은 포인터 매크로를 모아 둔 것이다. 이 예제 마지막에서 직접 확인한다.
//
// 선행 학습 : 76_Functions      다음 단계 : 80_String
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

// 배열을 함수에 넘기면 사실 "시작 주소"만 전달된다
static void print_array(const int arr[], int len)
{
    for (int i = 0; i < len; i++)
        PRINT("%d ", arr[i]);
    PRINT("\n");
}

int main(void)
{
    uart_init();

    PRINT("=== 배열(Array): 같은 종류의 값을 여러 개 저장 ===\n");
    int led_pins[5] = {2, 3, 4, 5, 6};
    PRINT("led_pins 배열: ");
    print_array(led_pins, 5);
    PRINT("led_pins[0] = %d\n", led_pins[0]);
    PRINT("led_pins[4] = %d\n", led_pins[4]);
    PRINT("원소 개수 = sizeof(배열)/sizeof(원소) = %u\n\n", (unsigned)(sizeof(led_pins) / sizeof(led_pins[0])));

    PRINT("=== 포인터(Pointer): 변수의 '주소'를 저장하는 변수 ===\n");
    int x = 42;
    int *p = &x;                    // &x = x 의 주소, p 는 그 주소를 저장한다
    PRINT("x 의 값               : %d\n", x);
    PRINT("&x (x 의 주소)        : 0x%04X\n", (unsigned)(uintptr_t)&x);
    PRINT("p  (저장된 주소)      : 0x%04X\n", (unsigned)(uintptr_t)p);
    PRINT("*p (가리키는 곳의 값) : %d\n", *p);
    *p = 100;                       // 포인터를 통해 x 를 간접적으로 바꾼다
    PRINT("*p = 100; 후 x        : %d\n\n", x);

    PRINT("=== 배열 이름은 '첫 번째 원소의 포인터' ===\n");
    int *q = led_pins;
    PRINT("*q       = %d\n", *q);          // led_pins[0]
    PRINT("*(q + 1) = %d\n", *(q + 1));    // led_pins[1], 포인터 연산
    PRINT("q[2]     = %d\n\n", q[2]);      // 포인터도 [] 로 접근할 수 있다

    PRINT("=== 레지스터는 특정 주소의 메모리이다 ===\n");
    PRINT("&PORTB = 0x%02X (데이터시트의 I/O 주소 0x05 + 0x20)\n", (unsigned)(uintptr_t)&PORTB);
    volatile uint8_t *reg = (volatile uint8_t *)0x25;   // PORTB 의 주소를 포인터로 직접 지정
    DDRB |= (1 << DDB5);
    *reg |= (1 << PB5);                     // PORTB |= (1 << PB5) 와 같다
    PRINT("*reg |= (1<<PB5) 후 PORTB = 0x%02X\n", PORTB);
    PRINT("volatile 은 \"값이 프로그램 밖에서 바뀔 수 있으니 매번 실제로 읽고 쓰라\"는 뜻이다.\n");

    while (1)
        ;
}
