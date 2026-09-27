# 02 예제 재정리 계획안 (난이도 순 · 중복 정리)

- 상태: **계획안. 아직 아무것도 이동·삭제하지 않았다.** 승인 후 카테고리 단위로 실행한다.
- 제외: `_from_SampleCodes`, 각 `README.md` (README는 폴더 정리 후 일괄 작업).
- 이름 규칙: `NN_이름`. NN은 10 단위(중간 삽입용으로 2 단위 간격도 사용)이고 난이도 순이다. 01에 대응 예제가 있으면 01의 번호·이름을 따랐다.
- 난이도 기준: 개념 진행(폴링 → 인터럽트 → 라이브러리 → 통합)을 먼저 보고, 같은 단계에서는 소스 규모(줄 수)와 사용 레지스터 수를 참고했다.
- 유지 기준: 같은 예제가 여럿이면 PlatformIO(`platformio.ini`)판을 우선했고, 없으면 소스가 가장 완전한 쪽을 남겼다.
- 삭제 신뢰도: **A** = 소스가 사실상 동일(95% 이상)하거나 소스 없음. **B** = 변형·축소판으로 판단(내용 확인 권장).

- 실행 순서: ① 삭제 → ② 임시 이름을 거친 일괄 개명(옛 이름과 새 이름이 겹치는 경우가 있다. 예: 01의 옛 `10_Blink` 삭제 후 `0_Blink`를 `10_Blink`로) → ③ 솔루션(`uC_Examples.atsln`) 경로 갱신 → ④ 경로 해석 검증. 카테고리 단위로 진행하고 매번 결과를 보고한다.
- 줄 수는 주석·빈 줄을 뺀 소스 줄 수이며, 라이브러리가 포함된 프로젝트는 크게 나온다(난이도 판단에는 개념 진행을 우선했다).
- 내용을 직접 확인하지 못한 판단은 비고에 "내용 확인"으로 적었다.

집계: 유지 124개, 이동 3개(개명 포함), 삭제 46개 (A 16 / B 30).

## 00_Introduction

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Template` | `001_Template` | 3 |  |
| `20_How_to_Run` | `1How_to_run_uC` | 5 |  |
| `30_First_Program` | `2How_to_run_uC` | 15 |  |
| `40_Demo` | `Demo1` | 7 |  |
| `50_Bit_Twiddling` | `30_BitTwidding` | 25 |  |
| `60_Data_BSS` | `Check_databss` | 16 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `05_main` | B | 3줄 빈 main. 10_Template와 같은 구조 |
| `How_to_run_uC` | B | 3줄 빈 main. 10_Template와 같은 구조 |
| `InitialValues` | B | 3줄 빈 main. 10_Template와 같은 구조 |

## 01_Digital_IO

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Blink` | `0_Blink` | 8 | 가장 단순한 Blink(8줄, PlatformIO) |
| `12_Blink_Pattern` | `010_Blink` | 81 | 6파일, PlatformIO |
| `14_Blink_Arduino_Style` | `012_Blink_Arduino` | 2494 |  |
| `16_Blink13` | `13_Blink` | 56 | 내용 확인 후 이름 조정 |
| `18_Blink_Full` | `Blink` | 171 | 171줄, 내용 확인 후 이름 조정 |
| `20_Button` | `020_Input_LED` | 17 |  |
| `22_Input_UpDown` | `23_Input_UpDown` | 25 |  |
| `24_Input_Toggle` | `24_Input_Toggle` | 33 |  |
| `60_Asm_Blink` | `asmBlink` | 10 |  |
| `62_Asm_Blink2` | `002_asmBlink` | 36 |  |
| `64_Asm_Exam` | `assembler_Exam1` | 53 |  |
| `70_GPIO_OOP` | `GPIO_oop` | 164 | C++ 래퍼(164줄) |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `10_Blink` | B | 9줄 Blink 변형. 0_Blink와 같은 예제 |
| `Blink2` | B | 10줄 Blink 변형 |
| `Blink3` | B | 11줄 Blink 변형 |
| `Blink9` | B | 13줄 Blink 변형 |
| `20_Input_LED` | B | 17줄. 020_Input_LED의 변형 |
| `21_Input_LED` | B | 020_Input_LED의 변형 |
| `22_Inputs_LED` | B | 020_Input_LED의 변형 |
| `912_Blink_Arduino` | A | 012_Blink_Arduino와 소스 줄 100% 동일(파일 수만 절반) |
| `005_asmBlink` | B | 002_asmBlink와 75% 일치 |
| `GPIORead` | B | 03/062_UART_printf와 89% 일치(53줄). printf 예제가 본체 |

