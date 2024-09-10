#include "CardSystem/CardEffects/CardEffect_InterruptIfCardHasTag.h"

#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UCardEffect_InterruptIfCardHasTag::UCardEffect_InterruptIfCardHasTag()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_InterruptIfCardHasTag::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_InterruptIfCardHasTag::ResolveCardEffect(AActor* TargetActor)
{
	FGameplayTagContainer CardGameplayTags = ParentCard->GetGameplayTags();
	if (CardGameplayTags.HasAnyExact(GameplayTags))
	{
		if (ParentCard->Implements<UInterface_Interrupt>())
		{
			IInterface_Interrupt::Execute_Interrupt(ParentCard);
		}

		return true;
	}

	return true;
}
