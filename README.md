# 02. AVR C/ASM Development (Bare-metal)

이 폴더는 ATmega328P 마이크로컨트롤러를 **Bare-metal(AVR C 및 Assembly)** 수준에서 제어하기 위한 학습 예제 모음이다. 아두이노 프레임워크의 추상화 뒤에 감추어진 레지스터 제어와 하드웨어의 동작 원리를 직접 다룬다.

같은 상위 폴더의 [`01_Arduino_Examples`](../01_Arduino_Examples/)는 동일한 주제를 아두이노 함수(`digitalWrite`, `analogRead` 등)로 구현한 대응 예제 모음이다. 두 폴더는 **카테고리 번호와 예제 이름을 맞추어** 두었으므로, 같은 번호·이름끼리 나란히 놓고 추상화 계층의 차이를 비교할 수 있다. (예: `01_Arduino_Examples/01_Digital_IO/10_Blink` ↔ `02_AVR_C_Development/01_Digital_IO/10_Blink`)

## 📐 폴더 구성 규칙

*   카테고리 폴더는 `00`~`15`, `20`으로 구성하며 번호는 01과 같다.
*   예제 폴더는 `NN_이름`이다. **NN이 클수록 어렵다**(개념 진행 → 소스 규모 순). 10 단위 번호는 01과 같은 이름·같은 예제이고, 그 사이 번호(12, 14, 22 …)는 같은 주제의 변형이나 확장이다.
*   02에만 있는 예제(어셈블리, 라이브러리 계층 등)와 01에 대응 카테고리가 없는 예제(병렬 텍스트 LCD, 센서, GLCD 등)는 `20_Applications`에 있다.
*   `_to_delete`는 중복·교체로 판단해 격리한 폴더이다. 확인이 끝나면 통째로 삭제해도 된다.
*   `_from_SampleCodes`는 이번 정리 대상에서 제외한 옛 샘플이다.

## 🛠 개발 환경 (Development Environments)

두 가지 개발 환경을 지원한다. 예제마다 설정 파일이 따로 있으므로 필요에 따라 선택한다.

### 1. Visual Studio Code + PlatformIO (권장)
*   **특징**: 현대적인 에디터 환경, 빠른 코드 작성, 간편한 라이브러리 관리.
*   **사용법**: VS Code에서 해당 예제 폴더를 열면 `platformio.ini`를 인식하여 자동으로 환경을 구성한다.
*   **bare-metal 설정**: `platformio.ini`에 `framework`를 지정하지 않는다(`platform = atmelavr`, `board = uno`만 둔다). 새로 작성한 예제는 모두 이 형식이다.
*   **참고**: `42_LibUART`, `34_Timer0`, `60_PCF8574`, `10_EEPROM`, `10_Keypad`는 소스가 레지스터 위주인데 `framework = arduino`로 남아 있다.

### 2. Microchip Studio (구 Atmel Studio)
*   **특징**: 공식 IDE, 강력한 하드웨어 디버깅(Atmel-ICE 등), 레지스터 실시간 모니터링.
*   **사용법**: 루트의 `uC_Examples.atsln` 솔루션 파일을 열면 등록된 프로젝트 16개가 로드된다.
*   **참고**: 이번 정리에서 PlatformIO판을 우선해 남겼기 때문에 `.cproj`가 있는 예제만 등록되어 있다. 새로 작성한 예제는 `.cproj`가 없어 솔루션에 없다.

### 3. 명령행 빌드 (검증용)
PlatformIO가 설치한 `avr-gcc`로 직접 컴파일해 확인할 수 있다.
```
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -std=gnu11 -Wall -Wextra -o out.elf src/main.c
```

---

## 📚 학습 커리큘럼 (실제 폴더 구성 기준)

**🆕** 표시는 01에 대응하는 예제를 이번에 레지스터 기반으로 새로 작성한 것이다.

### 00. Introduction — `00_Introduction`
*   개발 환경과 첫 동작 확인 (`10_Template`, `20_How_to_Run`, `30_First_Program`)
*   비트 조작과 메모리 섹션 (`50_Bit_Twiddling`, `60_Data_BSS`)
*   C 언어 기초 🆕 (`70_Data_Types`, `72_Operators`, `74_Control_Flow`, `76_Functions`, `78_Arrays_Pointers`, `80_String`)

