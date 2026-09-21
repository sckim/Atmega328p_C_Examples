# 작업 기록 (WORKLOG)

> 2026-09-21 세션의 진행 기록이다. 다음에 이어서 작업할 때 이 문서부터 읽는다.

## 1. 목표

`01_Arduino_Examples`(아두이노 함수 예제)와 **매칭되는 레지스터 기반 예제**를 `02_AVR_C_Development`에 담는다.
- 01과 같은 카테고리 번호·예제 이름 체계로 맞춘다.
- 예제를 난이도 순으로 정리하고, 중복은 하나만 남긴다.
- 01에만 있고 02에는 없는 예제는 새로 작성한다.

## 2. 현재 상태 (한눈에)

| 항목 | 상태 |
|---|---|
| 카테고리 폴더 정렬 (01과 같은 번호·이름) | ✅ 완료 |
| `20_Applications` (01에 대응 없는 예제) | ✅ 완료 |
| `99_Archive` 정리 | ✅ 완료 (폴더째 삭제됨) |
| 예제 재정리 (난이도 순 개명 + 중복 정리) | ✅ 완료 (`PLAN_REORG.md`) |
| `uC_Examples.atsln` 경로 갱신 | ✅ 완료 (등록 16개, 전부 경로 확인) |
| 01 미대응 24개 신규 작성 + 교체 4개 | ✅ 작성·컴파일 완료 (실기 동작 미검증) |
| 최상위 `README.md` | ✅ 갱신 |
| **하위 폴더 README 일괄 갱신** | ⏳ **다음 작업** |
| `MAPPING_01_to_02.md` 갱신 | ⏳ 옛 폴더명 기준 |
| 01의 `15_Projects`/`20_Applications` 확장 예제(약 100개) | ⏳ 미착수 |
| **git 커밋** | ⏳ **아직 안 함** (변경 3,400여 건이 커밋 전) |

git: 저장소 루트 = `02_AVR_C_Development`, origin = `https://github.com/sckim/AVR328P.git`, 브랜치 `master`. 마지막 커밋은 `44d0697`(작업 전 상태). 지금까지의 변경은 모두 작업 트리에만 있으며 `git checkout`/`git restore`로 되돌릴 수 있다. push는 하지 않았다.

## 3. 결정 사항 (사용자가 정한 것)

- **README는 폴더 정리가 끝난 뒤에 한꺼번에** 작업한다. 이번에는 최상위 `README.md`만 갱신했다.
- **이름 체계** : 10 단위 번호 + 01 스타일 이름 (`10_Blink`, `20_Button` …). 번호가 클수록 어렵다.
- **유지 기준** : 같은 예제가 여럿이면 PlatformIO(.ini)판 우선.
- **삭제 방식** : 신뢰도 A(사실상 동일)는 삭제, 신뢰도 B(변형)는 `_to_delete`로 격리.
- `_from_SampleCodes`는 이번 정리 대상에서 **제외**한다(이후 별도 요청으로 본 트리와 겹치는 것만 삭제함).
- 01 대응 카테고리가 없는 병렬 텍스트 LCD, `09_Simple_Sensors` 등은 `20_Applications`로 옮긴다.

## 4. 진행 내역 (시간 순)

