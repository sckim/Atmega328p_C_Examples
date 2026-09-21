/*=======================================================*/
// String : C 문자열은 "널('\0')로 끝나는 char 배열"이다
//
// 01_Arduino_Examples/00_Introduction/60_String 의 AVR C 버전이다.
// 결과는 UART(9600bps)로 출력된다.
//
// 아두이노의 String 클래스는 힙 메모리를 써서 RAM 이 작은 MCU 에서 조각화 문제를 일으킨다.
// AVR C 에서는 char 배열과 <string.h> 함수를 쓴다.
//
// AVR 은 RAM 과 Flash 의 주소 공간이 분리되어 있다(하버드 구조).
//   const char s[] = "abc";              // RAM 에 복사본이 올라간다 (RAM 낭비)
//   const char s[] PROGMEM = "abc";      // Flash 에만 둔다, 읽을 때 pgm_read_byte / strcpy_P
//   PSTR("abc")                          // 리터럴을 Flash 에 둔다 (printf_P 와 함께 사용)
//
// 선행 학습 : 78_Arrays_Pointers
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static const char flash_msg[] PROGMEM = "Flash(PROGMEM)에 저장된 문자열";

int main(void)
{
    uart_init();

    PRINT("=== 문자열은 char 배열 ===\n");
    char s1[] = "AVR";                       // { 'A', 'V', 'R', '\0' } -> 4 byte
    PRINT("s1 = \"%s\", strlen = %u, sizeof = %u\n", s1, (unsigned)strlen(s1), (unsigned)sizeof(s1));
    PRINT("(끝의 '\\0' 때문에 sizeof 는 strlen 보다 1 크다)\n\n");

    PRINT("=== 문자열 함수 (<string.h>) ===\n");
    char buf[24];
    strcpy(buf, "ATmega");                   // 복사
    strcat(buf, "328P");                     // 이어 붙이기 (buf 크기를 넘지 않게 주의!)
    PRINT("strcpy + strcat : %s\n", buf);
    PRINT("strcmp(\"abc\",\"abd\") = %d  (0 이면 같음, 음수면 앞쪽이 작음)\n", strcmp("abc", "abd"));
    PRINT("strchr(buf,'g') = %s\n\n", strchr(buf, 'g'));

    PRINT("=== 숫자 <-> 문자열 ===\n");
    char num[12];
    itoa(-1234, num, 10);                    // 정수 -> 10진 문자열
    PRINT("itoa(-1234, 10) = %s\n", num);
    itoa(255, num, 2);                       // 2진 문자열
    PRINT("itoa(255, 2)    = %s\n", num);
    itoa(255, num, 16);                      // 16진 문자열
    PRINT("itoa(255, 16)   = %s\n", num);
    PRINT("atoi(\"4321\")    = %d\n\n", atoi("4321"));

    PRINT("=== sprintf : 서식 있는 문자열 만들기 ===\n");
    snprintf(buf, sizeof(buf), "T=%d.%01dC", 25, 3);   // 소수점은 정수 두 개로 나누어 표현한다
    PRINT("%s\n\n", buf);

    PRINT("=== Flash 에 둔 문자열 읽기 ===\n");
    char local[48];
    strcpy_P(local, flash_msg);              // Flash -> RAM 복사
    PRINT("%s (길이 %u)\n", local, (unsigned)strlen_P(flash_msg));

    while (1)
        ;
}
