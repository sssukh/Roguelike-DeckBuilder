#include "CardSystem/Piles/PileDiscardComponent.h"

#include "Utilities/CosGameplayTags.h"


UPileDiscardComponent::UPileDiscardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	FriendlyName = FText::FromString(FString(TEXT("Discard Pile")));
	PileTag = CosGameTags::Pile_Discard;
	Tooltip.RowName = FName(TEXT("DiscardPile"));
}


void UPileDiscardComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
