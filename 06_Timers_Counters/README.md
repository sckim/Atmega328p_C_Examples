# 06. Timers & Counters

## 🎯 학습 목표
*   Timer0(8비트), Timer1(16비트), Timer2(8비트)의 구조 차이를 이해한다.
*   Normal, CTC 모드로 정확한 주기를 만들고 `_delay_ms()`의 한계를 넘어선다.
*   프리스케일러와 비교값으로부터 실제 주기를 계산한다.

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `TCCR0A/B`, `TCCR1A/B` | 동작 모드(`WGM`), 프리스케일러(`CS`), 출력 비교 동작(`COM`) |
| `TCNT0`, `TCNT1` | 현재 계수값 |
| `OCR0A/B`, `OCR1A/B` | 비교 일치값 |
| `TIMSK0`, `TIMSK1` | 오버플로(`TOIE`) 및 비교 일치(`OCIE`) 인터럽트 허용 |

## 🧮 주기 계산
```
CTC 주기 = (OCRnA + 1) × 프리스케일 ÷ F_CPU
예) 16 MHz, 프리스케일 64, OCR0A = 249 → (250 × 64) / 16e6 = 1 ms
```


## 📌 참고
*   `_delay_ms()`는 CPU를 점유하는 바쁜 대기이다. 실제 시스템에서는 타이머 인터럽트로 만든 1 ms 틱을 쓴다.
*   Timer0을 아두이노 코어와 함께 쓰면 `millis()`가 어긋난다. 이 폴더는 코어를 쓰지 않으므로 그 제약이 없다.

---

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-28 -->

### 📂 예제 (15개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_Timer_Overflow` | Normal 모드 + 폴링으로 정확한 1초를 만든다 | 3 | 114 | ✓ | ✓ |  |  |
| `12_Timer0_OVF_ISR` | 10_Timer_Overflow 와 같은 동작을 인터럽트로 | 1 | 32 | ✓ |  |  |  |
| `12_Timer1_Overflow` | Timer1을 이용하여 1초마다 overflow | 1 | 46 |  | ✓ |  |  |
| `20_Timer_CTC` | CTC 모드로 8 ms 를 만들고, 비교 일치를 핀으로 직접 내보낸다 | 2 | 76 | ✓ | ✓ |  |  |
| `22_Timer0_CTC2` |  | 1 | 39 |  | ✓ |  |  |
| `24_Timer1_OCR` | 1초마다 overflow interrupt를 이용하여 | 1 | 47 |  | ✓ |  |  |
| `26_Timer1_Compare` | 4장_예제4-2(c) | 1 | 27 |  | ✓ |  |  |
| `30_Timer0_10mSec` | Timer0을 이용하여 1초마다 overflow | 1 | 46 |  | ✓ |  |  |
| `32_Timer0_Sec0_5` | 4장_예제4-2(c) | 1 | 24 |  | ✓ |  |  |
| `34_Timer0` | 2018. 10. 17. | 2 | 38 | ✓ | ✓ |  |  |
| `40_Osc1MHz` | Timer1 CTC 모드로 OC1A(PB1) 에 1MHz 구형파 출력 | 1 | 16 | ✓ |  |  |  |
| `50_LCDTimer` | 2016. 5. 5. | 1 | 82 |  | ✓ |  |  |
| `52_Timer_with_LCD` | 'lcd_lib.c' | 3 | 435 |  | ✓ |  |  |
| `60_PulseIn` | 2017. 10. 27. | 2 | 728 |  | ✓ |  |  |
| `62_PulseIn_timer` | 2017. 10. 27. | 2 | 766 |  | ✓ |  |  |

<!-- AUTO-INDEX:END -->
