#include "CardSystem/CardEffects/CardEffect_AddDamageToAttackCard.h"

#include "CardSystem/CardBase.h"
#include "Utilities/CosGameplayTags.h"


UCardEffect_AddDamageToAttackCard::UCardEffect_AddDamageToAttackCard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


void UCardEffect_AddDamageToAttackCard::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddDamageToAttackCard::ResolveCardEffect(AActor* TargetActor)
{
	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
		return false;

	FGameplayTagContainer RequiredTags{CosGameTags::Effect_Attack};
	TargetCard->ModifyCardEffectValues(EffectValue, ECardDataType::Hand, FGameplayTagContainer(), RequiredTags);
	TargetCard->ModifyCardEffectValues(EffectValue, ECardDataType::Pile, FGameplayTagContainer(), RequiredTags);
	TargetCard->CallLocalEventOnCard(CosGameTags::Event_Card_ModifyInHand, ECallGlobal::CallBefore);
	return true;
}
