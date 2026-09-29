# 05. Interrupts

## 🎯 학습 목표
*   인터럽트 벡터 테이블과 `ISR()` 매크로의 동작을 이해한다.
*   외부 인터럽트(INT0/INT1)와 핀 변화 인터럽트(PCINT)의 차이를 구분한다.
*   ISR을 짧게 유지하고 `volatile` 변수로 메인 루프와 통신하는 방법을 익힌다.

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `EICRA` | INT0/INT1의 트리거 조건 — Low / 변화 / 하강 / 상승 에지 |
| `EIMSK` | `INT0`, `INT1` 개별 허용 |
| `PCICR`, `PCMSK0:2` | 핀 변화 인터럽트 그룹 및 핀별 허용 |
| `SREG` 의 `I` 비트 | 전역 인터럽트 허용 — `sei()` / `cli()` |


## 📌 참고
*   ISR 안에서는 `_delay_ms()`나 `printf`를 쓰지 않는다.
*   ISR과 메인 루프가 공유하는 변수는 반드시 `volatile`로 선언한다. 2바이트 이상이면 읽는 동안 `cli()`로 보호해야 한다.
*   INT0/INT1은 지정한 두 핀(PD2, PD3)에만 있고, PCINT는 거의 모든 핀에 있으나 어느 핀이 바뀌었는지는 직접 판별해야 한다.

---

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-30 -->

### 📂 예제 (10개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_Volatile` | ISR 과 main 이 함께 쓰는 변수에는 volatile 이 필요하다 | 1 | 31 | ✓ |  |  |  |
| `12_ISR_Stack` | 인터럽트가 걸리면 스택에 무엇이 쌓이는가. ISR 앞뒤에 붙는 코드 | 1 | 19 | ✓ |  |  |  |
| `20_External_Interrupt` | 외부 인터럽트를 이용한 7 segment 값 변경 | 3 | 77 | ✓ | ✓ |  |  |
| `22_External_Interrupt2` | 인터텁트 0와 1을 사용 | 1 | 36 |  | ✓ |  |  |
| `30_PCInterrupt` |  | 1 | 45 | ✓ | ✓ |  |  |
| `32_PCInterrupt2` | 외부 인터럽트를 이용한 7 segment 값 변경 | 2 | 63 |  | ✓ |  |  |
| `40_Timer0_CTC_Int` | Timer0을 이용하여 1초마다 overflow | 2 | 84 | ✓ | ✓ |  |  |
| `42_Timer0_CTC_Int2` |  | 1 | 40 |  | ✓ |  |  |
| `50_Timer0_Overflow_Int` | Timer0을 이용하여 1초마다 overflow | 3 | 190 | ✓ | ✓ |  |  |
| `52_Timer0_Overflow_Int_module` | TCNT0가 0이면 16.384ms 마다 overflow 발생 | 1 | 76 | ✓ | ✓ |  |  |

<!-- AUTO-INDEX:END -->
