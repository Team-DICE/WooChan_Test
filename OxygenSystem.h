// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OxygenSystem.generated.h"
//기본 산소통 감소량
static const float DECREASE_OXYGEN_TANK_PER_SECOND_DEFAULT = 2.0f;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OXYGEN_API UOxygenSystem : public UActorComponent
{
	GENERATED_BODY()
	bool isInDeadZone;
	bool isBattle;

	float oxygenTankRemain;
	float oxygenTankMin;
	float oxygenTankMax;

	float oxygenPackageRestoreAmount;
	int oxygenPackageEA;

	FTimerHandle timerHandle;

	//test
	bool flag;

public:
	class AActor* OwnerActor;
	void ReduceOxygenTank();
	void ChargeOxygenTank(float increaseAmount);
	bool DecreaseOxygenPackage();
	void IncreaseOxygenPackage();
	void UseOxygenPackage();
	void GameOver();

public:	
	// Sets default values for this component's properties
	UOxygenSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


		
};
