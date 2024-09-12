#include "CardSystem/Piles/PileVoidComponent.h"

#include "Utilities/CosGameplayTags.h"



UPileVoidComponent::UPileVoidComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	FriendlyName = FText::FromString(FString(TEXT("Void")));
	PileTag = CosGameTags::Pile_Void;
	Tooltip.RowName = FName(TEXT("VoidPile"));
}


void UPileVoidComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
