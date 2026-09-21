/*=======================================================*/
// ADC_multi : ADC0 ~ ADC5 여섯 채널을 차례로 읽어 전압으로 출력
//
// 01_Arduino_Examples/04_ADC/20_ADC_multi 의 AVR C 버전이다.
//
// ADC 는 하나뿐이고 입력 앞의 멀티플렉서(MUX)로 채널을 바꿔 가며 쓴다.
//   ADMUX  : REFS1:0 기준전압 선택, MUX3:0 채널 선택
//   ADCSRA : ADEN 허용, ADSC 변환 시작(끝나면 0), ADPS2:0 클럭 분주
//
// 변환 결과 : ADC = Vin / Vref * 1024  ->  Vin(mV) = ADC * 5000 / 1024 (Vref = AVcc = 5V)
//
// 참고 : 채널을 바꾼 직후 첫 변환은 이전 채널의 잔류 전하 영향을 받을 수 있어,
//        정밀하게 측정하려면 첫 변환을 버리거나 채널 변경 후 잠시 기다린다.
//
// 선행 학습 : 10_AnalogReadSerial      다음 단계 : 30_ADC_Int
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
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

static void adc_init(void)
{
    ADMUX = (1 << REFS0);                                                  // 기준전압 = AVcc
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);     // 125kHz
}

static uint16_t adc_read(uint8_t ch)
{
    ADMUX = (ADMUX & 0xF0) | (ch & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    return ADC;
}

int main(void)
{
    uart_init();
    adc_init();

    while (1) {
        for (uint8_t ch = 0; ch < 6; ch++) {
            uint16_t val = adc_read(ch);
            uint16_t mv = (uint16_t)((uint32_t)val * 5000UL / 1024UL);     // 정수로 mV 계산 (float 를 쓰지 않는다)
            PRINT("ADC[%u]=%4u, %u.%02uV  ", ch, val, mv / 1000, (mv % 1000) / 10);
        }
        PRINT("\n");
        _delay_ms(100);
    }
}
