/*=======================================================*/
// MCUSR_ResetReason : MCU 가 "왜 리셋되었는지"를 MCUSR 레지스터로 확인한다
//
// 01_Arduino_Examples/14_Bootloader/10_MCUSR_ResetReason 의 AVR C 버전이다.
//
// MCUSR (MCU Status Register) 의 비트
//   PORF  (bit0) : 전원 인가 리셋 (Power-on)
//   EXTRF (bit1) : 외부 리셋 (리셋 버튼)
//   BORF  (bit2) : 전원 전압 강하 리셋 (Brown-out)
//   WDRF  (bit3) : 워치독 리셋
//
// 중요한 점 2가지
//   1. MCUSR 는 리셋되어도 자동으로 지워지지 않는다. 읽은 뒤 직접 0 으로 지워야 한다.
//   2. 워치독 리셋 후에는 WDT 가 계속 켜져 있어 다시 리셋되는 무한 루프에 빠질 수 있다.
//      그래서 시작 즉시 MCUSR 를 저장하고 지운 뒤 wdt_disable() 을 호출해야 한다.
//      이 처리는 main() 이 시작되기 전인 ".init3" 섹션에서 해야 안전하다.
//
// 부트로더가 있는 경우 (Arduino Uno 의 Optiboot)
//   부트로더가 먼저 실행되어 MCUSR 를 읽고 지운 뒤 레지스터 r2 에 복사해서 넘겨 준다.
//   그러면 우리 프로그램이 시작할 때 MCUSR 는 이미 0 이다. 그래서 두 방법을 모두 저장한다.
//     - r2 : .init0 섹션(가장 먼저 실행되는 코드)에서 저장    -> 부트로더가 있는 보드
//     - MCUSR : .init3 섹션에서 저장                         -> 부트로더 없이 ISP 로 올린 경우
//
// 관찰 방법 (UART 9600bps)
//   1. 리셋 버튼을 누른다               -> External Reset
//   2. 전원을 껐다 켠다                 -> Power-on Reset
//   3. 10_Watchdog_Basic 처럼 WDT 를 켜고 wdt_reset() 을 하지 않는다 -> Watchdog Reset
//
// 선행 학습 : 13_WatchDog_Sleep/10_Watchdog_Basic      다음 단계 : 20_Read_Signature_Fuses
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>

// .noinit : C 런타임이 0 으로 초기화하지 않는 영역. init 섹션에서 저장한 값이 지워지지 않는다.
uint8_t reset_flags_r2 __attribute__((section(".noinit")));
uint8_t reset_flags_mcusr __attribute__((section(".noinit")));

// .init0 : 리셋 직후 가장 먼저 실행된다. 부트로더가 r2 에 넣어 준 값을 가로챈다.
void get_reset_flags_r2(void) __attribute__((naked)) __attribute__((used)) __attribute__((section(".init0")));
void get_reset_flags_r2(void)
{
    __asm__ __volatile__("mov %0, r2\n" : "=r"(reset_flags_r2) :);
}

// .init3 : 스택 설정 직후, main() 이전. 부트로더가 없는 경우 MCUSR 를 직접 읽는다.
void get_reset_flags_mcusr(void) __attribute__((naked)) __attribute__((used)) __attribute__((section(".init3")));
void get_reset_flags_mcusr(void)
{
    reset_flags_mcusr = MCUSR;
    MCUSR = 0;                       // 반드시 지운다 (지우지 않으면 다음 리셋 때 과거 값과 섞인다)
    wdt_disable();                   // 워치독 리셋 후의 무한 리셋 루프 방지
}

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

static void print_bin(uint8_t v)
{
    for (int8_t i = 7; i >= 0; i--)
        putchar((v & (1 << i)) ? '1' : '0');
}

int main(void)
{
    uart_init();

    PRINT("=== MCUSR Reset Reason ===\n");
    PRINT("MCUSR 직접 읽은 값 (.init3) = 0b"); print_bin(reset_flags_mcusr); putchar('\n');
    PRINT("r2 로 받은 값 (.init0, 부트로더) = 0b"); print_bin(reset_flags_r2); putchar('\n');

    uint8_t flags = reset_flags_mcusr ? reset_flags_mcusr : reset_flags_r2;   // 둘 중 값이 있는 쪽을 쓴다
    if (flags & (1 << WDRF))  PRINT("-> Watchdog Reset (WDRF)\n");
    if (flags & (1 << BORF))  PRINT("-> Brown-out Reset (BORF)\n");
    if (flags & (1 << EXTRF)) PRINT("-> External Reset (리셋 버튼, EXTRF)\n");
    if (flags & (1 << PORF))  PRINT("-> Power-on Reset (PORF)\n");
    if (flags == 0)           PRINT("-> (플래그 없음: 부트로더나 다른 코드가 이미 지웠을 수 있다)\n");

    while (1)
        ;
}
