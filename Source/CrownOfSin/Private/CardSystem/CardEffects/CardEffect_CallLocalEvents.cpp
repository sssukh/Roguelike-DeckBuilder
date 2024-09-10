#include "CardSystem/CardEffects/CardEffect_CallLocalEvents.h"

#include "Core/DispatcherHubLocalComponent.h"


// Sets default values for this component's properties
UCardEffect_CallLocalEvents::UCardEffect_CallLocalEvents()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCardEffect_CallLocalEvents::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_CallLocalEvents::ResolveCardEffect(AActor* TargetActor)
{
	UDispatcherHubLocalComponent* DispatcherHubLocalComponent = Cast<UDispatcherHubLocalComponent>(TargetActor->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass()));
	if (IsValid(DispatcherHubLocalComponent))
	{
		return DispatcherHubLocalComponent->CallMultipleBoundEvents(GameplayTags.GetGameplayTagArray(), TargetActor, nullptr, ECallGlobal::CallBefore);
	}

	return false;
}
