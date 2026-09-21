/*=======================================================*/
// Power_Management : 워치독(알람) + Power-down 슬립 + 주변장치 전원 차단
//
// 01_Arduino_Examples/13_WatchDog_Sleep/50_Power_Management 의 AVR C 버전이다.
//
// 배터리로 동작하는 센서 노드의 기본 패턴이다.
//   "대부분의 시간은 Power-down 으로 자고, 워치독 인터럽트로 주기적으로 깨어나서
//    짧게 일하고 다시 잠든다."
//
// 10_Watchdog_Basic 에서는 WDT 를 "멈춘 프로그램을 리셋하는" 용도로 썼다.
// 여기서는 WDIE(인터럽트 모드)를 켜서 리셋 대신 인터럽트만 발생시키고 알람 시계로 쓴다.
//
// 전력을 줄이는 방법 (모두 함께 쓰면 평균 전류가 수 uA 로 내려간다)
//   1. Power-down 슬립          : set_sleep_mode(SLEEP_MODE_PWR_DOWN)
//   2. ADC 끄기                 : ADCSRA 의 ADEN = 0  (ADC 를 켠 채 슬립하면 전류가 남는다)
//   3. 주변장치 클럭 끄기       : power_all_disable()  -> PRR 레지스터의 모든 비트 (타이머, UART, SPI, TWI, ADC)
//   4. 슬립 중 BOD 끄기         : sleep_bod_disable()
//   5. 사용하지 않는 핀을 입력 + 풀업으로 두기 (떠 있는 핀은 전류를 만든다)
//
// 주의 : PRR 로 UART 를 껐다 켜면 모듈이 초기화되므로 깨어난 뒤 uart_init() 를 다시 호출한다.
//
// 동작 : 약 8초마다 깨어나 LED(PB5)를 한 번 깜빡이고 메시지를 출력한 뒤 다시 잠든다.
//
// 선행 학습 : 10_Watchdog_Basic, 20_Sleep_delay, 40_Deep_Sleep_ExtInterrupt
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>
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

static volatile bool wdt_woke_up = false;

// WDT 를 인터럽트 모드(리셋 없음)로 설정 : 약 8초 주기
static void watchdog_interrupt_setup(void)
{
    cli();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);                       // 변경을 허용받는 타이밍 시퀀스
    WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);         // 인터럽트 모드, 타임아웃 약 8초
    sei();
}

ISR(WDT_vect)                                                 // 타임아웃 -> 리셋 대신 이 ISR 이 실행되고 MCU 가 깨어난다
{
    wdt_woke_up = true;
}

static void go_to_sleep(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    ADCSRA &= ~(1 << ADEN);           // ADC 끄기
    power_all_disable();              // 모든 주변장치 클럭 차단

    cli();
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();                      // ---- WDT 인터럽트가 깨울 때까지 잠든다 ----
    sleep_disable();

    power_all_enable();               // 깨어나면 주변장치 클럭을 다시 켠다
}

int main(void)
{
    // 사용하지 않는 핀은 입력 + 풀업
    DDRB = (1 << DDB5);
    PORTB = ~(1 << PB5);
    DDRC = 0x00;
    PORTC = 0xFF;
    DDRD = 0x00;
    PORTD = 0xFF;

    watchdog_interrupt_setup();
    uart_init();
    PRINT("Power_Management : 8초마다 깨어납니다\n");
    _delay_ms(5);

    uint16_t wakeups = 0;
    while (1) {
        go_to_sleep();

        if (wdt_woke_up) {
            wdt_woke_up = false;

            uart_init();                          // PRR 을 껐다 켰으므로 UART 를 다시 초기화한다
            PORTB |= (1 << PB5);                  // LED 를 잠깐 켠다 (깨어 있는 시간을 짧게)
            PRINT("깨어남 #%u\n", ++wakeups);
            _delay_ms(50);
            PORTB &= ~(1 << PB5);
            _delay_ms(5);                         // 마지막 글자가 나갈 시간
        }
    }
}
