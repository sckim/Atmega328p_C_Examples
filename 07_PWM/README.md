# 07. PWM

## 🎯 학습 목표
*   Fast PWM, Phase-correct PWM, CTC 토글 방식의 파형 차이와 용도를 구분한다.
*   `OCRnx` 값으로 듀티비를, `TOP` 값과 프리스케일러로 주파수를 결정하는 원리를 이해한다.
*   하드웨어 PWM과 소프트웨어 PWM(비트뱅잉)의 장단점을 비교한다.

## 🛠 주요 부품
*   LED, DC 모터, RC 서보, 부저

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `TCCR0A/B`, `TCCR1A/B` | `WGM` 파형 모드, `COM` 출력 극성, `CS` 프리스케일러 |
| `OCR0A/B`, `OCR1A/B` | 듀티비 결정 |
| `ICR1` | Timer1에서 `TOP`을 지정해 주파수를 자유롭게 설정 |

## 🧮 주파수 계산
```
Fast PWM  : f = F_CPU / (프리스케일 × (TOP + 1))
Phase-correct : f = F_CPU / (2 × 프리스케일 × TOP)
```


## 📌 참고
*   RC 서보는 20 ms 주기에 1~2 ms 펄스폭이 필요하다. Timer1 + `ICR1`로 `TOP`을 잡는 방식이 정확하다.
*   DC 모터 구동에는 드라이버 IC(L298N, TB6612 등)가 필요하다. MCU 핀을 직접 연결하지 않는다.
*   `080_PWM_bitbang_v2`, `081_PWM_Timer0Interrupt2`, `81_PWM`, `82_PWM`은 `04_ADC`에 있던 것을 이 폴더로 옮겼다.
*   `081_PWM_Timer0Interrupt`은 `05_Interrupts/044_Timer0Overflow_Int_module`과 내용이 같아 제거했다.

---

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-30 -->

### 📂 예제 (15개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_AnalogWrite` |  | 2 | 47 | ✓ | ✓ |  |  |
| `12_PWM_bitbang` |  | 1 | 16 |  | ✓ |  |  |
| `14_PWM_Dual` |  | 1 | 15 |  | ✓ |  |  |
| `16_PWM_ADC` |  | 1 | 36 |  | ✓ |  |  |
| `20_Timer0_PWM` |  | 1 | 22 |  | ✓ |  |  |
| `30_Timer0_FastPWM` | Timer0 Fast PWM 으로 OC0A(PD6)·OC0B(PD5)에 PWM 두 개를 낸다 | 1 | 19 | ✓ | ✓ |  |  |
| `32_Timer0_FastPWM2` |  | 2 | 43 |  | ✓ |  |  |
| `34_FastPWM` |  | 1 | 32 | ✓ | ✓ |  |  |
| `36_Timer0_CTC_PWM` | TCNT0가 0이면 16.384ms 마다 overflow 발생 | 1 | 31 |  | ✓ |  |  |
| `38_FastPWM_Tone` | TOP 을 OCR0A 로 옮겨 PWM 의 주파수를 바꾼다. 부저로 음계를 낸다 | 1 | 25 | ✓ |  |  |  |
| `40_Timer1_PWM` |  | 1 | 15 |  | ✓ |  |  |
| `42_Timer1_FastPWM` | (percentage < 0 ? 0 : percentage)); | 2 | 211 |  | ✓ |  |  |
| `44_Timer1_Servo` | 서보 펄스를 Timer1 하드웨어 PWM 으로 만든다. CPU 는 각도만 바꾼다 | 1 | 28 | ✓ |  |  |  |
| `50_PWM_Timer0Interrupt` |  | 2 | 60 |  | ✓ |  |  |
| `60_PWM_Arduino_Style` | 2017. 10. 27. | 1 | 190 |  | ✓ |  |  |

<!-- AUTO-INDEX:END -->
