# 사용법: .\_worklog\build.ps1 -Dirs '01_Digital_IO\10_Blink','04_ADC\30_ADC_Int'
# 저장소 루트(02_AVR_C_Development)에서 실행한다.
# PlatformIO 가 설치한 avr-gcc 로 src\main.c 를 컴파일만 해 본다(출력은 %TEMP%\avr_build).
param([string[]]$Dirs)
$ROOT = (Get-Location).Path
$bin = "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin"
$out = Join-Path $env:TEMP 'avr_build'
New-Item -ItemType Directory -Path $out -Force | Out-Null
foreach ($d in $Dirs) {
    $src = Join-Path (Join-Path $ROOT $d) 'src\main.c'
    $elf = Join-Path $out (($d -replace '\\', '_') + '.elf')
    $res = & "$bin\avr-gcc.exe" -mmcu=atmega328p -DF_CPU=16000000UL -Os -std=gnu11 -Wall -Wextra -o $elf $src 2>&1
    if ($LASTEXITCODE -eq 0) {
        $p = ((& "$bin\avr-size.exe" $elf | Select-Object -Skip 1) -replace '\s+', ' ').Trim().Split(' ')
        $w = @($res | Where-Object { $_ -match 'warning' }).Count
        "OK   {0,-52} text={1} data={2} bss={3} warnings={4}" -f $d, $p[0], $p[1], $p[2], $w
    } else {
        "FAIL {0}" -f $d
        $res | Select-Object -First 10
    }
}
