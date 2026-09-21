/*=======================================================*/
// Deep_Sleep_ExtInterrupt : Power-down 슬립에 들었다가 외부 인터럽트(INT0)로 깨어난다
//
// 01_Arduino_Examples/13_WatchDog_Sleep/40_Deep_Sleep_ExtInterrupt 의 AVR C 버전이다.
//
// Power-down 은 가장 깊은 슬립이다. 메인 클럭이 모두 멈추므로 타이머·UART·ADC 가 동작하지 않는다.
// 깨울 수 있는 것은 클럭이 필요 없는 것들뿐이다.
//   - INT0/INT1 의 Low 레벨 (에지가 아니라 "레벨")
//   - 핀 변화 인터럽트(PCINT)      - TWI 주소 일치      - WDT 인터럽트
// 그래서 이 예제는 INT0 를 Low 레벨로 설정한다.
//
// 동작
//   1초마다 카운터를 출력한다. 10번 세면 "잠들겠다"고 알리고 Power-down 에 든다.
//   PD2(INT0)를 Low 로 만들면(스위치를 누르면 GND) 깨어나서 다시 센다.
//   (01 예제는 RX 핀과 저항으로 D2 를 Low 로 만들어 깨운다. 여기서는 단순하게 스위치를 쓴다)
//
// 주의
//   - 깨어난 직후에는 클럭이 안정될 때까지(SUT, 약 수십~수백 us) 시간이 걸린다. 스위치를 그 이상 누르고 있어야 한다.
//   - 잠들기 전에 UART 전송을 마쳐야 한다. 그렇지 않으면 마지막 글자가 깨진다.
//   - sleep_bod_disable() 로 슬립 중 BOD(전원 감시)를 꺼서 전류를 더 줄인다. sleep_cpu() 직전에 호출해야 한다.
//   - Power-down 의 소비 전류 : 약 0.1uA (칩 단독). 보드에 따라 USB-UART 칩·레귤레이터가 전류를 훨씬 많이 쓴다.
//
// 연결 : PD2 에 스위치(누르면 GND), 내부 풀업 사용
//
// 선행 학습 : 30_IDLE_Sleep_ExtInterrupt      다음 단계 : 50_Power_Management
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
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

static void uart_flush(void)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    _delay_ms(2);                              // 마지막 글자가 시프트 레지스터에서 나갈 시간
}

ISR(INT0_vect)                                 // 깨우는 것이 목적이다. 반복 호출을 막으려고 끈다
{
    EIMSK &= ~(1 << INT0);
}

static void sleep_now(void)
{
    EICRA &= ~((1 << ISC01) | (1 << ISC00));   // Low 레벨
    EIFR = (1 << INTF0);
    EIMSK |= (1 << INT0);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sleep_bod_disable();                       // sleep_cpu() 직전(3클럭 이내)에 호출해야 효과가 있다
    sei();
    sleep_cpu();                               // ---- 여기서 잠든다 ----
    sleep_disable();                           // 깨어나면 여기서부터 계속된다
}

int main(void)
{
    uart_init();
    DDRD &= ~(1 << DDD2);                      // PD2 입력
    PORTD |= (1 << PD2);                       // 풀업
    sei();

    uint8_t count = 0;
    while (1) {
        PRINT("Awake... %u\n", count);
        count++;
        _delay_ms(1000);

        if (count >= 10) {
            PRINT("Timer elapsed - going to sleep (스위치를 누르면 깨어남)\n");
            uart_flush();                      // 잠들기 전에 전송을 마친다
            sleep_now();
            PRINT("Woke up!\n");
            count = 0;
        }
    }
}
