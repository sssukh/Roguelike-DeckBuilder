#include "CardSystem/CardEffects/CardEffect_Exhaust.h"

#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UCardEffect_Exhaust::UCardEffect_Exhaust()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bTargeted = false;
}


// Called when the game starts
void UCardEffect_Exhaust::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Exhaust::ResolveCardEffect(AActor* TargetActor)
{
	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
		return false;

	TargetCard->Exhaust();
	return true;
}
