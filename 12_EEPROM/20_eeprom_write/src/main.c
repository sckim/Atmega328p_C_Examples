/*=======================================================*/
// eeprom_write : 내장 EEPROM 을 EEAR / EEDR / EECR 레지스터로 직접 쓰고 읽는다
//
// 01_Arduino_Examples/12_EEPROM/20_eeprom_write 의 AVR C 버전이다.
// (10_EEPROM 은 avr-libc 의 eeprom_read/write 함수를 쓴다. 여기서는 그 함수 안에서 하는 일을 직접 한다)
//
// ATmega328P 내장 EEPROM : 1024바이트, 전원을 꺼도 값이 남는다.
//
// 레지스터
//   EEAR (16비트) : 주소     EEDR : 데이터
//   EECR : EERE 읽기 시작, EEPE 쓰기 시작(쓰기가 끝나면 0), EEMPE 쓰기 허용(EEPE 를 켜기 전에 필요),
//          EEPM1:0 프로그래밍 모드(00 = 지우고 쓰기)
//
// 쓰기 순서 (반드시 이 순서와 시간 제한을 지켜야 한다)
//   1. 이전 쓰기가 끝날 때까지(EEPE == 0) 기다린다
//   2. EEAR, EEDR 에 주소와 데이터를 넣는다
//   3. EEMPE 를 1 로 만들고, "4클럭 안에" EEPE 를 1 로 만든다  <- 그 사이에 인터럽트가 끼면 실패하므로 cli()
//   쓰기 한 번에 약 3.4ms 가 걸린다.
//
// EEPROM 한 칸은 약 10만 번만 쓸 수 있다. 01 예제는 loop 안에서 계속 쓰지만,
// 이 예제는 부팅할 때마다 한 번만 쓰도록 만들었다.
//
// 동작
//   - 주소 0 : 부팅 횟수. 전원을 껐다 켜도 이어서 센다.
//   - 주소 16~31 : 테스트 패턴을 쓰고 다시 읽어서 비교한다.
// 결과 : UART(9600bps)
//
// 선행 학습 : 10_EEPROM      다음 단계 : 30_eeprom_24c02
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
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart_out;
}
#define PRINT(fmt, ...) printf_P(PSTR(fmt), ##__VA_ARGS__)

static void ee_write(uint16_t addr, uint8_t data)
{
    while (EECR & (1 << EEPE))          // 1. 이전 쓰기가 끝날 때까지 대기
        ;
    EEAR = addr;                        // 2. 주소, 데이터
    EEDR = data;
    uint8_t sreg = SREG;
    cli();                              // 3. EEMPE -> (4클럭 이내) EEPE 사이에 인터럽트 금지
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
    SREG = sreg;                        // 인터럽트 상태 복원
}

static uint8_t ee_read(uint16_t addr)
{
    while (EECR & (1 << EEPE))          // 쓰기 중이면 기다린다
        ;
    EEAR = addr;
    EECR |= (1 << EERE);                // 읽기 시작 (CPU 가 4클럭 멈추고 바로 EEDR 에 값이 들어온다)
    return EEDR;
}

int main(void)
{
    uart_init();
    DDRB |= (1 << DDB5);

    // 부팅 횟수 : 한 번도 쓴 적이 없는 EEPROM 은 0xFF 이다
    uint8_t boots = ee_read(0);
    if (boots == 0xFF)
        boots = 0;
    boots++;
    ee_write(0, boots);
    PRINT("부팅 횟수 = %u (전원을 껐다 켜 보세요)\n", boots);

    // 테스트 패턴 쓰기 -> 읽기 -> 비교
    uint8_t errors = 0;
    for (uint8_t i = 0; i < 16; i++)
        ee_write(16 + i, (uint8_t)(0xA0 + i));
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t v = ee_read(16 + i);
        PRINT("EEPROM[%u] = 0x%02X%s\n", 16 + i, v, v == (uint8_t)(0xA0 + i) ? "" : "  <- 불일치");
        if (v != (uint8_t)(0xA0 + i))
            errors++;
    }
    if (errors) {
        PRINT("검증 실패 (%u 개)\n", errors);
    } else {
        PRINT("검증 성공\n");
        PORTB |= (1 << PB5);            // 성공하면 LED 켜기
    }

    while (1)
        ;
}
