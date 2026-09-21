/*=======================================================*/
// ADC_Int : Timer0 비교 일치로 ADC 변환을 자동 시작하고, 변환 완료 인터럽트로 값을 받는다
//
// 01_Arduino_Examples/04_ADC/30_ADC_Int 의 AVR C 버전이다.
//
// 흐름 (CPU 는 변환을 기다리지 않는다)
//   Timer0 CTC 가 100Hz 로 비교 일치(OCF0A) 발생
//     -> ADC 자동 트리거(ADATE=1, ADTS = Timer0 Compare Match A) 로 변환 시작
//     -> 변환이 끝나면 ADC_vect 인터럽트 -> ISR 에서 결과를 읽고 flag 를 세운다
//   main 은 flag 를 보고 값을 출력한다 (여기서는 10번에 한 번 = 10Hz)
//
// 주의 : 자동 트리거는 "플래그가 1인 동안"이 아니라 "플래그가 0 -> 1 로 바뀔 때" 시작된다.
//        ADC 를 이 소스로 다시 트리거하려면 ISR 에서 OCF0A 를 지워 주어야 한다.
//
// 선행 학습 : 20_ADC_multi, 05_Interrupts, 06_Timers_Counters      다음 단계 : 15_Projects
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

static volatile uint16_t adc_value = 0;
static volatile uint8_t adc_flag = 0;
static volatile uint8_t sample_count = 0;

ISR(ADC_vect)
{
    TIFR0 = (1 << OCF0A);               // 트리거 소스 플래그 지움 (1 을 써서 지운다)
    adc_value = ADC;                    // 결과 읽기 (ADCL 을 읽으면 ADCH 가 잠기므로 16비트 ADC 로 한 번에)
    if (++sample_count >= 10) {         // 10 샘플마다 main 에 알린다
        sample_count = 0;
        adc_flag = 1;
    }
}

static void timer0_init(void)
{
    TCCR0A = (1 << WGM01);              // CTC 모드
    OCR0A = 155;                        // 16MHz / 1024 / 156 = 약 100Hz
    TCCR0B = (1 << CS02) | (1 << CS00); // 분주 1024, 시작
}

static void adc_init(uint8_t ch)
{
    ADMUX = (1 << REFS0) | (ch & 0x0F);                                    // AVcc 기준, 채널 선택
    ADCSRB = (1 << ADTS1) | (1 << ADTS0);                                  // 트리거 소스 : Timer0 Compare Match A
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE)                      // 허용, 자동 트리거, 완료 인터럽트
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);                   // 125kHz
}

int main(void)
{
    uart_init();
    timer0_init();
    adc_init(0);
    sei();

    while (1) {
        if (adc_flag) {
            uint16_t v;
            cli();                      // 16비트 값을 읽는 동안 ISR 이 갱신하지 못하게 한다
            v = adc_value;
            adc_flag = 0;
            sei();
            PRINT("ADC0 = %u\n", v);
        }
    }
}
