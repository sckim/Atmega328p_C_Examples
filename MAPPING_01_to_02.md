# 01 Arduino 예제 ↔ 02 AVR C 예제 매핑표

- **2026-09-21 반영:** 0번의 카테고리 정렬과 `20_Applications` 이동은 완료했다. 1번 표의 후보 이름은 개별 예제 폴더명이며 개명·통합은 아직 하지 않았다. 표 제목의 `(02: ...)`는 옛 카테고리명이다.
- 기준: `01_Arduino_Examples`의 폴더 체계. 02의 기존 폴더는 **폴더명으로만** 판단했고 코드 내용은 아직 확인하지 않았다.
- 상태: ✅ 대응 있음(이동·개명 대상) / 🔶 일부만 대응(내용 확인 필요) / ➕ 대응 없음(레지스터 기반 신규 작성)
- 이동 후 02의 옛 폴더명은 각 예제의 `README`에 기록해 이력을 남긴다.

## 0. 카테고리 폴더명 정렬

01과 02는 08번까지 번호가 같고 09번부터 어긋난다. 01의 이름으로 통일하는 것을 제안한다.

| 01 (기준) | 02 현재 | 조치 |
|---|---|---|
| 00_Introduction | 00_Introduction | 유지 |
| 01_Digital_IO | 01_Digital_IO | 유지 |
| 02_Segment_Display | 02_Segment_Display | 유지 |
| 03_UART_Communication | 03_Serial_Comm | 개명 |
| 04_ADC | 04_ADC | 유지 |
| 05_Interrupts | 05_Interrupts | 유지 |
| 06_Timers_Counters | 06_Timers_Counters | 유지 |
| 07_PWM | 07_PWM | 유지 |
| 08_Motors | 08_Motors | 유지 |
| 09_I2C_Communication | 10_I2C_Devices | 개명·번호 변경 |
| 10_SPI_Communication | 11_SPI_Devices | 개명·번호 변경 |
| 11_OneWire_Communication | 12_OneWire_Devices | 개명·번호 변경 |
| 12_EEPROM | 13_EEPROM_Storage | 개명·번호 변경 |
| 13_WatchDog_Sleep | 14_Advanced_Internal 일부 | 분리 |
| 14_Bootloader | 14_Advanced_Internal 일부 | 분리 |
| 15_Projects | 15_Integrated_Projects | 개명 |
| 20_Applications | 09_Simple_Sensors 등 | 신설, 센서류 이동 |

02의 `09_Simple_Sensors`는 01에 대응 카테고리가 없어서 `20_Applications/Simple_Sensors_Extended`로 옮기는 안이다.

## 1. 예제별 매핑

### 00_Introduction
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Data_Types | ➕ | (`Check_databss`, `InitialValues`는 메모리 관점이라 참고용) |
| 20_Operators | 🔶 | `30_BitTwidding` |
| 30_Control_Flow | ➕ | |
| 40_Functions | ➕ | |
| 50_Arrays_Pointers | ➕ | |
| 60_String | ➕ | |

### 01_Digital_IO
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Blink | ✅ | `010_Blink`(대표), `0_Blink`, `10_Blink`, `13_Blink`, `Blink`, `Blink2`, `Blink3`, `Blink9`, `012_Blink_Arduino`, `912_Blink_Arduino`. asm 계열(`002_asmBlink`, `005_asmBlink`, `asmBlink`, `assembler_Exam1`)은 별도 보조로 유지 |
| 20_Button | ✅ | `020_Input_LED`, `20_Input_LED`, `21_Input_LED`, `22_Inputs_LED`, `23_Input_UpDown`, `24_Input_Toggle`, `GPIORead` |
| 30_LED_bar | ➕ | |
| 40_ShiftOut | ✅ | `02_Segment_Display/016_74LS595`, `74LS595`, `500_74LS595`, `11_SPI_Devices/105_SerialShift_595` |

