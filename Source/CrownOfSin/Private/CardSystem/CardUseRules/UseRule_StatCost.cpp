#include "CardSystem/CardUseRules/UseRule_StatCost.h"

#include "CardSystem/CardPlayer.h"
#include "Core/CosEnumStruct.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/CosLog.h"

UUseRule_StatCost::UUseRule_StatCost()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UUseRule_StatCost::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UUseRule_StatCost::CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage)
{
	// 플레이어 객체를 가져옴
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		COS_LOG_SCREEN_ERROR(TEXT("CardPlayer가 UUseRule_StatCost::CheckIfUseAllowed에서 nullptr입니다."));
		return false;
	}

	// 지정된 상태 컴포넌트를 가져옴
	UStatusComponent* TargetStatusComponent = Cast<UStatusComponent>(CardPlayer->GetComponentByClass(UseRuleData.Status));
	if (!IsValid(TargetStatusComponent))
	{
		COS_LOG_SCREEN_ERROR(TEXT("StatusComponent는 UUseRule_StatCost::IsUseAllowedByStatCost의 지정된 상태 클래스에 대해 nullptr입니다."));
		return false;
	}

	// 상태 값이 충분한지 확인
	if (TargetStatusComponent->StatusValue >= UseRuleData.Cost)
	{
		// 사용 가능
		return true;
	}

	FailMessage = FString::Printf(TEXT("Not Enough %s"), *TargetStatusComponent->FriendlyName.ToString());
	return false;
}

bool UUseRule_StatCost::ResolveUseConsequence(FUseRule UseRuleData)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		COS_LOG_SCREEN_ERROR(TEXT("CardPlayer가 UUseRule_StatCost::ResolveUseConsequence에서 nullptr입니다."));
		return false;
	}
	
	IInterface_CardTarget::Execute_SubtractFromStatus(CardPlayer, UseRuleData.Status, UseRuleData.Cost, true, nullptr);
	return true;
}