## 02_Segment_Display

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_7Segments` | `013_7Segments` | 33 | PlatformIO |
| `20_Four7Segments` | `015_Four7Segments` | 36 | PlatformIO |
| `22_Four7Segments_Input` | `47_Four7Segments_Input` | 45 |  |
| `24_Four7Segments_itoa` | `48_Four7Segments_itoa` | 32 |  |
| `30_Six7Segments` | `42_Six7Segments` | 20 |  |
| `40_7SegWithButtons` | `022_7SegWithButtons` | 36 |  |
| `50_74LS595` | `74LS595` | 58 |  |
| `52_74LS595_Test` | `74LS595_Test` | 41 |  |
| `54_74LS595_Two` | `500_74LS596_two` | 66 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `016_74LS595` | A | 500_74LS595와 소스 100% 동일 |
| `500_74LS595` | B | 74LS595와 77% 일치, 변형 |
| `14_7Segments` | B | 10줄 1자리 표시 변형. 013_7Segments와 같은 주제 |
| `40_One7Segments` | B | 12줄 1자리 표시 변형 |
| `7Segments` | B | 25줄 1자리 표시 변형 |
| `15_7Segments` | B | 34줄 1자리 표시 변형 |
| `45_Four7Segments` | B | 015_Four7Segments의 변형 |
| `46_Four7Segments_short` | B | 015_Four7Segments의 변형 |

## 03_UART_Communication

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Serial` | `90_UART_start` | 32 |  |
| `20_Print` | `062_UART_printf` | 56 |  |
| `22_UART` | `92_UART` | 75 |  |
| `24_USART` | `91_USART` | 125 |  |
| `26_UART_print` | `91_UART_print` | 133 |  |
| `30_UART_Reg` | `060_UART` | 175 | PlatformIO |
| `40_UART_myLib` | `063_UART_myLib` | 307 |  |
| `42_LibUART` | `064_LibUART` | 1218 | PlatformIO |
| `44_LibUART_printf` | `065_LIbUART_printf` | 616 |  |
| `50_Comm_UART` | `SerialComm` | 649 |  |
| `52_SerialTest` | `SerialTest` | 357 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `RS232` | A | 064_LibUART와 소스 100% 동일 |
| `UART_Lib` | A | 065_LIbUART_printf와 소스 100% 동일 |
| `SetRC_UART` | B | 08/811_Motor_RC와 95% 일치(RC 서보 예제) |

## 04_ADC

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_AnalogReadSerial` | `108_ADC_serial` | 67 |  |
| `12_ADC_Basic` | `050_ADC` | 60 | PlatformIO |
| `14_ADC_hold` | `100_ADC_hold` | 24 |  |
| `40_ADC_on_LCD` | `105_ADC_KeyIn_on_LCD` | 313 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `102_ADC_on_LCD` | A | 105_ADC_KeyIn_on_LCD와 99% 일치 |

## 05_Interrupts

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `20_External_Interrupt` | `027_Interrupt` | 56 | PlatformIO |
| `22_External_Interrupt2` | `29_Interrupts` | 27 |  |
| `30_PCInterrupt` | `028_PCInterrupt` | 37 | PlatformIO |
| `32_PCInterrupt2` | `028_PC_Interrupt` | 48 | 외부 인터럽트 예제 포함 |
| `40_Timer0_CTC_Int` | `043_Timer0CTC_Int` | 60 |  |
| `42_Timer0_CTC_Int2` | `043_Timer0_CTC_int` | 29 |  |
| `50_Timer0_Overflow_Int` | `041_Timer0Overflow_Int` | 124 |  |
| `52_Timer0_Overflow_Int_module` | `044_Timer0Overflow_Int_module` | 67 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `27_Interrupt` | B | 14줄. 027_Interrupt의 축소 변형 |
| `28_Interrupt` | B | 21줄. 027_Interrupt의 축소 변형 |

## 06_Timers_Counters

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Timer_Overflow` | `040_Timer0Overflow` | 81 | PlatformIO |
| `12_Timer1_Overflow` | `71_Timer1Overflow` | 33 |  |
| `20_Timer_CTC` | `042_Timer0CTC` | 55 | PlatformIO |
| `22_Timer0_CTC2` | `042_Timer0_CTC` | 27 |  |
| `24_Timer1_OCR` | `73_Timer1OCR` | 34 |  |
| `26_Timer1_Compare` | `76_Timer1_Compare` | 19 |  |
| `30_Timer0_10mSec` | `70_Timer0_10mSec` | 30 |  |
| `32_Timer0_Sec0_5` | `75_Timer0_Sec0_5` | 16 |  |
| `34_Timer0` | `Timer0` | 129 | PlatformIO |
| `50_LCDTimer` | `A1_LCDTimer` | 131 |  |
| `52_Timer_with_LCD` | `78_Timer_with_LCD` | 421 |  |
| `60_PulseIn` | `83_PulseIn` | 566 |  |
| `62_PulseIn_timer` | `84_PulseIn_timer` | 593 |  |

