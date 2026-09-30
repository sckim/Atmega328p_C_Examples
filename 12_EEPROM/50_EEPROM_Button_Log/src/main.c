/*=======================================================*/
// EEPROM_Button_Log : 버튼을 누를 때마다 EEPROM 에 한 바이트를 쓰고 곧바로 읽어 비교한다
//
// 교재 20장 실습 20-2. 원천은 옛 ATmega128 교재 영상 "6-8 EEPROM과 통합" 이다.
// 영상은 포토인터럽터가 가려질 때마다 EEPROM 에 쓰고 읽어 문자 LCD 에 [WR] [RD] 를 보였다.
// 여기서는 포토인터럽터 대신 버튼, LCD 대신 UART 를 쓴다. 흐름은 같다.
//   누를 때마다  주소 ee_addr 에 값 ee_wdata 를 쓰고 읽는다.  그다음 ee_addr++, ee_wdata--
//
// 영상의 교재 소스는 쓰기·읽기 함수를 직접 만들어 eeprom_write_byte 라는 이름을 붙였다.
// 그 이름은 avr-libc 의 <avr/eeprom.h> 와 겹친다. 여기서는 avr-libc 의 것을 그대로 쓴다.
//
// 켤 때 주소 0~7 을 먼저 찍는다. 몇 번 누른 뒤 전원을 껐다 켜면 쓴 값이 남아 있다.
//
// 연결 : 버튼 한쪽을 PD2(아두이노 2번), 다른 쪽을 GND. 내부 풀업을 쓴다(눌리면 Low).
// 결과 : UART(9600bps)
//
// 선행 학습 : 20_eeprom_write
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

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
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

#define PRESSED()  (!(PIND & (1 << PD2)))       // 풀업이라 눌리면 0

static void wait_press(void)                      // 뗀 상태 -> 누른 상태를 기다린다
{
    while (PRESSED())  ;                          // 이미 눌려 있으면 뗄 때까지
    _delay_ms(20);                                // 떼는 순간의 떨림
    while (!PRESSED()) ;
    _delay_ms(20);                                // 누르는 순간의 떨림
}

int main(void)
{
    UBRR0  = F_CPU / 16 / 9600 - 1;
    UCSR0B = (1 << TXEN0);
    stdout = &uart_out;
    PORTD |= (1 << PD2);                          // 내부 풀업

    PRINT("EEPROM 0~7 :");
    for (uint8_t a = 0; a < 8; a++)
        PRINT(" %02X", eeprom_read_byte((const uint8_t *)(uint16_t)a));
    PRINT("\n");

    uint16_t ee_addr  = 0;
    uint8_t  ee_wdata = 0xFF;
    while (1) {
        wait_press();

        eeprom_write_byte((uint8_t *)ee_addr, ee_wdata);
        uint8_t ee_rdata = eeprom_read_byte((const uint8_t *)ee_addr);

        PRINT("[EEPROM ADDR]%3X [WR]%02X [RD]%02X\n", ee_addr, ee_wdata, ee_rdata);

        ee_addr++;                                // 영상과 같이 주소는 늘고
        ee_wdata--;                               // 값은 준다
    }
}
