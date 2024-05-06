# WooChan_Test


노우찬 테스트 레포

## OxygenSystem

- 액터 컴포넌트를 상속하여 컴포넌트 방식으로 동작.
- 산소 시스템을 추가하기를 원하는 액터 클래스의 **.h** 에 추가해야 되는 코드
  - `#include "OxygenSystem.h"` 
  - ```cpp
    public:
    UPROPERTY(VisibleAnywhere)
    UOxygenSystem* OxygenSystem;
    ```
- 원하는 액터 클래스의 **.cpp**에 추가해야되는 코드
  - `OxygenSystem = CreateDefaultSubobject<UOxygenSystem>(TEXT("OxygenSystem"));`
- 위의 세 부분을 추가로 작성하여 사용한다.

- 산소팩 사용(press E)/ 산소팩 개수 추가(press R) 임시 테스트 코드
  - ```cpp
     //선언 필요
     bool flag =false;
     //TICK에 추가할 부분
     if (!flag && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::E))
     {
        OxygenSystem->UseOxygenPackage();
     }
     flag = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::E);
     if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::R))
     {
   	  OxygenSystem->IncreaseOxygenPackage();
     }
    ```
  - 위의 코드 추가하여 산소통 회복 함수 테스트 가능

### OxygenSystem.cpp
- `static const float DECREASE_OXYGEN_TANK_PER_SECOND_DEFAULT = 2.0f;`
  - 1초마다 감소할 산소량 : **.h**에 선언되어 있음
- Timer를 사용하여 `ReduceOxygenTank()`를 1초마다 호출함
  - 원래는 파라미터를 추가하여, 전투 패배 시 산소팩 회복량을 파라미터로 전달하여 상황에 맞게 산소통을 감소시키려 했음
  - 그러나 함수가 timer의 인자로 사용되는 경우 파라미터가 있으면 사용할 수 없어서 파라미터를 빼고 기본 감소량 적용에만 활용함
  - 전투 패배로 인한 산소 감소를 하고 싶으면,  `ReduceOxygenTank()` 함수를 활용하여 전투 패배 시 따로 호출할 함수를 추가로 작성할 것
- E를 눌러 `UseOxygenPackage()` 를 호출하면 해당 함수 내부에서 `DecreaseOxygenPackage()`를 무조건 호출함
  - `DecreaseOxygenPackage()`에서 산소팩 개수를 체크하고 감소시킨다.
  - 산소팩이 1개 이상이여서 감소에 성공하면 true를 반환하고, `ChargeOxygenTank()`함수를 호출한다.
    - 이 때, 산소팩을 사용하는 시간에 대한 로직(지연 시간)은 없음. timer의 delay 활용하여 구현할 것
    - 도중에 회복을 취소한다면, 산소팩을 다시 +1 시키는 코드가 필요함.
    - 지금 함수에서는 산소팩을 무조건 1개 감소시키고 그 다음 회복 함수를 호출하고 있다.
    - 연속 사용에 대한 제한도 현재는 없음 (지연 시간에 대한 코드가 없으므로)