### 02_Segment_Display
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_7Segments | ✅ | `013_7Segments`, `7Segments`, `14_7Segments`, `15_7Segments`, `40_One7Segments` |
| 20_BCD_4511 | ➕ | |
| 30_Two_7Segments | 🔶 | `015_Four7Segments`, `45_Four7Segments`, `46_..._short`, `47_..._Input`, `48_..._itoa`, `42_Six7Segments`. 2자리 전용은 없음 |

### 03_UART_Communication
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Serial | ✅ | `060_UART`, `90_UART_start`, `91_USART`, `92_UART` |
| 20_Print | ✅ | `91_UART_print`, `062_UART_printf`, `065_LIbUART_printf` |
| 30_Serial_Input | ➕ | (`SerialTest` 확인 필요) |
| 40_SerialEvent | ➕ | RX 인터럽트 기반으로 신규 작성 |
| 50_Comm_UART | ✅ | `SerialComm`, `RS232`, `063_UART_myLib`, `064_LibUART`, `UART_Lib`, `SetRC_UART` |

### 04_ADC
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_AnalogReadSerial | ✅ | `050_ADC`, `108_ADC_serial`, `100_ADC_hold` |
| 20_ADC_multi | ➕ | 다중 채널 (`_from_SampleCodes/ADC` 확인) |
| 30_ADC_Int | ➕ | ADC 변환완료 인터럽트 |
| (02에만 있음) | | `102_ADC_on_LCD`, `105_ADC_KeyIn_on_LCD` → 15_Projects |

### 05_Interrupts
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Volatile | ➕ | |
| 20_External_Interrupt | ✅ | `027_Interrupt`, `27_Interrupt`, `28_Interrupt`, `29_Interrupts` |
| 30_PCInterrupt | ✅ | `028_PCInterrupt`, `028_PC_Interrupt` |

### 06_Timers_Counters
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Timer_Overflow | ✅ | `040_Timer0Overflow`, `71_Timer1Overflow`, `05_Interrupts/041_Timer0Overflow_Int`, `044_..._module` |
| 20_Timer_CTC | ✅ | `042_Timer0CTC`, `042_Timer0_CTC`, `05_Interrupts/043_Timer0CTC_Int`, `73_Timer1OCR`, `76_Timer1_Compare` |
| 40_Osc1MHz | ➕ | CLKPR 분주 (Arduino 쪽은 1MHz 클럭 설정) |
| (02에만 있음) | | `70_Timer0_10mSec`, `75_Timer0_Sec0_5`, `83/84_PulseIn*`, `78_Timer_with_LCD`, `A1_LCDTimer`, `Timer0` |

### 07_PWM
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_AnalogWrite | ✅ | `080_PWM`, `081_PWM_Dual`, `082_PWM_ADC`, 소프트웨어 PWM `080_PWM_bitbang(_v2)` |
| 20_Timer0_PWM | ✅ | `082_PWM_Timer0PWM`, `082_PWM_Timer1PWM` |
| 30_Timer0_FastPWM | ✅ | `045_PWM_Timer0FPWM`, `082_PWM_Timer0FPWM`, `082_PWM_Timer1FPWM`, `088_FastPWM` |

### 08_Motors
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Servo1 | ✅ | `811_Motor_RC` |
| 30_Stepper_motor | ✅ | `810_Motor_Stepper` |
| (02에만 있음) | | `812_Motor_DCM` (DC 모터) |

### 09_I2C_Communication  (02: 10_I2C_Devices)
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_i2c_scanner | ➕ | |
| 20_I2C_write | ➕ | (`_from_SampleCodes/800_I2C` 확인) |
| 30_LCD_I2Cm | ✅ | `225_TWI_LCD`, `LCD_I2C`, `LCD_I2C_v2`, `TWI_LCD` |
| 40_DS1307 | ✅ | `221_TWI_RTC` |
| 50_LM75 | ✅ | `220_TWI_LM75` |
| 60_PCF8574 | ✅ | `224_TWI_PCF8574` |
| 70_MAX30105 | ➕ | |
| 80_ADXL345 | 🔶 | `ADXL_ITG_20150521` (135개 파일, 내용 확인 필요) |
| (02에만 있음) | | 텍스트 LCD 병렬 계열(`030_myTextLCD`, `032_LibTextLCD`, `6x_textLCD*`, `TestLCD`)은 I2C가 아니므로 `20_Applications` 또는 `02_Segment_Display` 옆 별도 카테고리 검토. `IMU_9DOF*` → 15_Projects |

