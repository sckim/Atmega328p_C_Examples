/*=======================================================*/
// Template : 새 AVR C 프로젝트의 출발점
//
// 아두이노 스케치의 구조는 AVR C 에서 다음과 같이 대응된다.
//
//   void setup() { ... }         ->  main() 안에서 while (1) 앞의 부분 (한 번만 실행)
//   void loop()  { ... }         ->  while (1) { ... }   (끝나지 않고 반복)
//
// 아두이노는 main() 을 프레임워크가 만들어 두고 setup()/loop() 만 부르게 하지만,
// AVR C 에서는 우리가 main() 을 직접 쓴다. main() 은 절대 return 하지 않아야 한다.
//
// F_CPU : CPU 클럭(Hz). _delay_ms() 가 이 값으로 지연 시간을 계산한다.
//         PlatformIO 는 board 설정에 맞게 자동으로 정의하고, Microchip Studio 는 프로젝트 속성에서 정의한다.
/*=======================================================*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // ---- setup : 한 번만 실행 ----

    while (1) {
        // ---- loop : 반복 실행 ----
    }
}
