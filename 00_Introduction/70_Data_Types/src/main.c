/*=======================================================*/
// Data_Types : 기본 자료형의 크기와 표현 범위, 오버플로우
//
// 01_Arduino_Examples/00_Introduction/10_Data_Types 의 AVR C 버전이다.
// 결과는 UART(9600bps, 8N1)로 출력된다. 터미널을 열어 확인한다.
//
// 임베디드에서는 "몇 바이트를 쓰는가"가 중요하다. ATmega328P 의 RAM 은 2KB 뿐이다.
// 그래서 <stdint.h> 의 고정폭 정수형(uint8_t 등)을 상황에 맞게 골라 쓴다.
//   uint8_t  : 레지스터(PORTB, DDRB ...) 1개와 같은 크기
//   uint16_t : 16비트 타이머 레지스터(TCNT1, OCR1A ...)와 같은 크기
//
// 다음 단계 : 72_Operators
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* ---- printf 를 UART 로 연결하는 최소 코드 (자세한 설명은 03_UART_Communication) ---- */
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
    UBRR0 = F_CPU / 16 / 9600 - 1;           // 9600 bps
    UCSR0B = (1 << TXEN0);                   // 송신 허용
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 data, no parity, 1 stop
    stdout = &uart_out;
}
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)  // 문자열을 Flash 에 둔다

int main(void)
{
    uart_init();

    PRINT("=== 기본 자료형 크기(byte) ===\n");
    PRINT("bool     : %u byte  (true/false)\n", (unsigned)sizeof(bool));
    PRINT("char     : %u byte  (-128 ~ 127)\n", (unsigned)sizeof(char));
    PRINT("int      : %u byte  (-32768 ~ 32767, 8비트 AVR 에서 int 는 16비트!)\n", (unsigned)sizeof(int));
    PRINT("long     : %u byte\n", (unsigned)sizeof(long));
    PRINT("float    : %u byte\n", (unsigned)sizeof(float));
    PRINT("double   : %u byte  (avr-gcc 에서는 float 와 같은 4byte)\n", (unsigned)sizeof(double));
    PRINT("pointer  : %u byte  (주소도 16비트)\n\n", (unsigned)sizeof(void *));

    PRINT("=== 고정폭 정수형 (<stdint.h>) ===\n");
    PRINT("uint8_t  : %u byte  (0 ~ 255)          <- 레지스터 1바이트\n", (unsigned)sizeof(uint8_t));
    PRINT("int8_t   : %u byte  (-128 ~ 127)\n", (unsigned)sizeof(int8_t));
    PRINT("uint16_t : %u byte  (0 ~ 65535)        <- 16비트 타이머 레지스터\n", (unsigned)sizeof(uint16_t));
    PRINT("uint32_t : %u byte  (0 ~ 4294967295)\n\n", (unsigned)sizeof(uint32_t));

    PRINT("=== 오버플로우 관찰 ===\n");
    volatile uint8_t small = 255;      // volatile: 컴파일러가 결과를 미리 계산해 버리지 못하게 한다
    small = small + 1;
    PRINT("uint8_t small = 255; small + 1 -> %u\n", small);   // 256 이 아니라 0

    volatile int8_t sign = 127;
    sign = sign + 1;
    PRINT("int8_t  sign  = 127; sign  + 1 -> %d\n", sign);    // -128
    PRINT("(표현 범위를 넘으면 값이 한 바퀴 돌아 처음으로 되돌아간다)\n");

    while (1)
        ;   // 관찰용 예제이므로 아무 것도 하지 않는다
}