### 10_SPI_Communication  (02: 11_SPI_Devices)
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_Comm_SPI | ✅ | `110_SPI` |
| 40_DigitalPot | ✅ | `110_SPI_MCP41xx` |
| 50_MCP3208 | ➕ | |
| (02에만 있음) | | `110_SPI_MAX7219(_v2)`, `115_SPI_Software_MAX7219`, `018_74LS595_oop` → 15_Projects의 SPI Extended |

### 11_OneWire_Communication  (02: 12_OneWire_Devices)
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_DHT11 | ➕ | |
| 20_DS18B20 | ✅ | `600_DS18B20` |

### 12_EEPROM  (02: 13_EEPROM_Storage)
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 10_EEPROM | ✅ | `201_eeprom`, `eeprom` |
| 20_eeprom_write | 🔶 | `201_eeprom` 내용 확인 필요 |
| 30_eeprom_24c02 | 🔶 | `_from_SampleCodes/222_TWI_eeprom`, `800_I2C_eeprom` (아직 본 트리에 없음) |
| (02에만 있음) | | `200_Menu_start`, `210_Menu_eeprom` → 15_Projects |

### 13_WatchDog_Sleep / 14_Bootloader  (02: 14_Advanced_Internal)
| 01 예제 | 상태 | 02 후보 |
|---|---|---|
| 13/10_Watchdog_Basic | ✅ | `Watchdog`, `900_Wachdog` |
| 13/20_Sleep_delay | ➕ | |
| 13/30_IDLE_Sleep_ExtInterrupt | ➕ | |
| 13/40_Deep_Sleep_ExtInterrupt | ➕ | |
| 13/50_Power_Management | ➕ | |
| 14/10_MCUSR_ResetReason | ➕ | |
| 14/20_Read_Signature_Fuses | ➕ | |
| 14/30_optiboot | 🔶 | `Bootloader` (6개 파일) |
| (02에만 있음) | | `056_GLCD_Img`, `057_GLCD_Text`, `900_megaOS`, `910_megaOs` → 15_Projects 또는 20_Applications |

### 15_Projects / 20_Applications
01은 `NN_..._Extended`(다수의 라이브러리 기반 예제)와 신호처리·센서 응용이 주류다. 02의 `15_Integrated_Projects`(`777_*`, `MainPro328F_v6`, `Modules`, `Sensors`, `ArduinoSketch1`, `GPIO_oop`)와 `09_Simple_Sensors`가 여기에 해당한다. 개별 대응은 1차 재배치 후 따로 정리하는 것을 제안한다. 센서 대응 후보는 다음과 같다.

| 01 (Simple_Sensors_Extended) | 02 후보 |
|---|---|
| 80_Keypad | `09_Simple_Sensors/026_Keypad`, `25_Keypad`, `26_Keypad(4x4)` |
| 60_HC_SR04, 62_Ultrasound | `510_Ultrasound`, `510_Ultrasound_v2` |
| 70_Encoder, 71_RotaryEncoder | `511_RotaryEncoder` |
| 그 외 (Joystick, Flex, RF433 …) | ➕ |

## 2. 집계 (Introduction~Bootloader, 01의 56개 예제)

| 상태 | 개수 |
|---|---|
| ✅ 대응 있음 | 26 |
| 🔶 일부만 대응 | 6 |
| ➕ 신규 작성 필요 | 24 |

(신규 24개: Introduction 5, Digital_IO 1, Segment 1, UART 2, ADC 2, Interrupts 1, Timers 1, I2C 3, SPI 1, OneWire 1, WatchDog/Sleep 4, Bootloader 2. 🔶 6개는 내용을 확인해야 최종 확정된다.)
