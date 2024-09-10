#include "CardSystem/CardEffects/CardEffect_AddCardStatus.h"

#include "StatusSystem/StatusComponent.h"


UCardEffect_AddCardStatus::UCardEffect_AddCardStatus()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UCardEffect_AddCardStatus::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddCardStatus::ResolveCardEffect(AActor* TargetActor)
{
	if (TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
	{
		UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(TargetActor, TargetComponent);
		NewStatusComponent->RegisterComponent();
		NewStatusComponent->StatusValue = EffectValue;
		NewStatusComponent->bShowImmediately = false;
		NewStatusComponent->GameplayTags = GameplayTags;
		return true;
	}

	return false;
}
