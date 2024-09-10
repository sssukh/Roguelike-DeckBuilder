#include "CardSystem/CardEffects/CardEffect_AddStatusFromDrawPileSize.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDrawComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UCardEffect_AddStatusFromDrawPileSize::UCardEffect_AddStatusFromDrawPileSize()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_AddStatusFromDrawPileSize::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddStatusFromDrawPileSize::ResolveCardEffect(AActor* TargetActor)
{
	if (!TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
	{
		return false;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false;
	}

	if (!TargetActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		return false;
	}

	int32 Amount = FMath::FloorToInt(CardPlayer->PileDrawComponent->Cards.Num() / static_cast<float>(EffectValue));

	TSubclassOf<UStatusComponent> StatusComponentClass = *TargetComponent;
	IInterface_CardTarget::Execute_AddToStatus(TargetActor, StatusComponentClass, Amount, true, this);
	return true;
}
