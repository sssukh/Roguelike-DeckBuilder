#include "CardSystem/CardEffects/CardEffect_GainStatusByStatusCount.h"

#include "Interfaces/Interface_CardTarget.h"
#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UCardEffect_GainStatusByStatusCount::UCardEffect_GainStatusByStatusCount()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardEffect_GainStatusByStatusCount::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_GainStatusByStatusCount::ResolveCardEffect(AActor* TargetActor)
{
	TArray<UActorComponent*> StatusComponents;
	TargetActor->GetComponents(UStatusComponent::StaticClass(), StatusComponents);

	int32 ValidStatusCount = 0;
	for (int i = StatusComponents.Num() - 1; i >= 0; --i)
	{
		UStatusComponent* StatusComponent = Cast<UStatusComponent>(StatusComponents[i]);
		if (!StatusComponent) continue;

		if (StatusComponent->GameplayTags.HasAnyExact(GameplayTags))
		{
			ValidStatusCount = ValidStatusCount + StatusComponent->StatusValue;
		}
	}

	if (!TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
	{
		return false;
	}

	if (!TargetActor->Implements<UInterface_CardTarget>())
	{
		return false;
	}

	IInterface_CardTarget::Execute_AddToStatus(TargetActor, *TargetComponent, ValidStatusCount, true, nullptr);
	return true;
}
