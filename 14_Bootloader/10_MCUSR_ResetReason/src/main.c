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
// 부트로더가 있는 경우 (Arduino Uno)
//   우노에 든 Optiboot 4.4 는 먼저 실행되어 MCUSR 를 읽고 0 으로 지운다. 넘겨 주지는 않는다.
//   그래서 우노에서는 이 프로그램이 시작할 때 MCUSR 가 늘 0 이다.
//   Optiboot 4.6 부터는 지운 값을 레지스터 r2 에 넣어 넘겨 준다. 그 판을 구운 보드에서만 r2 가 쓸모 있다.
//     - MCUSR : .init3 섹션에서 저장    -> 부트로더 없이 ISP 로 올린 경우 (판단은 이 값으로 한다, 교재 22장)
//     - r2    : .init0 섹션에서 저장    -> 참고로만 찍는다
//   리셋은 I/O 레지스터만 초깃값으로 돌린다. r0~r31 은 초기화된다는 보장이 없다.
//   r2 를 채워 주는 부트로더가 없으면 r2 에는 리셋 전 값이 남는다. 그것을 리셋 원인으로 믿으면
//   엉뚱한 원인이 찍힌다 (simavr 에서 r2 = 4 를 남겨 두면 "Brown-out" 이 찍혔다).
//
// 관찰 방법 (UART 9600bps)
//   1. 리셋 버튼을 누른다               -> External Reset
//   2. 전원을 껐다 켠다                 -> Power-on Reset
//   3. 10_Watchdog_Basic 처럼 WDT 를 켜고 wdt_reset() 을 하지 않는다 -> Watchdog Reset
//   이 셋이 갈려 보이는 것은 ISP 로 올리고 BOOTRST 를 끈(High 퓨즈 0xDF) 보드다.
//   기본 우노에서는 셋 다 "플래그 없음"이다 (simavr 는 부트로더가 없어 갈려 보인다).
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

// .init0 : 리셋 직후 가장 먼저 실행된다. r2 를 가로챈다 (Optiboot 4.6 이상에서만 리셋 원인이다).
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
    PRINT("r2 (.init0, Optiboot 4.6 이상에서만 의미) = 0b"); print_bin(reset_flags_r2); putchar('\n');

    uint8_t flags = reset_flags_mcusr;   // r2 는 믿지 않는다: 채워 주는 부트로더가 없으면 남은 값이다
    if (flags & (1 << WDRF))  PRINT("-> Watchdog Reset (WDRF)\n");
    if (flags & (1 << BORF))  PRINT("-> Brown-out Reset (BORF)\n");
    if (flags & (1 << EXTRF)) PRINT("-> External Reset (리셋 버튼, EXTRF)\n");
    if (flags & (1 << PORF))  PRINT("-> Power-on Reset (PORF)\n");
    if (flags == 0)           PRINT("-> (플래그 없음: 부트로더가 이미 지웠다. 우노의 Optiboot 4.4 가 그렇다)\n");

    while (1)
        ;
}