1. **분석** : 01(1,380개)과 02(5,874개, 자체 `99_Archive` 3,619개) 비교. 매핑표 `MAPPING_01_to_02.md` 작성 (01 예제 56개 중 ✅ 26 / 🔶 6 / ➕ 24).
2. **`99_Archive` 중복 삭제** : 본 트리와 같거나 내부 중복인 파일 2,188개 삭제. 이후 나머지도 정리한 뒤 사용자가 폴더째 삭제.
3. **카테고리 정렬** : `03_Serial_Comm`→`03_UART_Communication`, `10_I2C_Devices`→`09_I2C_Communication`, `11_SPI_Devices`→`10_SPI_Communication`, `12_OneWire_Devices`→`11_OneWire_Communication`, `13_EEPROM_Storage`→`12_EEPROM`, `15_Integrated_Projects`→`15_Projects`, `14_Advanced_Internal`은 `13_WatchDog_Sleep`·`14_Bootloader`·`20_Applications`로 분리.
4. **복구** : 사용자가 정리하다 사라진 `_from_SampleCodes`(111개)와 `15_Projects`의 4개 폴더(`ArduinoSketch1`, `MainPro328F_v6`, `Modules`, `Sensors`)를 git에서 복구. 단 `.gitignore` 대상 파일(`*.xml`, `*.pdsbak` 등)은 복구 불가였다(`MainPro328F_v6` 39→16개, `Sensors` 18→16개).
5. **`_from_SampleCodes` 겹침 정리** : 바이트 동일 파일은 0개. 줄 단위 85% 이상 일치하는 폴더 4개(`084_PWM_arduino`, `084_PWM_Timer1`, `111_SPI_MAX7219`, `328P`)와 동일 파일 2개 삭제.
6. **예제 재정리 실행** : 173개 프로젝트 → 유지 124 / 이동 3 / 삭제 46(A 16 삭제, B 30 `_to_delete`). 개명 127건. `uC_Examples.atsln`은 14개 제거·16개 경로 수정.
7. **신규 예제 작성** : 아래 5절.
8. **최상위 README 갱신, 이 기록 작성.**

## 5. 신규·교체 예제 (레지스터 기반 C)

프로젝트마다 `platformio.ini`(bare-metal, `framework` 없음) + `src/main.c` 하나로 자립한다. UART 출력은 `printf_P`에 연결한 9600bps. 주석은 한국어이며 01 예제와 대응 관계·레지스터 설명을 담았다.

- 00: `70_Data_Types`, `72_Operators`, `74_Control_Flow`, `76_Functions`, `78_Arrays_Pointers`, `80_String`
- 01/02: `30_LED_bar`, `18_Two_7Segments`, `26_BCD_4511`
- 03: `34_Serial_Input`, `38_SerialEvent`
- 04/05/06: `20_ADC_multi`, `30_ADC_Int`, `10_Volatile`, `40_Osc1MHz`
- 09: `10_i2c_scanner`, `20_I2C_write`, `70_MAX30105`, `82_ADXL345`
- 10/11/12: `50_MCP3208`, `10_DHT11`, `20_eeprom_write`, `30_eeprom_24c02`
- 13/14: `20_Sleep_delay`, `30_IDLE_Sleep_ExtInterrupt`, `40_Deep_Sleep_ExtInterrupt`, `50_Power_Management`, `10_MCUSR_ResetReason`, `20_Read_Signature_Fuses`
- **교체(아두이노 스케치 → 레지스터 C)** : `00/10_Template`, `01/10_Blink`, `10/20_SerialShift_595`, `00/40_Demo`. 옛 스케치는 `_to_delete/replaced_arduino/`에 있다.

검증은 avr-gcc 7.3.0(`-mmcu=atmega328p -DF_CPU=16000000UL -Os -std=gnu11 -Wall -Wextra`)로 33개 모두 컴파일해 **경고 없음**까지 확인했다. **실제 보드나 시뮬레이터로는 실행하지 않았다.** (`_worklog/build.ps1`)

## 6. 확인이 필요한 사항 (내가 하지 않은 변경)

- **`00_Introduction/40_Demo`가 없다.** 작성해 컴파일까지 했던 폴더인데 이후 사라졌다. 사용자가 지운 것으로 보이며 복구하지 않았다. 최상위 README 정리 내역에서는 교체 목록에서 뺐다.
- **`LICENSE` 파일이 작업 트리에 없다**(git 기준 삭제 상태). 삭제 의도인지 확인이 필요하다. 복구: `git checkout HEAD -- LICENSE`.
- 위 두 가지는 이번 세션 중 사용자가 직접 정리하다 바뀐 것으로 추정한다.

## 7. 다음에 할 일 (우선순위 순)

