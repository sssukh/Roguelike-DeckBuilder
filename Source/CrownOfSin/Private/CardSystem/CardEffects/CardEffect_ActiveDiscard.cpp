#include "CardSystem/CardEffects/CardEffect_ActiveDiscard.h"

#include "CardSystem/CardBase.h"
#include "Utilities/CosGameplayTags.h"


UCardEffect_ActiveDiscard::UCardEffect_ActiveDiscard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...

	RelevantCardEventTags.AddTag(CosGameTags::Event_Card_Discard);
	RelevantCardEventTags.AddTag(CosGameTags::Event_Card_Exhaust);

	bTargeted = false;
	//toDo: 타겟팅 설정
	// DefaultTargetingClass = 변경해야합니다.
}


void UCardEffect_ActiveDiscard::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_ActiveDiscard::ResolveCardEffect(AActor* TargetActor)
{
	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
		return false;

	FGameplayTag CallTags = CosGameTags::Flag_Active;
	TargetCard->Discard(CallTags.GetSingleTagContainer());
	return true;
}
