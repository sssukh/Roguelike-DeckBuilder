#include "CardSystem/CardEffects/CardEffect_RevealShop.h"

#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_RevealShop::UCardEffect_RevealShop()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardEffect_RevealShop::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_RevealShop::ResolveCardEffect(AActor* TargetActor)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
		return false;

	//ToDo: 구현해야합니다. PlayerUI의 WBP_Shop가 구현되어야 할수있습니다.
}
