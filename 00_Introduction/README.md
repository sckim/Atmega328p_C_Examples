# 00. Introduction

## 🎯 학습 목표
*   AVR 툴체인(avr-gcc, avrdude)과 Microchip Studio / PlatformIO 개발 환경을 구축한다.
*   `main()` 진입 전에 수행되는 초기화 과정과 `.data` / `.bss` 섹션의 의미를 이해한다.
*   레지스터를 다루는 데 필요한 비트 조작(bit twiddling) 관용구를 익힌다.

## 💻 핵심 관용구
```c
PORTB |=  (1 << PB5);   // 비트 세트
PORTB &= ~(1 << PB5);   // 비트 클리어
PORTB ^=  (1 << PB5);   // 비트 토글
if (PINB & (1 << PB0))  // 비트 검사
```


## 📌 참고
*   `F_CPU`는 실제 클록(기본 16 MHz, 내부 RC 사용 시 8 MHz 또는 1 MHz)과 반드시 일치시켜야 한다. 불일치하면 `_delay_ms()`와 UART 보율이 모두 어긋난다.

---

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-27 -->

### 📂 예제 (11개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_Template` | 새 AVR C 프로젝트의 출발점 | 1 | 12 | ✓ |  |  |  |
| `20_How_to_Run` | 2020-05-01 오전 10:18:01 | 1 | 16 |  |  |  |  |
| `30_First_Program` | INT0(PD2), INT1(PD3)를 활성화 | 1 | 22 |  |  |  |  |
| `50_Bit_Twiddling` |  | 1 | 30 |  | ✓ |  |  |
| `60_Data_BSS` | 2020-04-14 오후 10:26:28 | 1 | 26 |  |  |  |  |
| `70_Data_Types` | 기본 자료형의 크기와 표현 범위, 오버플로우 | 1 | 56 | ✓ |  |  |  |
| `72_Operators` | 산술·비교·논리 연산자와 비트 연산자 | 1 | 65 | ✓ |  |  |  |
| `74_Control_Flow` | if/else, for, while, do-while, switch | 1 | 79 | ✓ |  |  |  |
| `76_Functions` | 함수(매개변수, 반환값)와 매크로(#define)의 차이 | 1 | 70 | ✓ |  |  |  |
| `78_Arrays_Pointers` | 배열과 포인터의 기초, 그리고 "레지스터는 주소다" | 1 | 67 | ✓ |  |  |  |
| `80_String` | C 문자열은 "널('\0')로 끝나는 char 배열"이다 | 1 | 63 | ✓ |  |  |  |

<!-- AUTO-INDEX:END -->
