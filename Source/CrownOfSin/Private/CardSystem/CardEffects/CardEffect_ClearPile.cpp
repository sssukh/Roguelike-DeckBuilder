#include "CardSystem/CardEffects/CardEffect_ClearPile.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"

UCardEffect_ClearPile::UCardEffect_ClearPile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCardEffect_ClearPile::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_ClearPile::ResolveCardEffect(AActor* TargetActor)
{
	if (TargetComponent->IsChildOf(UPileComponent::StaticClass()))
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		if (!CardPlayer) return false;

		UActorComponent* FoundPileComponent = CardPlayer->GetComponentByClass(TargetComponent);
		if (FoundPileComponent->GetClass()->ImplementsInterface(UInterface_Pile::StaticClass()))
		{
			return IInterface_Pile::Execute_ClearPile(FoundPileComponent);
		}
	}
	return false;
}
