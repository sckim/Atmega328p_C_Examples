# 01. Digital I/O

## 🎯 학습 목표
*   `DDRx` / `PORTx` / `PINx` 세 레지스터의 역할을 구분하여 GPIO를 직접 제어한다.
*   내부 풀업 저항을 활성화하여 외부 부품 없이 버튼을 읽는다.
*   C 코드와 어셈블리 코드가 동일한 동작을 어떻게 표현하는지 비교한다.

## 🛠 주요 부품
*   LED, 저항(220~330 Ω), 택트 스위치

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `DDRx` | 방향 설정 — 1이면 출력, 0이면 입력 |
| `PORTx` | 출력값. 입력 모드에서는 1을 쓰면 내부 풀업이 켜진다 |
| `PINx` | 핀의 실제 논리 상태를 읽는다. 1을 쓰면 해당 비트가 토글된다 |

## 📂 예제
| 폴더 | 내용 |
|---|---|
| `010_Blink`, `0_Blink`, `10_Blink`, `13_Blink`, `Blink*` | 출력 제어 기본형 |
| `002_asmBlink`, `005_asmBlink`, `asmBlink` | 어셈블리로 작성한 최소 제어 |
| `assembler_Exam1` | 어셈블리 명령어 실습 |
| `020_Input_LED`, `20~22_Input*` | 버튼 입력과 풀업 저항 |
| `23_Input_UpDown`, `24_Input_Toggle` | 에지 검출과 토글 동작 |
| `GPIORead` | 핀 상태 읽기 |
| `012_Blink_Arduino`, `912_Blink_Arduino`, `Arduino 328P` | 아두이노 스타일 구현과의 대조 |

## 📌 참고
*   버튼은 기계적 바운스를 동반한다. 수 ms 지연 또는 다수결 샘플링으로 디바운스를 처리한다.
*   `01_Arduino_Projects/01_Digital_IO`의 `digitalWrite()`가 내부적으로 수행하는 일이 곧 이 폴더의 코드이다.
*   C++ 클래스로 GPIO를 감싼 예제는 `15_Projects/GPIO_oop`에 있다. 같은 내용이 이 폴더에도 `012_Blink_oop`로 중복되어 있어 `_to_delete`로 옮겼다.

---

<!-- AUTO-INDEX:BEGIN -->

## 🗂 폴더 현황 (자동 생성)

기준일 2026-09-20. 예제 폴더 22개, 회로도 보유 13개.

| 폴더 | 소스 | Proteus 회로도 |
|---|---|---|
| `002_asmBlink` | 2개 파일 | — |
| `005_asmBlink` | 1개 파일 | — |
| `010_Blink` | 6개 파일 | `AVR328P_Basic.pdsprj`; `AVR328P_LCD_7Seg_Terminal.DSN`; `AVR328P_Testboard.pdsprj` |
| `012_Blink_Arduino` | 16개 파일 | `AVR328P_Basic.pdsprj` |
| `020_Input_LED` | 1개 파일 | `AVR328P_Key_7Seg.DSN` |
| `0_Blink` | 1개 파일 | `Arduino 328.pdsprj` |
| `10_Blink` | 1개 파일 | `AVR128_LED_7Seg.DSN` |
| `13_Blink` | 1개 파일 | — |
| `20_Input_LED` | 1개 파일 | `Input.DSN` |
| `21_Input_LED` | 1개 파일 | `Input.DSN` |
| `22_Inputs_LED` | 1개 파일 | `Input.DSN` |
| `23_Input_UpDown` | 1개 파일 | `Input.DSN`; `Input4.DSN` |
| `24_Input_Toggle` | 1개 파일 | `Input.DSN` |
| `912_Blink_Arduino` | 8개 파일 | `AVR328P_LCD_7Seg_Terminal.DSN` |
| `Arduino 328P` | 1개 파일 | — |
| `Blink` | 1개 파일 | `AVR328P_Basic.pdsprj` |
| `Blink2` | 1개 파일 | `AVR328P_Basic.pdsprj` |
| `Blink3` | 1개 파일 | — |
| `Blink9` | 1개 파일 | — |
| `GPIORead` | 1개 파일 | — |
| `asmBlink` | 1개 파일 | — |
| `assembler_Exam1` | 1개 파일 | — |

<!-- AUTO-INDEX:END -->