### 01. Digital I/O — `01_Digital_IO`
*   `DDRx`/`PORTx`/`PINx` 레지스터에 의한 LED 출력 (`10_Blink`, `12_Blink_Pattern`, `14_Blink_Arduino_Style`, `16_Blink13`, `18_Blink_Full`)
*   버튼 입력 (`20_Button`, `22_Input_UpDown`, `24_Input_Toggle`)
*   LED 막대 🆕 (`30_LED_bar`)
*   Assembly (`60_Asm_Blink`, `62_Asm_Blink2`, `64_Asm_Exam`), C++ 래퍼 (`70_GPIO_OOP`)

### 02. Segment Display — `02_Segment_Display`
*   7-세그먼트 구동 (`10_7Segments`), 2자리 🆕 (`18_Two_7Segments`), 4자리 (`20_Four7Segments`, `22_Four7Segments_Input`, `24_Four7Segments_itoa`), 6자리 (`30_Six7Segments`)
*   BCD 디코더 🆕 (`26_BCD_4511`), 버튼 연동 (`40_7SegWithButtons`)
*   시프트 레지스터 74LS595 (`50_74LS595`, `52_74LS595_Test`, `54_74LS595_Two`)

### 03. UART Communication — `03_UART_Communication`
*   UART 기초와 출력 (`10_Serial`, `20_Print`, `22_UART`, `24_USART`, `26_UART_print`, `30_UART_Reg`)
*   수신 🆕 (`34_Serial_Input` 폴링, `38_SerialEvent` 수신 인터럽트)
*   라이브러리화 (`40_UART_myLib`, `42_LibUART`, `44_LibUART_printf`), 통신 응용 (`50_Comm_UART`, `52_SerialTest`)

### 04. ADC — `04_ADC`
*   변환과 시리얼 출력 (`10_AnalogReadSerial`, `12_ADC_Basic`, `14_ADC_hold`)
*   다중 채널 🆕 (`20_ADC_multi`), 변환 완료 인터럽트 🆕 (`30_ADC_Int`), LCD 표시 (`40_ADC_on_LCD`)

### 05. Interrupts — `05_Interrupts`
*   `volatile` 🆕 (`10_Volatile`), 외부 인터럽트 (`20_External_Interrupt`, `22_External_Interrupt2`), 핀 변화 인터럽트 (`30_PCInterrupt`, `32_PCInterrupt2`)
*   타이머 인터럽트 (`40_Timer0_CTC_Int`, `42_Timer0_CTC_Int2`, `50_Timer0_Overflow_Int`, `52_Timer0_Overflow_Int_module`)

### 06. Timers & Counters — `06_Timers_Counters`
*   Overflow / CTC (`10_Timer_Overflow`, `12_Timer1_Overflow`, `20_Timer_CTC`, `22_Timer0_CTC2`), 비교 일치 (`24_Timer1_OCR`, `26_Timer1_Compare`)
*   주기 생성 (`30_Timer0_10mSec`, `32_Timer0_Sec0_5`, `34_Timer0`), 1MHz 발진 🆕 (`40_Osc1MHz`)
*   LCD 연동 (`50_LCDTimer`, `52_Timer_with_LCD`), 펄스 폭 측정 (`60_PulseIn`, `62_PulseIn_timer`)

### 07. PWM — `07_PWM`
*   `analogWrite` 원리 (`10_AnalogWrite`), 소프트웨어 PWM (`12_PWM_bitbang`), 2채널·ADC 연동 (`14_PWM_Dual`, `16_PWM_ADC`)
*   Timer0 PWM (`20_Timer0_PWM`, `30_Timer0_FastPWM`, `32_Timer0_FastPWM2`, `34_FastPWM`, `36_Timer0_CTC_PWM`), Timer1 PWM (`40_Timer1_PWM`, `42_Timer1_FastPWM`)
*   인터럽트 기반 (`50_PWM_Timer0Interrupt`), 아두이노 스타일 (`60_PWM_Arduino_Style`)

### 08. Motors — `08_Motors`
*   RC 서보 (`10_Servo1`), DC 모터 (`20_DC_Motor`), 스테핑 모터 (`30_Stepper_motor`)

