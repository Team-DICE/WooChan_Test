// Fill out your copyright notice in the Description page of Project Settings.


#include "OxygenSystem.h"

// Sets default values for this component's properties
UOxygenSystem::UOxygenSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	isInDeadZone = false; //자기장에 있는지 확인
	isBattle = false; //전투 중인지 확인

	oxygenTankMax = 100.0f; //산소통 최대 용량
	oxygenTankMin = 0.0f; //산소통 최소 용량
	oxygenTankRemain = 100.0f; //산소통 잔량

	oxygenPackageRestoreAmount = 20.0f;//산소팩 개당 회복량
	oxygenPackageEA = 4;//산소팩 총 개수

	//Test용 변수
	flag = false;
}
/// <summary>
/// 산소통의 산소 잔량을 감소시킨다. 전투 중엔 감소하지 않는다.
/// <para>자기장 내부에 있는 경우 기본 감소량을 한 번 더 적용한다.</para>
/// <para>감소 후 산소통이 0이면 게임 오버 함수를 호출한다.</para>
/// </summary>
void UOxygenSystem::ReduceOxygenTank()
{
	//전투 중이면 감소하지 않음
	if (isBattle)
		return;

	//감소량 계산
	float decreaseResult = oxygenTankRemain - DECREASE_OXYGEN_TANK_PER_SECOND_DEFAULT;

	//자기장 확인(추가 감소)
	if (isInDeadZone)
	{
		oxygenTankRemain -= DECREASE_OXYGEN_TANK_PER_SECOND_DEFAULT;
	}

	//게임 오버 조건 체크
	if (decreaseResult <= oxygenTankMin)
	{
		oxygenTankRemain = oxygenTankMin;
		GameOver();
		return;
	}

	//산소통 감소 적용
	oxygenTankRemain = decreaseResult;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Decrease Oxygen Tank! <Remain Oxygen Tank> : %2f"), oxygenTankRemain));

}

void UOxygenSystem::GameOver()
{
	//게임 오버 로직 필요함
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Oxygen Tank is Zero. <Game Over>")));
	return;
}

/// <summary>
/// 산소팩을 사용했을 때(press E) 호출되는 함수.
/// <para>1.산소팩이 없는 경우 2.산소통이 가득 차 있는 경우에는 동작하지 않는다.</para>
/// <para>산소팩이 있다면, 산소팩을 1개 감소시키고 산소통 회복 함수를 호출한다.</para> 
/// </summary>
void UOxygenSystem::UseOxygenPackage()
{
	//산소통이 100인데 회복을 시도하는 경우
	if (oxygenTankRemain == oxygenTankMax)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Oxygen Tank is FULL.")));
		return;
	}

	if (DecreaseOxygenPackage())
	{
		// n초 동안 회복하며, 도중에 움직이면 회복이 취소되는 로직을 먼저 적용해야 함.
		// 연속 사용을 막기 위한 쿨타임도 필요함
		// 만약 회복 도중에 취소된다면, 감소된 산소팩을 다시 증가시켜야함
		// UI에는 산소통 회복이 완전히 끝나면 감소된 산소팩 개수를 적용할 것
		// 이게 전부 끝나면 산소통 회복

		ChargeOxygenTank(oxygenPackageRestoreAmount);

	}
}

/// <summary>
/// 산소통을 회복한다.
/// </summary>
/// <param name="increaseAmount"></param>
void UOxygenSystem::ChargeOxygenTank(float increaseAmount)
{
	float increaseResult = oxygenTankRemain + increaseAmount;

	if (increaseResult > oxygenTankMax)
		oxygenTankRemain = oxygenTankMax;
	else
		oxygenTankRemain = increaseResult;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Charge Oxygen Tank! <Remain Oxygen Tank> : %f"), oxygenTankRemain));
}

/// <summary>
/// 산소팩 감소
/// <para>전투 패배 시, 보스 패배 시, 산소팩 사용 시(press E) 호출함</para>
/// <para>전투에 패배하여 호출했는데, 감소시킬 산소팩이 없으면(retrun false) 산소통 잔량 감소 함수를 추가로 호출해야함 </para>
/// </summary>
/// <returns>산소팩 감소 성공 시 true 반환</returns>
bool UOxygenSystem::DecreaseOxygenPackage()
{
	//산소팩 수량이 1미만
	if (oxygenPackageEA <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("No Oxygen Package")));
		return false;
	}

	//산소팩 수량이 1이상
	oxygenPackageEA--;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Decrease Oxygen Package! <Remain OxygenPackage> : %d"), oxygenPackageEA));
	return true;
}

/// <summary>
/// 전투 승리 시, 보스 승리 시 호출
/// </summary>
void UOxygenSystem::IncreaseOxygenPackage()
{
	oxygenPackageEA++;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Increase Oxygen Package! <Remain OxygenPackage> : %d"), oxygenPackageEA));
}


// Called when the game starts
void UOxygenSystem::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = Cast<AActor>(GetOwner());
	OwnerActor->GetWorldTimerManager().SetTimer(timerHandle, this, &UOxygenSystem::ReduceOxygenTank, 1.0f, true);


}

////테스트용 (액터의 Tick에 사용)
//if (!flag && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::E))
//{
//	UseOxygenPackage();
//}
//flag = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::E);
//if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::R))
//{
//	IncreaseOxygenPackage();
//}


