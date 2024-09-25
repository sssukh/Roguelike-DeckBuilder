#include "ActionSystem/Action_Reshuffle.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_Layout_Cos.h"
#include "Utilities/CosGameplayTags.h"


AAction_Reshuffle::AAction_Reshuffle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAction_Reshuffle::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_Reshuffle::SetUpAction_Implementation()
{
	CardPlayerReference = UFunctionLibrary_Singletons::GetCardPlayer(this);
	PlayerUIReference = CardPlayerReference->PlayerUI;
	HandWidgetReference = PlayerUIReference->WBP_CardHand;
}

void AAction_Reshuffle::PlayAction_Implementation()
{
	DiscardPilePosition = HandWidgetReference->GetAnchorWidgetPosition(CosGameTags::Pile_Discard);
	DrawPilePosition = HandWidgetReference->GetAnchorWidgetPosition(CosGameTags::Pile_Draw);
}
