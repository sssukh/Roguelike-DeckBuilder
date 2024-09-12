#include "CardSystem/Piles/PileShopComponent.h"

#include "Utilities/CosGameplayTags.h"



UPileShopComponent::UPileShopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	PileTag = CosGameTags::Pile_Shop;
}


// Called when the game starts
void UPileShopComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
