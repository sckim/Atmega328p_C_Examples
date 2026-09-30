# 13. WatchDog & Sleep

## 🎯 학습 목표
*   워치독 타이머로 시스템 행(hang)을 감지하고 자동 복구한다.
*   리셋 원인(`MCUSR`)을 읽어 워치독 리셋을 구분한다.

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `WDTCSR` | `WDE` 리셋 허용, `WDIE` 인터럽트 허용, `WDP3:0` 타임아웃 주기 |
| `MCUSR` | 리셋 원인 — `PORF`, `EXTRF`, `BORF`, `WDRF` |


## 📌 참고
*   워치독은 리셋 후에도 활성 상태가 유지된다. 시작 즉시 `MCUSR`을 읽어 지우고 `wdt_disable()`을 호출한 뒤 재설정해야 무한 리셋 루프에 빠지지 않는다.
*   Sleep 예제 네 개(`20_Sleep_delay`, `30_IDLE_Sleep_ExtInterrupt`, `40_Deep_Sleep_ExtInterrupt`, `50_Power_Management`)는 2026-09에 레지스터 기반 C로 작성했다. 01 트리와 1:1로 대응한다.
*   `10_Watchdog_Basic`만 아직 영문 레거시이고 `platformio.ini`가 없다.

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-30 -->

### 📂 예제 (6개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_Watchdog_Basic` | 워치독을 켜고 제때 wdt_reset() 으로 먹이를 준다. 원래 Microchip Studi | 2 | 102 | ✓ |  |  |  |
| `12_Watchdog_Count` | 2초 워치독을 켜고 0.25초마다 센다. 먹이를 주지 않으면 8에서 리셋된다 | 1 | 48 | ✓ |  |  |  |
| `20_Sleep_delay` | delay 대신 "잠자면서 기다리는" 지연 함수 | 1 | 95 | ✓ |  |  |  |
| `30_IDLE_Sleep_ExtInterrupt` | Idle 슬립에 들었다가 외부 인터럽트(INT0)로 깨어난다 | 1 | 40 | ✓ |  |  |  |
| `40_Deep_Sleep_ExtInterrupt` | Power-down 슬립에 들었다가 외부 인터럽트(INT0)로 깨어난다 | 1 | 73 | ✓ |  |  |  |
| `50_Power_Management` | 워치독(알람) + Power-down 슬립 + 주변장치 전원 차단 | 1 | 86 | ✓ |  |  |  |

<!-- AUTO-INDEX:END -->