### 09. I2C Communication — `09_I2C_Communication`
*   버스 스캔 🆕 (`10_i2c_scanner`), 레지스터 쓰기·읽기 🆕 (`20_I2C_write`)
*   장치 제어 (`30_LCD_I2C`, `40_DS1307`, `50_LM75`, `60_PCF8574`, `62_PCF8575`, `90_LCD_OOP`)
*   센서 🆕 (`70_MAX30105`, `82_ADXL345`), 관성 센서 복합 프로젝트 (`80_ADXL_ITG`)

### 10. SPI Communication — `10_SPI_Communication`
*   SPI 기초 (`10_Comm_SPI`), 시프트 레지스터 (`20_SerialShift_595`, `22_74LS595_oop`)
*   MAX7219 (`30_MAX7219`, `32_MAX7219_Software`), 디지털 가변저항 (`40_DigitalPot`), 12비트 ADC 🆕 (`50_MCP3208`)

### 11. One-Wire Communication — `11_OneWire_Communication`
*   온습도 센서 🆕 (`10_DHT11`), DS18B20 온도 센서 (`20_DS18B20`)

### 12. EEPROM — `12_EEPROM`
*   내장 EEPROM (`10_EEPROM`, `20_eeprom_write` 🆕), 외부 24C02 🆕 (`30_eeprom_24c02`)
*   설정값을 보존하는 메뉴 (`40_Menu_start`, `42_Menu_eeprom`)

### 13. WatchDog & Sleep — `13_WatchDog_Sleep`
*   워치독 (`10_Watchdog_Basic`)
*   슬립 🆕 (`20_Sleep_delay`, `30_IDLE_Sleep_ExtInterrupt`, `40_Deep_Sleep_ExtInterrupt`, `50_Power_Management`)

### 14. Bootloader — `14_Bootloader`
*   리셋 원인 🆕 (`10_MCUSR_ResetReason`), 시그니처·퓨즈 🆕 (`20_Read_Signature_Fuses`), 부트로더 (`30_Bootloader`)

### 15. Projects — `15_Projects`
*   메뉴·센서·모듈 (`10_Menu_start`, `20_Sensors`, `40_Modules`), 보드 통합 펌웨어 (`30_MainPro328F`)
*   전 주제 통합 (`50_AllFunctions`, `60_AllFunctions_OOP`), 아두이노 코어 이식 (`70_ArduinoSketch1`), 9DOF IMU (`80_IMU_9DOF`, `82_IMU_9DOF_V21`)

### 20. Applications — `20_Applications`
01에 대응 카테고리가 없는 예제 모음이다.
*   `Simple_Sensors_Extended` : 키패드, 로터리 엔코더, 초음파, 74LS595 확장
*   `Text_LCD_Parallel` : HD44780 병렬(4/8비트) 텍스트 LCD 구동과 라이브러리화
*   `Graphic_LCD` : 그래픽 LCD 문자·비트맵 출력
*   `megaOS` : 협력형 스케줄러 실험

---

## 📄 기타 문서

| 파일 | 내용 |
|---|---|
| [`WORKLOG.md`](./WORKLOG.md) | **진행 기록.** 지금까지 한 작업, 결정 사항, 현재 상태, 다음에 할 일 |
| [`PLAN_REORG.md`](./PLAN_REORG.md) | 예제 재정리 계획안(이름 변경, 삭제·격리 목록, 근거) |
| [`MAPPING_01_to_02.md`](./MAPPING_01_to_02.md) | 01 ↔ 02 예제 대응표. **옛 폴더명 기준이라 일부가 현재와 다르다** |
| `README_Microchip.md` | 「마이크로시스템설계」 16주차 강의 진행표 원본 |
| `uC_Examples.atsln` | Microchip Studio 솔루션 (등록 16개). `.bak`은 경로 갱신 전 백업 |

---

## ✅ 정리 내역

### 2026-09-19
*   루트 및 하위 폴더 README 18개를 bare-metal 관점으로 재작성(이전에는 01의 복사본).
*   프로젝트 165개를 소스 내용 해시로 비교해 완전히 같은 5건을 `_to_delete`로 격리.
*   비어 있던 `12_OneWire`에 `600_DS18B20`(현 `20_DS18B20`)을 새로 작성.

