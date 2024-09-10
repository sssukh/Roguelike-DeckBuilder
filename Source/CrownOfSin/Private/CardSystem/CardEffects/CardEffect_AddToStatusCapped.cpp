#include "CardSystem/CardEffects/CardEffect_AddToStatusCapped.h"

#include "Libraries/FunctionLibrary_CardEffect.h"
#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UCardEffect_AddToStatusCapped::UCardEffect_AddToStatusCapped()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_AddToStatusCapped::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddToStatusCapped::ResolveCardEffect(AActor* TargetActor)
{
	if (!TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
		return false;

	UFunctionLibrary_CardEffect::AddToStatusCappedToMax(TargetActor, *TargetComponent, EffectValue, true, this);
	return true;
}
