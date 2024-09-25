#include "ActionSystem/Action_CardTransfer.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_Layout_Cos.h"
#include "Utilities/CosLog.h"


// Sets default values
AAction_CardTransfer::AAction_CardTransfer(): Card(nullptr), SourcePileAmount(0), TargetPileAmount(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}


void AAction_CardTransfer::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_CardTransfer::SetUpAction_Implementation()
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	UPileComponent* SourcePileComponent;
	if (CardPlayer->GetPileWithPileTag(SourcePile, SourcePileComponent))
	{
		SourcePileAmount = SourcePileComponent->Cards.Num();
	}

	UPileComponent* TargetPileComponent;
	if (CardPlayer->GetPileWithPileTag(TargetPile, TargetPileComponent))
	{
		TargetPileAmount = TargetPileComponent->Cards.Num();
	}
}

void AAction_CardTransfer::PlayAction_Implementation()
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	CardPlayer->PlayerUI->WBP_CardHand->TransferCardAction(Card, SourcePile, TargetPile);
	CardPlayer->PlayerUI->UpdatePileWidgetAmount(SourcePile, SourcePileAmount);
	CardPlayer->PlayerUI->UpdatePileWidgetAmount(TargetPile, TargetPileAmount);
	Execute_EndAction(this);
}
