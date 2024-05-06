// Fill out your copyright notice in the Description page of Project Settings.


#include "OxygenSystem.h"

// Sets default values for this component's properties
UOxygenSystem::UOxygenSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	isInDeadZone = false; //�ڱ��忡 �ִ��� Ȯ��
	isBattle = false; //���� ������ Ȯ��

	oxygenTankMax = 100.0f; //����� �ִ� �뷮
	oxygenTankMin = 0.0f; //����� �ּ� �뷮
	oxygenTankRemain = 100.0f; //����� �ܷ�

	oxygenPackageRestoreAmount = 20.0f;//����� ���� ȸ����
	oxygenPackageEA = 4;//����� �� ����

	//Test�� ����
	flag = false;
}
/// <summary>
/// ������� ��� �ܷ��� ���ҽ�Ų��. ���� �߿� �������� �ʴ´�.
/// <para>�ڱ��� ���ο� �ִ� ��� �⺻ ���ҷ��� �� �� �� �����Ѵ�.</para>
/// <para>���� �� ������� 0�̸� ���� ���� �Լ��� ȣ���Ѵ�.</para>
/// </summary>
void UOxygenSystem::ReduceOxygenTank()
{
	//���� ���̸� �������� ����
	if (isBattle)
		return;

	//���ҷ� ���
	float decreaseResult = oxygenTankRemain - DECREASE_OXYGEN_TANK_PER_SECOND_DEFAULT;

	//�ڱ��� Ȯ��(�߰� ����)
	if (isInDeadZone)
	{
		oxygenTankRemain -= DECREASE_OXYGEN_TANK_PER_SECOND_DEFAULT;
	}

	//���� ���� ���� üũ
	if (decreaseResult <= oxygenTankMin)
	{
		oxygenTankRemain = oxygenTankMin;
		GameOver();
		return;
	}

	//����� ���� ����
	oxygenTankRemain = decreaseResult;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Decrease Oxygen Tank! <Remain Oxygen Tank> : %2f"), oxygenTankRemain));

}

void UOxygenSystem::GameOver()
{
	//���� ���� ���� �ʿ���
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Oxygen Tank is Zero. <Game Over>")));
	return;
}

/// <summary>
/// ������� ������� ��(press E) ȣ��Ǵ� �Լ�.
/// <para>1.������� ���� ��� 2.������� ���� �� �ִ� ��쿡�� �������� �ʴ´�.</para>
/// <para>������� �ִٸ�, ������� 1�� ���ҽ�Ű�� ����� ȸ�� �Լ��� ȣ���Ѵ�.</para> 
/// </summary>
void UOxygenSystem::UseOxygenPackage()
{
	//������� 100�ε� ȸ���� �õ��ϴ� ���
	if (oxygenTankRemain == oxygenTankMax)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Oxygen Tank is FULL.")));
		return;
	}

	if (DecreaseOxygenPackage())
	{
		// n�� ���� ȸ���ϸ�, ���߿� �����̸� ȸ���� ��ҵǴ� ������ ���� �����ؾ� ��.
		// ���� ����� ���� ���� ��Ÿ�ӵ� �ʿ���
		// ���� ȸ�� ���߿� ��ҵȴٸ�, ���ҵ� ������� �ٽ� �������Ѿ���
		// UI���� ����� ȸ���� ������ ������ ���ҵ� ����� ������ ������ ��
		// �̰� ���� ������ ����� ȸ��

		ChargeOxygenTank(oxygenPackageRestoreAmount);

	}
}

/// <summary>
/// ������� ȸ���Ѵ�.
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
/// ����� ����
/// <para>���� �й� ��, ���� �й� ��, ����� ��� ��(press E) ȣ����</para>
/// <para>������ �й��Ͽ� ȣ���ߴµ�, ���ҽ�ų ������� ������(retrun false) ����� �ܷ� ���� �Լ��� �߰��� ȣ���ؾ��� </para>
/// </summary>
/// <returns>����� ���� ���� �� true ��ȯ</returns>
bool UOxygenSystem::DecreaseOxygenPackage()
{
	//����� ������ 1�̸�
	if (oxygenPackageEA <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("No Oxygen Package")));
		return false;
	}

	//����� ������ 1�̻�
	oxygenPackageEA--;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Decrease Oxygen Package! <Remain OxygenPackage> : %d"), oxygenPackageEA));
	return true;
}

/// <summary>
/// ���� �¸� ��, ���� �¸� �� ȣ��
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

////�׽�Ʈ�� (������ Tick�� ���)
//if (!flag && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::E))
//{
//	UseOxygenPackage();
//}
//flag = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::E);
//if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::R))
//{
//	IncreaseOxygenPackage();
//}