## 07_PWM

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_AnalogWrite` | `080_PWM` | 36 | PlatformIO |
| `12_PWM_bitbang` | `080_PWM_bitbang` | 14 |  |
| `14_PWM_Dual` | `081_PWM_Dual` | 11 |  |
| `16_PWM_ADC` | `082_PWM_ADC` | 24 |  |
| `20_Timer0_PWM` | `082_PWM_Timer0PWM` | 97 |  |
| `30_Timer0_FastPWM` | `045_PWM_Timer0FPWM` | 111 | PlatformIO |
| `32_Timer0_FastPWM2` | `082_PWM_Timer0FPWM` | 135 |  |
| `34_FastPWM` | `088_FastPWM` | 123 |  |
| `36_Timer0_CTC_PWM` | `082_PWM_Timer0CTC` | 104 |  |
| `40_Timer1_PWM` | `082_PWM_Timer1PWM` | 11 |  |
| `42_Timer1_FastPWM` | `082_PWM_Timer1FPWM` | 192 |  |
| `50_PWM_Timer0Interrupt` | `081_PWM_Timer0Interrupt2` | 105 |  |
| `60_PWM_Arduino_Style` | `82_PWM` | 153 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `080_PWM_bitbang_v2` | B | 080_PWM_bitbang와 75% 일치(14줄) |
| `081_PWM_Timer0Interrupt` | B | 05/044_Timer0Overflow_Int_module와 74% 일치. 05단원 쪽을 유지 |
| `083_PWM_SetRC_UART` | A | 08/811_Motor_RC와 소스 99% 일치 |
| `81_PWM` | B | 82_PWM과 79% 일치(같은 Arduino 스타일 PWM) |

## 08_Motors

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Servo1` | `811_Motor_RC` | 1598 | RC 서보 |
| `20_DC_Motor` | `812_Motor_DCM` | 120 | PlatformIO |
| `30_Stepper_motor` | `810_Motor_Stepper` | 164 | PlatformIO |

## 09_I2C_Communication

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `30_LCD_I2C` | `225_TWI_LCD` | 288 |  |
| `40_DS1307` | `221_TWI_RTC` | 237 |  |
| `50_LM75` | `220_TWI_LM75` | 306 | PlatformIO |
| `60_PCF8574` | `224_TWI_PCF8574` | 432 | PlatformIO |
| `62_PCF8575` | `225_TWI_PCF8575` | 786 |  |
| `80_ADXL_ITG` | `ADXL_ITG_20150521` | 2872 | 41개 파일 |
| `80_IMU_9DOF` (→ `..\15_Projects`) | `IMU_9DOF` | 808 | 9DOF IMU(808줄) |
| `82_IMU_9DOF_V21` (→ `..\15_Projects`) | `IMU_9DOF_V21` | 961 |  |
| `90_LCD_OOP` | `226_TWO_LCD_OOP` | 292 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `TWI_LCD` | A | 225_TWI_LCD와 98% 일치 |
| `LCD_I2C_v2` | A | 225_TWI_LCD와 97% 일치 |
| `LCD_I2C` | B | 16줄 스텁, 225_TWI_LCD에 포함되는 예제 |
| `IMU_9DOF_euler` | A | IMU_9DOF와 소스 100% 동일 |
| `MainPro328F` | A | 15_Projects/MainPro328F_v6와 소스 100% 동일 |

