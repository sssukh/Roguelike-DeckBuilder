#include "CardSystem/CardEffects/CardEffectComponent.h"

#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "CombatSystem/TargetSystem/TargetingComponent_AllValidCardsInHand.h"

UCardEffectComponent::UCardEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
	DefaultTargetingClass = UTargetingComponent_AllValidCardsInHand::StaticClass();
}

void UCardEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(TargetingClass))
		TargetingClass = DefaultTargetingClass;
}

void UCardEffectComponent::InitializeCardEffect()
{
}

bool UCardEffectComponent::ResolveCardEffect(AActor* TargetActor)
{
	return true;
}

void UCardEffectComponent::Interrupt_Implementation()
{
	bInterrupt = true;
}

void UCardEffectComponent::CancelInterruption_Implementation()
{
	bInterrupt = false;
}
