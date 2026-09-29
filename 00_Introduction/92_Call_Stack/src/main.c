/*=======================================================*/
// Call_Stack : 함수를 부르면 스택에 무엇이 쌓이는가. 인자는 레지스터로 간다
//
// 교재 9장 실습 9-2. 시뮬레이터(simavr)로 한 명령씩 따라가며 SP 와 스택을 본다.
//
// 인자와 반환값 (avr-gcc 호출 규약)
//   첫째 인자 a  -> r25:r24      둘째 인자 b -> r23:r22      셋째 인자 c -> r21:r20
//   반환값       -> r25:r24
//
// add 는 명령 셋이다. 16비트 덧셈을 8비트 두 번으로 한다.
//   add r24, r22     하위 바이트. 캐리가 나면 SREG 의 C = 1
//   adc r25, r23     상위 바이트. 캐리까지 더한다
//   ret
//
// add3 는 add 를 두 번 부른다. c 를 첫 호출 너머까지 들고 가야 하므로
// r28:r29 에 옮겨 두고, 원래 값은 push 로 스택에 맡겼다가 pop 으로 되돌린다.
// 두 번째 add 는 call 이 아니라 jmp 로 간다. add 의 ret 가 main 으로 곧장 돌아간다.
//
// 왜 volatile 전역과 noinline 인가
//   add(10, 2) 라고 쓰면 컴파일러가 12 를 미리 계산해 함수 호출이 사라진다.
//   파일을 나눠도 PlatformIO 는 링크 때 최적화(-flto)를 해서 역시 사라진다.
//   값을 실행 중에만 알 수 있게(volatile) 하고, 함수를 펼치지 못하게(noinline) 했다.
//
// 선행 학습 : 90_Empty_Main
/*=======================================================*/

volatile int x = 10, y = 2, z = 3;
volatile int result;

__attribute__((noinline))
int add(int a, int b)
{
    return a + b;
}

__attribute__((noinline))
int add3(int a, int b, int c)
{
    return add(add(a, b), c);
}

int main(void)
{
    result = add(x, y);          // 12
    result = add3(x, y, z);      // 15

    while (1) {
    }
}