## 10_SPI_Communication

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Comm_SPI` | `110_SPI` | 104 |  |
| `20_SerialShift_595` | `105_SerialShift_595` | 68 | PlatformIO |
| `22_74LS595_oop` | `018_74LS595_oop` | 64 |  |
| `30_MAX7219` | `110_SPI_MAX7219` | 309 | PlatformIO |
| `32_MAX7219_Software` | `115_SPI_Software_MAX7219` | 343 |  |
| `40_DigitalPot` | `110_SPI_MCP41xx` | 721 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `110_SPI_MAX7219_v2` | B | 110_SPI_MAX7219에 92% 포함(원본이 파일 더 많음) |

## 11_OneWire_Communication

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `20_DS18B20` | `600_DS18B20` | 152 | PlatformIO |

## 12_EEPROM

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_EEPROM` | `201_eeprom` | 1254 | PlatformIO |
| `40_Menu_start` | `200_Menu_start` | 212 |  |
| `42_Menu_eeprom` | `210_Menu_eeprom` | 194 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `eeprom` | A | 201_eeprom과 소스 99% 동일 |

## 13_WatchDog_Sleep

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Watchdog_Basic` | `Watchdog` | 38 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `900_Wachdog` | B | Watchdog과 84% 일치. 철자 오류 구버전 |

## 14_Bootloader

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `30_Bootloader` | `Bootloader` | 1094 | 1094줄 |

## 15_Projects

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Menu_start` | `200_Menu_start` | 119 |  |
| `20_Sensors` | `Sensors` | 560 |  |
| `30_MainPro328F` | `MainPro328F_v6` | 697 |  |
| `40_Modules` | `Modules` | 328 |  |
| `50_AllFunctions` | `777_Allfunction` | 1153 |  |
| `60_AllFunctions_OOP` | `777_AllFunctions_OOP_v2` | 1323 |  |
| `70_ArduinoSketch1` | `ArduinoSketch1` | 5304 | 5304줄 |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `777_AllFunctions_OOP` | B | _v2와 96% 일치 |

## 20_Applications\Simple_Sensors_Extended

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_Keypad` | `026_Keypad` | 116 | PlatformIO |
| `12_Keypad_4x4` | `26_Keypad(4x4)` | 76 |  |
| `20_RotaryEncoder` | `511_RotaryEncoder` | 102 |  |
| `30_Ultrasound` | `510_Ultrasound_v2` | 125 |  |
| `32_Ultrasound_UART` | `510_Ultrasound` | 796 |  |
| `40_74LS595_two` | `017_74LS596_two` | 97 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `25_Keypad` | B | 026_Keypad의 축소 변형 |
| `950_ttl_uC` | A | 소스 파일 없음 |

## 20_Applications\Text_LCD_Parallel

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_TextLCD` | `62_textLCD` | 79 |  |
| `12_TextLCD2` | `63_textLCD` | 102 |  |
| `14_myTextLCD` | `030_myTextLCD` | 166 | PlatformIO |
| `20_TextLCD3` | `65_textLCD2` | 193 |  |
| `22_TextLCD_Read` | `69_textLCD3_read` | 265 |  |
| `24_TextLCD4` | `67_textLCD4` | 320 |  |
| `30_TextLCD_4bits` | `64_textLCD_4bits` | 383 |  |
| `40_LibTextLCD` | `032_LibTextLCD` | 648 | PlatformIO |
| `50_TextLCD_ADC` | `035_textLCDADC` | 327 |  |
| `60_TestLCD` | `TestLCD` | 370 |  |

삭제:

| 삭제 | 신뢰도 | 사유 |
|---|---|---|
| `LCDTest` | A | 032_LibTextLCD와 99% 일치 |
| `66_textLCD3` | A | 69_textLCD3_read와 99% 일치(read 기능 없는 쪽) |
| `68_textLCDLib` | A | 032_LibTextLCD와 96% 일치 |
| `LCD1` | A | 3줄 빈 프로젝트 |

