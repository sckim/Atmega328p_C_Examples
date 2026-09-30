/*=======================================================*/
// Read_Signature_Fuses : 칩의 시그니처와 퓨즈 비트, 락 비트를 읽기 전용으로 확인한다
//
// 01_Arduino_Examples/14_Bootloader/20_Read_Signature_Fuses 의 AVR C 버전이다.
//
// 부트로더가 어떻게 설치되고 동작하는지는 결국 퓨즈가 결정한다.
//   BOOTRST : 리셋 후 부트로더 영역에서 시작할지(0), 프로그램 0번지에서 시작할지(1)
//   BOOTSZ1:0 : 부트로더 영역의 크기 (우노의 Optiboot 는 11 = 256 words = 512 B, 0x7E00 부터)
//
// 읽는 방법 : SPM 명령의 특수한 사용
//   SPMCSR 에 SIGRD(시그니처 읽기, 또는 BLBSET)를 쓰고 3클럭 이내에 LPM 을 실행하면
//   플래시 대신 시그니처/퓨즈/락 값이 읽힌다. <avr/boot.h> 의 매크로가 이 절차를 해 준다.
//   읽기 전용이므로 칩을 망가뜨릴 위험이 없다. (쓰기 SPM 은 하지 않는다)
//
// ATmega328P 정상 시그니처 : 0x1E 0x95 0x0F
//   0x1E = Atmel 제조사 코드, 0x95 = 32KB Flash 계열, 0x0F = ATmega328P (ATmega328 은 0x14)
//
// Arduino Uno 의 기본 퓨즈 예 : Low 0xFF, High 0xDE(또는 0xD6), Extended 0xFD(또는 0x05)
//   High 의 bit0 = BOOTRST. 0 이면 부트로더로 먼저 진입한다.
//
// 퓨즈의 논리는 반대이다 : "프로그램됨" = 0, "프로그램 안 됨" = 1
//
// 우노에서 퓨즈를 보는 길은 사실상 이 예제뿐이다. USB 로 avrdude 를 돌려도 Optiboot 는 퓨즈 읽기
// 명령(STK_UNIVERSAL)에 늘 0 을 돌려준다. 제대로 읽으려면 ISP 프로그래머가 있어야 한다.
// simavr 는 보드의 퓨즈를 모른다. ELF 에 <avr/fuse.h> 의 FUSES 값이 없으면 엉뚱한 값이 찍힌다.
//
// 결과 : UART(9600bps)
//
// 선행 학습 : 10_MCUSR_ResetReason      다음 단계 : 30_Bootloader
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/boot.h>
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

int main(void)
{
    uart_init();

    PRINT("=== Device Signature ===\n");
    uint8_t sig0, sig1, sig2;
    cli();                                          // 3클럭 이내에 LPM 을 실행해야 하므로 인터럽트 금지
    sig0 = boot_signature_byte_get(0x0000);
    sig1 = boot_signature_byte_get(0x0002);
    sig2 = boot_signature_byte_get(0x0004);
    sei();
    PRINT("Signature = 0x%02X 0x%02X 0x%02X\n", sig0, sig1, sig2);
    PRINT("(ATmega328P 정상값: 1E 95 0F)\n\n");

    PRINT("=== Fuse Bits / Lock Bits ===\n");
    cli();
    uint8_t low_fuse  = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
    uint8_t high_fuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
    uint8_t ext_fuse  = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
    uint8_t lock_bits = boot_lock_fuse_bits_get(GET_LOCK_BITS);
    sei();
    PRINT("Low Fuse      = 0x%02X\n", low_fuse);
    PRINT("High Fuse     = 0x%02X\n", high_fuse);
    PRINT("Extended Fuse = 0x%02X\n", ext_fuse);
    PRINT("Lock Bits     = 0x%02X\n\n", lock_bits);

    PRINT("BOOTRST (High Fuse bit0) = %u -> %s\n", high_fuse & 0x01,
          (high_fuse & 0x01) ? "프로그램 0번지에서 시작" : "부트로더 영역에서 먼저 시작");
    PRINT("BOOTSZ1:0 (High Fuse bit2:1) = %u%u\n", (high_fuse >> 2) & 1, (high_fuse >> 1) & 1);
    PRINT("CKDIV8  (Low Fuse bit7)  = %u -> %s\n", (low_fuse >> 7) & 1,
          (low_fuse & 0x80) ? "클럭 분주 없음" : "클럭을 1/8 로 나눔");

    while (1)
        ;
}
