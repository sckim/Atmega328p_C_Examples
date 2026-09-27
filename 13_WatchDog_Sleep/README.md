# 13. WatchDog & Sleep

## 🎯 학습 목표
*   워치독 타이머로 시스템 행(hang)을 감지하고 자동 복구한다.
*   리셋 원인(`MCUSR`)을 읽어 워치독 리셋을 구분한다.

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `WDTCSR` | `WDE` 리셋 허용, `WDIE` 인터럽트 허용, `WDP3:0` 타임아웃 주기 |
| `MCUSR` | 리셋 원인 — `PORF`, `EXTRF`, `BORF`, `WDRF` |

## 📂 예제
| 폴더 | 내용 |
|---|---|
| `Watchdog` | 워치독 타이머 설정과 `wdt_reset()` (Microchip Studio 솔루션 등록 프로젝트) |
| `900_Wachdog` | 철자 오류(Wachdog)가 있는 구버전 |

## 📌 참고
*   워치독은 리셋 후에도 활성 상태가 유지된다. 시작 즉시 `MCUSR`을 읽어 지우고 `wdt_disable()`을 호출한 뒤 재설정해야 무한 리셋 루프에 빠지지 않는다.
*   01의 `13_WatchDog_Sleep`에 있는 Sleep 예제(Sleep_delay, IDLE/Deep Sleep, Power_Management)는 아직 없다. 신규 작성 대상이다.