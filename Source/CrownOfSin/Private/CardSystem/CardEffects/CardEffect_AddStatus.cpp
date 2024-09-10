#include "CardSystem/CardEffects/CardEffect_AddStatus.h"

#include "Interfaces/Interface_CardTarget.h"
#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UCardEffect_AddStatus::UCardEffect_AddStatus()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCardEffect_AddStatus::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddStatus::ResolveCardEffect(AActor* TargetActor)
{
	if (TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
	{
		if (TargetActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			//TargetComponent를 TSubclassOf<UStatusComponent>로 캐스팅
			TSubclassOf<UStatusComponent> StatusComponentClass = *TargetComponent;
			IInterface_CardTarget::Execute_AddToStatus(TargetActor, StatusComponentClass, EffectValue, true, this);
		}
		return true;
	}

	return false;
}