## 20_Applications\Graphic_LCD

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_GLCD_Text` | `057_GLCD_Text` | 238 |  |
| `20_GLCD_Img` | `056_GLCD_Img` | 329 |  |

## 20_Applications\megaOS

| 새 이름 | 원본 | 줄 수 | 비고 |
|---|---|---|---|
| `10_megaOs` | `910_megaOs` | 39 |  |
| `20_megaOS` | `900_megaOS` | 216 |  |

## 솔루션(uC_Examples.atsln) 영향

| 등록 프로젝트 | 처리 |
|---|---|
| `10_SPI_Communication\110_SPI_MAX7219_v2\110_SPI_MAX7219.cproj` | 삭제 → 솔루션에서 제거 |
| `15_Projects\777_AllFunctions_OOP_v2\777_AllFunctions_OOP.cppproj` | 개명 → `60_AllFunctions_OOP` |
| `07_PWM\082_PWM_Timer0PWM\082_PWM_Timer0PWM.cproj` | 개명 → `20_Timer0_PWM` |
| `07_PWM\080_PWM_bitbang_v2\080_PWM_bitbang.cproj` | 삭제 → 솔루션에서 제거 |
| `07_PWM\082_PWM_Timer1PWM\082_PWM_Timer1PWM.cproj` | 개명 → `40_Timer1_PWM` |
| `20_Applications\Text_LCD_Parallel\LCDTest\LCDTest.cproj` | 삭제 → 솔루션에서 제거 |
| `03_UART_Communication\RS232\RS232.cproj` | 삭제 → 솔루션에서 제거 |
| `03_UART_Communication\SerialComm\065_LibUART_printf.cproj` | 개명 → `50_Comm_UART` |
| `09_I2C_Communication\TWI_LCD\TWI_LCD.cppproj` | 삭제 → 솔루션에서 제거 |
| `03_UART_Communication\SerialTest\SerialTest.cproj` | 개명 → `52_SerialTest` |
| `02_Segment_Display\74LS595\74LS595.cproj` | 개명 → `50_74LS595` |
| `02_Segment_Display\74LS595_Test\74LS595_Test.cproj` | 개명 → `52_74LS595_Test` |
| `03_UART_Communication\UART_Lib\UART_Lib.cproj` | 삭제 → 솔루션에서 제거 |
| `01_Digital_IO\GPIORead\GPIORead.cproj` | 삭제 → 솔루션에서 제거 |
| `01_Digital_IO\Blink3\Blink3.cproj` | 삭제 → 솔루션에서 제거 |
| `09_I2C_Communication\LCD_I2C_v2\LCD_I2C.cppproj` | 삭제 → 솔루션에서 제거 |
| `03_UART_Communication\SetRC_UART\SetRC_UART.cproj` | 삭제 → 솔루션에서 제거 |
| `01_Digital_IO\Blink2\Blink2.cproj` | 삭제 → 솔루션에서 제거 |
| `13_WatchDog_Sleep\Watchdog\Watchdog.cproj` | 개명 → `10_Watchdog_Basic` |
| `20_Applications\Text_LCD_Parallel\LCD1\LCD1.cppproj` | 삭제 → 솔루션에서 제거 |
| `12_EEPROM\eeprom\eeprom.cproj` | 삭제 → 솔루션에서 제거 |
| `01_Digital_IO\GPIO_oop\GPIO_oop.cppproj` | 이동·개명 → 경로 수정 |
| `15_Projects\ArduinoSketch1\ArduinoCore\ArduinoCore.cppproj` | 개명 → `70_ArduinoSketch1` |
| `00_Introduction\How_to_run_uC\0How_to_run_uC.cproj` | 삭제 → 솔루션에서 제거 |
| `00_Introduction\Check_databss\1Check_databss.cproj` | 개명 → `60_Data_BSS` |
| `00_Introduction\1How_to_run_uC\1How_to_run_uC.cproj` | 개명 → `20_How_to_Run` |
| `00_Introduction\2How_to_run_uC\2How_to_run_uC.cproj` | 개명 → `30_First_Program` |
| `14_Bootloader\Bootloader\Bootloader.cproj` | 개명 → `30_Bootloader` |
| `01_Digital_IO\assembler_Exam1\assembler_Exam1.asmproj` | 개명 → `64_Asm_Exam` |
| `01_Digital_IO\asmBlink\asmBlink.asmproj` | 개명 → `60_Asm_Blink` |
