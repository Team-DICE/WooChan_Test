# WooChan_Test

노우찬 테스트 레포

## OxygenSystem

- 액터 컴포넌트를 상속하여 컴포넌트 방식으로 동작.
- 원하는 액터 클래스의 **.h**에
- `#include "OxygenSystem.h"` 추가
  - `````cpp
    public:
    UPROPERTY(VisibleAnywhere)
    UOxygenSystem* OxygenSystem;
    ```` 추가
    `````
- 원하는 액터 클래스의 **.cpp**에 -`OxygenSystem = CreateDefaultSubobject<UOxygenSystem>(TEXT("OxygenSystem"));` 추가
- 이후 사용 가능

- 산소팩 사용/ 산소팩 추가 테스트 코드
- ```cpp
   //선언 필요
   bool flag =false;
   //TICK에 추가
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
- 위의 코드 추가하여 테스트 가능

### OxygenSystem.h

-
