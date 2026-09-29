# 14. Bootloader

## 🎯 학습 목표
*   부트로더의 역할과 BOOTSZ / BOOTRST 퓨즈의 의미를 이해한다.

## 💻 주요 레지스터
| 레지스터 | 역할 |
|---|---|
| `SPMCSR` | 플래시 자가 기록(부트로더에서 사용) |


## 📌 참고
*   부트로더 작업은 퓨즈 비트를 다룬다. 잘못 설정하면 ISP 없이는 복구할 수 없다.
*   `10_MCUSR_ResetReason`, `20_Read_Signature_Fuses`는 2026-09에 작성했다. 01 트리와 1:1로 대응한다.
*   `30_Bootloader`는 Optiboot 원본이다. Makefile과 빌드된 `.hex`는 01의 `30_optiboot` 쪽이 완전하다.

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-29 -->

### 📂 예제 (3개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_MCUSR_ResetReason` | MCU 가 "왜 리셋되었는지"를 MCUSR 레지스터로 확인한다 | 1 | 62 | ✓ |  |  |  |
| `20_Read_Signature_Fuses` | 칩의 시그니처와 퓨즈 비트, 락 비트를 읽기 전용으로 확인한다 | 1 | 60 | ✓ |  |  |  |
| `30_Bootloader` | See README.TXT            */ | 1 | 563 |  |  |  |  |

<!-- AUTO-INDEX:END -->
