# 08. Motors

## 🎯 학습 목표
*   스테핑 모터의 상 여자 순서(full-step, half-step)를 직접 생성한다.
*   RC 서보의 펄스폭–각도 관계를 이해하고 타이머로 정확한 펄스를 만든다.
*   DC 모터의 속도와 회전 방향을 PWM과 H-브리지로 제어한다.

## 🛠 주요 부품
*   28BYJ-48 + ULN2003, SG90 RC 서보, DC 모터 + L298N/TB6612 드라이버, 별도 전원

## 📂 예제
| 폴더 | 내용 |
|---|---|
| `810_Motor_Stepper` | 스테핑 모터 상 여자와 속도 제어 |
| `811_Motor_RC` | RC 서보 각도 제어 |
| `812_Motor_DCM` | DC 모터 정역 회전과 속도 제어 |

## 📌 참고
*   모터 전원은 MCU 전원과 분리하고 GND만 공통으로 묶는다. 같은 레귤레이터를 쓰면 돌입 전류로 MCU가 리셋된다.
*   브러시 DC 모터와 코일 부하에는 역기전력 보호용 플라이백 다이오드가 필요하다.

---

<!-- AUTO-INDEX:BEGIN -->

## 🗂 폴더 현황 (자동 생성)

기준일 2026-09-20. 예제 폴더 3개, 회로도 보유 3개, `Project Backups` 백업본 2개.

| 폴더 | 소스 | Proteus 회로도 |
|---|---|---|
| `810_Motor_Stepper` | 2개 파일 | `AVR328P_Basic.pdsprj` |
| `811_Motor_RC` | 10개 파일 | `AVR328P_Basic_RC.pdsprj`; `AVR328P_RCServo.pdsprj`; `AVR328P_UART.pdsprj` (백업 2) |
| `812_Motor_DCM` | 2개 파일 | `AVR328P_Basic_DCM.pdsprj`; `AVR328P_DCM.pdsprj` |

<!-- AUTO-INDEX:END -->
