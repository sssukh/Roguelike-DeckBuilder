#include "CardSystem/CardEffects/CardEffect_AddToAllStatusesWithTags.h"

#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UCardEffect_AddToAllStatusesWithTags::UCardEffect_AddToAllStatusesWithTags()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_AddToAllStatusesWithTags::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddToAllStatusesWithTags::ResolveCardEffect(AActor* TargetActor)
{
	TArray<UActorComponent*> StatusComponents;
	TargetActor->GetComponents(UStatusComponent::StaticClass(), StatusComponents);
	
	for (int i = StatusComponents.Num() - 1; i >= 0; --i)
	{
		UStatusComponent* StatusComponent = Cast<UStatusComponent>(StatusComponents[i]);
		if (!StatusComponent) continue;

		if (StatusComponent->GameplayTags.HasAnyExact(GameplayTags))
		{
			bool bIsCondition = EffectValue > 0;
			StatusComponent->AddStatusValue(EffectValue, bIsCondition, bIsCondition, true, nullptr);
		}
	}

	return true;
}
