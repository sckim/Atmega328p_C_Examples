# 15. Projects

## 🎯 학습 목표
*   앞선 단계에서 익힌 주변장치를 하나의 펌웨어로 통합한다.
*   드라이버 계층과 응용 계층을 분리하여 유지보수 가능한 구조를 설계한다.
*   C++ 클래스로 주변장치를 추상화했을 때의 이점과 비용(코드 크기, RAM)을 확인한다.


## 📌 참고
*   ATmega328P의 RAM은 2 KB뿐이다. C++ 가상 함수와 동적 할당은 신중하게 쓴다.
*   `-Os` 최적화와 `avr-size`로 코드·데이터 크기를 항상 확인하는 습관을 들인다.
*   `MainPro328F`, `sensor-interface1`은 각각 `MainPro328F_v6`, `Sensors`와 바이트 단위로 같아 제거했다.

---

<!-- AUTO-INDEX:BEGIN -->
<!-- gen_index.py가 만든다. 손으로 고치지 마세요. 기준일 2026-09-27 -->

### 📂 예제 (9개)

| 폴더 | 내용 | 소스 | 줄 | PlatformIO | 회로도 | Wokwi | README |
|---|---|---:|---:|:---:|:---:|:---:|:---:|
| `10_Menu_start` | 2019. 6. 16. | 4 | 136 |  | ✓ |  |  |
| `20_Sensors` | SDO pin must be tied high for address 0x1D or low fo | 6 | 494 |  |  |  |  |
| `30_MainPro328F` |  | 5 | 648 |  |  |  |  |
| `40_Modules` |  | 6 | 317 |  | ✓ |  |  |
| `50_AllFunctions` | 2019. 7. 5. | 6 | 1189 |  | ✓ |  |  |
| `60_AllFunctions_OOP` | 2019. 7. 5. | 7 | 1362 |  | ✓ |  |  |
| `70_ArduinoSketch1` | 2019-12-12 오후 4:33:29 | 26 | 4312 |  |  |  |  |
| `80_IMU_9DOF` | 57600bps | 1 | 833 |  |  |  |  |
| `82_IMU_9DOF_V21` | 57600bps | 1 | 913 |  |  |  |  |

<!-- AUTO-INDEX:END -->
