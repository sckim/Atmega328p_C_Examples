/*=======================================================*/
// MCP3208 : 12비트 8채널 SPI ADC 를 하드웨어 SPI 로 읽는다
//
// 01_Arduino_Examples/10_SPI_Communication/50_MCP3208 의 AVR C 버전이다.
// 01 예제는 핀을 소프트웨어로 흔들었지만(bit-bang), 여기서는 ATmega328P 의 SPI 모듈을 쓴다.
// 하드웨어 SPI 는 CPU 가 SPDR 에 바이트를 쓰기만 하면 클럭 8개를 자동으로 만들어 준다.
//
// 연결 (ATmega328P)
//   MCP3208 /CS   <- PB2 (SS, 아두이노 10)     MCP3208 DIN  <- PB3 (MOSI, 11)
//   MCP3208 DOUT  -> PB4 (MISO, 12)            MCP3208 CLK  <- PB5 (SCK, 13)
//   VDD = VREF = 5V, AGND = DGND = GND
//
// SPI 레지스터
//   SPCR : SPE 허용, MSTR 마스터, SPR1:0 클럭 분주, CPOL/CPHA 모드 (MCP3208 은 mode 0)
//   SPSR : SPIF 전송 완료 플래그
//   SPDR : 쓰면 전송이 시작되고, 전송이 끝나면 같은 레지스터에 받은 값이 들어 있다
//
// MCP3208 통신 (한 번 변환 = 3바이트)
//   보내는 바이트 : [0000 0 1 SGL D2] [D1 D0 xxxxxx] [00000000]     (SGL/DIFF=1 : 싱글엔디드)
//   받는 바이트   : [xxxxxxxx] [xxxx B11 B10 B9 B8] [B7 ... B0]      -> 12비트 결과
//
// 결과 : UART(9600bps)로 CH0 ~ CH7 값을 출력 (01 예제의 Ch[1]~Ch[8] 은 여기서 CH0~CH7)
//
// 선행 학습 : 10_Comm_SPI, 20_ADC_multi
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

#define CS_PIN PB2
#define MOSI   PB3
#define MISO   PB4
#define SCK    PB5

static void spi_init(void)
{
    DDRB |= (1 << CS_PIN) | (1 << MOSI) | (1 << SCK);      // 출력 (SS 핀은 마스터에서도 반드시 출력이어야 한다)
    DDRB &= ~(1 << MISO);                                  // 입력
    PORTB |= (1 << CS_PIN);                                // /CS = High (비선택)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);         // SPI 허용, 마스터, mode 0, F_CPU/16 = 1MHz
}

static uint8_t spi_transfer(uint8_t data)
{
    SPDR = data;                        // 쓰는 순간 8클럭이 시작된다
    while (!(SPSR & (1 << SPIF)))       // 전송 완료 대기
        ;
    return SPDR;                        // 동시에 받은 바이트
}

static uint16_t mcp3208_read(uint8_t ch)          // ch : 0 ~ 7
{
    PORTB &= ~(1 << CS_PIN);                      // /CS = Low : 변환 시작
    spi_transfer(0x06 | (ch >> 2));               // 시작 비트 + SGL/DIFF + D2
    uint8_t hi = spi_transfer((ch & 0x03) << 6);  // D1 D0 보내며 상위 4비트
    uint8_t lo = spi_transfer(0x00);              // 하위 8비트
    PORTB |= (1 << CS_PIN);                       // /CS = High
    return ((uint16_t)(hi & 0x0F) << 8) | lo;
}

int main(void)
{
    uart_init();
    spi_init();
    PRINT("MCP3208 test\n");

    while (1) {
        for (uint8_t ch = 0; ch < 8; ch++)
            PRINT("Ch[%u]=%4u, ", ch, mcp3208_read(ch));
        PRINT("\n");
        _delay_ms(1000);
    }
}
