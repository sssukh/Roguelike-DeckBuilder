#include "CardSystem/CardEffects/CardEffect_Draw.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileHandComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_Draw::UCardEffect_Draw()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
	bTargeted = false;
}


// Called when the game starts
void UCardEffect_Draw::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Draw::ResolveCardEffect(AActor* TargetActor)
{
	if (EffectValue <= 0)
	{
		return true;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false;
	}

	UPileHandComponent* PileHandComponent = Cast<UPileHandComponent>(CardPlayer->GetComponentByClass(UPileHandComponent::StaticClass()));
	if (!PileHandComponent)
	{
		return false;
	}

	for (int i = 1; i <= EffectValue; ++i)
	{
		PileHandComponent->AttemptDraw();
	}

	return true;
}
