#include "CardSystem/CardEffects/CardEffect_AddSpecifiedArtifact.h"

#include "Interfaces/Interface_CardTarget.h"
#include "StatusSystem/StatusComponent.h"

UCardEffect_AddSpecifiedArtifact::UCardEffect_AddSpecifiedArtifact()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UCardEffect_AddSpecifiedArtifact::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddSpecifiedArtifact::ResolveCardEffect(AActor* TargetActor)
{
	if (TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
	{
		if (TargetActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			//TargetComponent를 TSubclassOf<UStatusComponent>로 캐스팅
			TSubclassOf<UStatusComponent> StatusComponentClass = *TargetComponent;
			IInterface_CardTarget::Execute_AddToStatus(TargetActor, StatusComponentClass, EffectValue, false, this);
		}
		return true;
	}
	return false;
}