1. **하위 폴더 README 일괄 갱신** — 카테고리별 `README.md`(00~15, 20)의 예제 표·자동 생성 색인(`AUTO-INDEX`)이 옛 폴더명을 가리킨다. 신규 예제 설명(🆕)도 넣는다. 그룹(`Simple_Sensors_Extended` 등) README도 확인.
2. **`MAPPING_01_to_02.md` 갱신** (현재 폴더명 기준으로 다시 생성, 미대응 목록 갱신).
3. **git 커밋** — 변경이 크므로 단계로 나누어 커밋하는 것을 권한다 (예: ① 구조 변경 ② 신규 예제 ③ 문서). 커밋 전에 `_to_delete`를 그대로 둘지 결정한다. push는 사용자가 판단.
4. **`_to_delete` 검토 후 삭제**, `uC_Examples.atsln.bak` 정리.
5. **01의 확장 예제**(`01/15_Projects`의 `*_Extended`, `01/20_Applications`) 중 레지스터로 옮길 것을 골라 작성. 서드파티 라이브러리 기반이 많다. 후보: 서보 RC, 엔코더, MCP41xx, 각종 I2C 센서.
6. `framework = arduino`로 남은 5개(`03/42_LibUART`, `06/34_Timer0`, `09/60_PCF8574`, `12/10_EEPROM`, `20/Simple_Sensors_Extended/10_Keypad`)를 bare-metal 설정으로 정리.
7. 필요 시 신규 예제용 `.cproj`를 만들어 솔루션에 등록.
8. 실기(또는 시뮬레이터)로 신규 예제 동작 검증. 특히 `10_DHT11`(us 타이밍), `70_MAX30105`, `40_Deep_Sleep_ExtInterrupt`.

## 6b. 이번 세션에서 얻은 주의점

- **PlatformIO 규칙의 함정** : "PlatformIO(.ini)판 우선"으로 남긴 것 중 `framework = arduino`이면서 소스가 아두이노 API인 것이 4개 있었다(위 교체 목록). 앞으로 `.ini`가 있다고 bare-metal로 단정하지 말고 소스를 확인한다.
- **줄 단위 유사도의 한계** : UART·LCD 라이브러리를 공유하는 프로젝트는 유사도가 높게 나온다. 중복 판단은 라이브러리를 뺀 본체로 해야 한다. 신뢰도 B 삭제 30개는 내용을 직접 열어 확인하지 않은 판단이다(`PLAN_REORG.md`에 사유 기재).
- **PowerShell 도구** : 변수 이름이 대소문자를 구분하지 않아(`$R`과 `$r`, `$L`과 `$l`) 덮어쓰는 사고가 있었다. 함수 이름 `Rd`는 `Remove-Item`의 별칭이라 차단된다. 삭제는 명시적 절대 경로로 하고 `Join-Path`를 쓴다.

## 8. 되돌리기·재현에 쓰는 파일

| 파일 | 용도 |
|---|---|
| `_worklog/plan.csv` | 예제 재정리 계획 데이터(카테고리, 옛 이름, 새 이름, 동작, 사유). `PLAN_REORG.md`의 원본 |
| `_worklog/reorg_log_step1.tsv` | 삭제(A)·격리(B) 실행 기록 |
| `_worklog/reorg_log_step2.tsv` | 개명·이동 실행 기록(옛 경로 → 새 경로) |
| `_worklog/move_log.tsv` | 카테고리 정렬·`20_Applications` 이동 기록 |
| `_worklog/uC_Examples.atsln.before` | 예제 재정리 직전 솔루션 |
| `_worklog/meta.csv`, `pairs.csv`, `sample_dup.csv` | 규모·유사도 분석 자료 |
| `_worklog/build.ps1` | 신규 예제 컴파일 검증 스크립트 |
| `_to_delete/` | 격리한 변형 예제(B)와 교체된 아두이노 스케치. 확인 후 삭제 |

## 9. 다음 세션 시작 요령

```
cd D:\Github\Microcontroller\10_Atmega328P\02_AVR_C_Development
git status --short | more           # 변경 현황
.\_worklog\build.ps1 -Dirs '01_Digital_IO\10_Blink'   # 컴파일 환경 확인
```
Claude에게는 "WORKLOG.md 읽고 하위 README 갱신부터 이어서 해줘"라고 요청하면 된다.
