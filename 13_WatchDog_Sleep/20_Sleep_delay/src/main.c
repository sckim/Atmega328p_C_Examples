/*=======================================================*/
// Sleep_delay : delay 대신 "잠자면서 기다리는" 지연 함수
//
// 01_Arduino_Examples/13_WatchDog_Sleep/20_Sleep_delay 의 AVR C 버전이다.
//
// _delay_ms() 는 CPU 가 빈 반복문을 돌며 기다리므로 전류를 계속 소비한다.
// 대신 Power-down 슬립에 들어가고 워치독 타이머(WDT)를 알람으로 써서 깨어나면
// 기다리는 동안의 소비 전류가 수 uA 로 떨어진다 (ATmega328P Power-down 약 0.1uA + WDT 약 4uA).
//
// 원리
//   WDT 는 자체 128kHz 오실레이터로 동작하므로 CPU 클럭이 꺼진 Power-down 에서도 돈다.
//   WDIE=1, WDE=0(인터럽트 모드)으로 두면 리셋 대신 인터럽트가 걸리고 MCU 가 깨어난다.
//   WDP3:0 로 16ms ~ 8s 의 타임아웃을 고른다.
//   원하는 시간(ms)을 큰 타임아웃부터 차례로 채워 넣는다. (예: 900ms = 500 + 250 + 125 + 16 ...)
//
// 한계 : WDT 오실레이터는 온도와 전압에 따라 약 ±10% 흔들린다. 정확한 시간이 필요하면
//        32.768kHz 워치 크리스털과 Timer2(비동기 모드)를 쓴다.
//        (01 예제는 Timer0 로 이 오차를 실측해 보정하지만, 여기서는 원리에 집중한다)
//
// 동작 : LED(PB5)를 100ms 켜고 900ms 는 슬립으로 기다린다. 한 번 깜빡일 때마다 UART 로 알린다.
//
// 선행 학습 : 10_Watchdog_Basic, 05_Interrupts      다음 단계 : 30_IDLE_Sleep_ExtInterrupt
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

static void uart_flush(void)                     // 마지막 글자가 다 나갈 때까지 잠깐 기다린다 (9600bps 에서 글자당 약 1ms)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    _delay_ms(2);
}

ISR(WDT_vect)
{
    // 깨우는 일만 한다. 여기서 할 일은 없다.
}

// WDT 를 인터럽트 모드로 켜고 Power-down 슬립 : wdp 는 WDP3:0 비트 조합
static void sleep_for_wdt(uint8_t wdp)
{
    cli();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);                       // WDRF 가 켜져 있으면 WDE 를 끌 수 없다
    WDTCSR |= (1 << WDCE) | (1 << WDE);          // 4클럭 안에 새 값을 써야 하는 타이밍 시퀀스
    WDTCSR = (1 << WDIE) | wdp;                  // 인터럽트 모드 + 타임아웃

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_bod_disable();                         // 슬립 중 BOD 를 꺼서 전류를 더 줄인다
    sei();                                       // sei 다음 한 명령은 인터럽트 없이 실행되므로 sleep_cpu 를 놓치지 않는다
    sleep_cpu();                                 // ---- 여기서 잠든다 ----
    sleep_disable();

    cli();                                       // 깨어난 뒤 WDT 를 끈다 (계속 두면 반복해서 깨운다)
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0x00;
    sei();
}

// ms 밀리초 동안 잠자며 기다린다 (WDT 오차 약 ±10%)
static void sleep_delay_ms(uint16_t ms)
{
    while (ms >= 16) {
        uint8_t wdp;
        uint16_t t;
        if      (ms >= 8000) { wdp = (1 << WDP3) | (1 << WDP0);              t = 8000; }
        else if (ms >= 4000) { wdp = (1 << WDP3);                            t = 4000; }
        else if (ms >= 2000) { wdp = (1 << WDP2) | (1 << WDP1) | (1 << WDP0); t = 2000; }
        else if (ms >= 1000) { wdp = (1 << WDP2) | (1 << WDP1);              t = 1000; }
        else if (ms >= 500)  { wdp = (1 << WDP2) | (1 << WDP0);              t = 500;  }
        else if (ms >= 250)  { wdp = (1 << WDP2);                            t = 250;  }
        else if (ms >= 125)  { wdp = (1 << WDP1) | (1 << WDP0);              t = 125;  }
        else if (ms >= 64)   { wdp = (1 << WDP1);                            t = 64;   }
        else if (ms >= 32)   { wdp = (1 << WDP0);                            t = 32;   }
        else                 { wdp = 0;                                      t = 16;   }
        sleep_for_wdt(wdp);
        ms -= t;
    }
}

int main(void)
{
    uart_init();
    DDRB |= (1 << DDB5);
    power_adc_disable();                         // 쓰지 않는 ADC 의 클럭을 끈다

    uint16_t n = 0;
    while (1) {
        PORTB |= (1 << PB5);                     // LED 켜기 (일하는 시간)
        _delay_ms(100);
        PORTB &= ~(1 << PB5);

        PRINT("%u : sleep 900ms\n", n++);
        uart_flush();                            // 슬립하기 전에 전송을 마친다 (UART 클럭은 슬립에서 멈춘다)
        sleep_delay_ms(900);                     // 잠자며 기다리는 시간
    }
}
