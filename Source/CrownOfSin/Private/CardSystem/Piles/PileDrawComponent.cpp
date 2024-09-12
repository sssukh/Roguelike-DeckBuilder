#include "CardSystem/Piles/PileDrawComponent.h"

#include "CardSystem/Piles/PileDiscardComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"


UPileDrawComponent::UPileDrawComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	FriendlyName = FText::FromString(FString(TEXT("Draw Pile")));
	PileTag = CosGameTags::Pile_Draw;
	Tooltip.RowName = FName(TEXT("DrawPile"));
}

void UPileDrawComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPileDrawComponent::ReshuffleDiscardIntoDraw()
{
	UPileDiscardComponent* PileDiscardComponent = Cast<UPileDiscardComponent>(GetOwner()->GetComponentByClass(UPileDiscardComponent::StaticClass()));
	if (!PileDiscardComponent) return;

	if (PileDiscardComponent->Cards.Num() <= 0) return;

	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Reshuffle, this);

	IInterface_Pile::Execute_AssignExistingCards(this, PileDiscardComponent->Cards);

	PileDiscardComponent->Cards.Empty();
}
