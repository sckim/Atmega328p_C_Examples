# Simple Sensors (Extended)

## 🎯 학습 목표
*   전용 통신 프로토콜 없이 디지털 입출력과 타이밍만으로 동작하는 센서를 다룬다.
*   행렬 스캔(키패드), 펄스폭 측정(초음파), 구적 신호 해석(엔코더) 기법을 익힌다.

## 🛠 주요 부품
*   4×4 매트릭스 키패드, HC-SR04 초음파 센서, 로터리 엔코더, 74LS595

## 💻 핵심 기법
| 센서 | 기법 |
|---|---|
| 키패드 | 행을 하나씩 Low로 내리며 열을 읽는 스캔. 내부 풀업 사용 |
| HC-SR04 | Trig에 10 µs 펄스 → Echo의 High 구간을 타이머로 측정. 거리(cm) = 시간(µs) / 58 |
| 로터리 엔코더 | A/B 두 신호의 위상차로 방향 판별. 핀 변화 인터럽트와 상태 천이표 사용 |

## 📂 예제
| 폴더 | 내용 |
|---|---|
| `026_Keypad`, `25_Keypad`, `26_Keypad(4x4)` | 매트릭스 키패드 스캔 |
| `510_Ultrasound`, `510_Ultrasound_v2` | 초음파 거리 측정 |
| `511_RotaryEncoder` | 로터리 엔코더 회전 방향·계수 |
| `017_74LS596_two` | 시프트 레지스터 입출력 확장 |
| `950_ttl_uC` | TTL 레벨 신호 실습 |

## 📌 참고
*   HC-SR04는 5 V 동작이다. 3.3 V MCU에 연결할 때는 Echo 쪽에 분압이 필요하다.
*   엔코더 신호는 채터링이 심하다. RC 필터 또는 상태 천이표 기반 디바운스를 쓴다.
*   `016_74LS595`는 `02_Segment_Display/500_74LS595`와 내용이 같아 `_to_delete`로 옮겼다.

---

<!-- AUTO-INDEX:BEGIN -->

## 🗂 폴더 현황 (자동 생성)

기준일 2026-09-20. 예제 폴더 8개, 회로도 보유 8개.

| 폴더 | 소스 | Proteus 회로도 |
|---|---|---|
| `017_74LS596_two` | 3개 파일 | `AVR328P_74ls575_two.pdsprj` |
| `026_Keypad` | 2개 파일 | `Keypad.DSN`; `Keypad.pdsprj` |
| `25_Keypad` | 1개 파일 | `Keypad.DSN` |
| `26_Keypad(4x4)` | 1개 파일 | `Keypad_7Seg.DSN` |
| `510_Ultrasound` | 5개 파일 | `AVR328P_Basic.pdsprj`; `AVR328P_I2C_Terminal.pdsprj` |
| `510_Ultrasound_v2` | 3개 파일 | `AVR328P_I2C_Terminal.DSN` |
| `511_RotaryEncoder` | 3개 파일 | `AVR328P_I2C_Terminal.DSN`; `AVR328P_I2C_Terminal.pdsprj` |
| `950_ttl_uC` | — | `2_two_reg_Add_Sub_ctrl_circuit.pdsprj`; `two_reg_Add_Sub_ctrl_circuit.pdsprj`; `two_reg_Add_Sub_manual_control.pdsprj` |

<!-- AUTO-INDEX:END -->
