#include "CardSystem/CardUseRules/UseRuleComponent.h"

#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UUseRuleComponent::UUseRuleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UUseRuleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


bool UUseRuleComponent::CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage)
{
	return true;
}

bool UUseRuleComponent::ResolveUseConsequence(FUseRule UseRuleData)
{
	return true;
}

void UUseRuleComponent::InitializeUseRule()
{
	if (ACardBase* OwnerCardCasting = Cast<ACardBase>(GetOwner()))
	{
		ParentCard = OwnerCardCasting;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("UseRuleComponent InitializeUseRule과정에서 GetOwner가 ACardBase로 형변환에 실패했습니다."));
	}
}