### 2026-09-21
*   **카테고리 정렬** : 01의 폴더명·번호에 맞추었다 (`03_UART_Communication`, `09_I2C_Communication`, `10_SPI_Communication`, `11_OneWire_Communication`, `12_EEPROM`, `13_WatchDog_Sleep`, `14_Bootloader`, `15_Projects`).
*   **`20_Applications` 신설** : 대응 카테고리가 없는 센서·병렬 텍스트 LCD·GLCD·megaOS를 이동.
*   **99_Archive 정리** : 중복·빌드 산출물을 삭제해 3,619개 → 약 80개로 줄였고, 이후 폴더째 삭제(직접).
*   **예제 재정리** : 예제 173개를 난이도 순 `NN_이름`으로 개명하고 중복 46개를 정리(완전 중복 16개 삭제, 변형 30개는 `_to_delete`). 근거는 `PLAN_REORG.md`.
*   **솔루션 갱신** : `uC_Examples.atsln`을 새 경로로 고치고 삭제된 프로젝트 14개를 제거(등록 30개 → 16개).
*   **신규 예제 작성** : 01에 대응하는 예제 중 02에 없던 24개를 레지스터 기반 C로 작성하고, 아두이노 스케치가 섞여 있던 `10_Template`, `10_Blink`, `20_SerialShift_595`를 교체했다. 모두 avr-gcc로 컴파일해 경고 없음을 확인했다(실기 동작은 미검증).

## 🧹 남은 과제

*   **하위 폴더 README 일괄 갱신** : 폴더명·번호가 바뀌어 하위 README의 예제 표와 자동 생성 색인이 옛 이름을 가리킨다.
*   **`MAPPING_01_to_02.md` 갱신** : 옛 폴더명 기준이다.
*   **01의 `15_Projects`·`20_Applications` 확장 예제**(약 100개) 중 레지스터로 옮길 것 선별·작성.
*   **`framework = arduino`로 남은 5개**(위 개발 환경 참고)를 bare-metal 설정으로 정리.
*   **신규 예제용 `.cproj`** 를 만들어 Microchip Studio 솔루션에 등록(필요 시).
*   **`_to_delete` 확인 후 삭제**, `uC_Examples.atsln.bak` 정리.
*   **확인 필요** : `LICENSE` 파일이 작업 트리에 없다(git 기준 삭제 상태). `00_Introduction/40_Demo`도 폴더가 없다(작성한 bare-metal 버전 포함).
*   `_from_SampleCodes`는 이번 정리에서 제외했다.

---
※ 각 폴더 내의 `README.md`에서 상세한 학습 목표와 하드웨어 연결 방법을 확인할 수 있다. (하위 README는 갱신 전이라 옛 폴더명이 남아 있을 수 있다.)

---

## 🗂 폴더 현황

기준일 2026-09-21. 예제 폴더 수는 각 카테고리의 하위 폴더 수이다(`20_Applications`는 그룹 아래 폴더까지 센다).

| 폴더 | 예제 |
|---|---|
| [`00_Introduction`](./00_Introduction/) | 11 |
| [`01_Digital_IO`](./01_Digital_IO/) | 13 |
| [`02_Segment_Display`](./02_Segment_Display/) | 11 |
| [`03_UART_Communication`](./03_UART_Communication/) | 13 |
| [`04_ADC`](./04_ADC/) | 6 |
| [`05_Interrupts`](./05_Interrupts/) | 9 |
| [`06_Timers_Counters`](./06_Timers_Counters/) | 14 |
| [`07_PWM`](./07_PWM/) | 13 |
| [`08_Motors`](./08_Motors/) | 3 |
| [`09_I2C_Communication`](./09_I2C_Communication/) | 11 |
| [`10_SPI_Communication`](./10_SPI_Communication/) | 7 |
| [`11_OneWire_Communication`](./11_OneWire_Communication/) | 2 |
| [`12_EEPROM`](./12_EEPROM/) | 5 |
| [`13_WatchDog_Sleep`](./13_WatchDog_Sleep/) | 5 |
| [`14_Bootloader`](./14_Bootloader/) | 3 |
| [`15_Projects`](./15_Projects/) | 9 |
| [`20_Applications`](./20_Applications/) | 20 |
| [`_from_SampleCodes`](./_from_SampleCodes/) | 47 |
