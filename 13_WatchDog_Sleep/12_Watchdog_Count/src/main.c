/*=======================================================*/
// Watchdog_Count : 2초 워치독을 켜고 0.25초마다 센다. 먹이를 주지 않으면 8에서 리셋된다
//
// 교재 21장 실습 21-1. 허경용 교재 코드 28-1(워치독 만료)·28-2(워치독 리셋)를 AVR C 로 옮겼다.
//   FEED 0 : wdt_reset() 을 하지 않는다 -> 2초 뒤 리셋, "** Initialization **" 부터 다시
//   FEED 1 : 셀 때마다 wdt_reset() -> 리셋 없이 계속 센다
//
// 켤 때 리셋 원인(MCUSR)을 찍는다. 이 값은 main() 전에(.init3) 읽고 지우며 워치독도 끈다.
// 워치독 리셋 뒤에는 WDRF 때문에 워치독이 16ms 로 켜진 채 시작하기 때문이다(교재 §21.4).
// 단, 우노의 부트로더(Optiboot 4.4)는 MCUSR 을 먼저 읽고 지운 뒤 넘겨주지 않는다.
// 그래서 보드에서는 MCUSR = 0x00 으로 보인다. 부트로더가 없는 simavr 에서는 0x08(WDRF) 이 보인다.
//
// 결과 : UART(9600bps)
//
// 선행 학습 : 10_Watchdog_Basic      다음 단계 : 20_Sleep_delay
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#define FEED 0

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

// .noinit : 시작 코드가 0 으로 지우지 않는 영역
uint8_t reset_cause __attribute__((section(".noinit")));

// .init3 : 스택을 세운 직후, main() 전. 16ms 안에 끄려면 여기서
void early_init(void) __attribute__((naked, used, section(".init3")));
void early_init(void)
{
    reset_cause = MCUSR;
    MCUSR = 0;                          // WDRF 를 지워야 WDE 를 끌 수 있다
    wdt_disable();
}

int main(void)
{
    UBRR0  = F_CPU / 16 / 9600 - 1;
    UCSR0B = (1 << TXEN0);
    stdout = &uart_out;

    PRINT("** Initialization **  MCUSR = 0x%02X%s\n", reset_cause,
          (reset_cause & (1 << WDRF)) ? "  (워치독 리셋)" : "");

    wdt_enable(WDTO_2S);                // 2초 안에 먹이를 주지 않으면 리셋
    uint16_t count = 0;
    while (1) {
        _delay_ms(250);
        PRINT("Count : %u\n", ++count);
#if FEED
        wdt_reset();                    // 먹이를 준다 : 타이머를 0 으로
#endif
    }
}
