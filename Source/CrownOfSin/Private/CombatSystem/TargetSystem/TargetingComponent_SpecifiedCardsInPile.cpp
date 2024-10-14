#include "CombatSystem/TargetSystem/TargetingComponent_SpecifiedCardsInPile.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_CardSelectorList.h"
#include "UI/UW_Layout_Cos.h"
#include "CardSystem/CardBase.h"


UTargetingComponent_SpecifiedCardsInPile::UTargetingComponent_SpecifiedCardsInPile(): CardSelectorList(nullptr)
{
	bRequiresInput = true;
}

bool UTargetingComponent_SpecifiedCardsInPile::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	if (bPreview) return false;

	if (!CardEffect.TargetComponent->IsChildOf(UPileComponent::StaticClass())) return false;

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer) return false;

	UPileComponent* PileComponent = Cast<UPileComponent>(CardPlayer->GetComponentByClass(CardEffect.TargetComponent));
	if (PileComponent->Cards.IsEmpty()) return false;

	CardPlayer->PlayerUI->WBP_CardHand->bSelectionMode = true;

	BindToCardConfirm(CardPlayer->PlayerUI->WBP_CardSelectorList);

	FText Title = FText::FromString(FString(TEXT("Choose Your Cards")));
	CardPlayer->PlayerUI->WBP_CardSelectorList->UpdateCardList(PileComponent->Cards, Title);

	return true;
}

void UTargetingComponent_SpecifiedCardsInPile::BindToCardConfirm(UUW_CardSelectorList* CardList)
{
	CardSelectorList = CardList;
	CardList->OnReturnSelectedCard.AddDynamic(this, &UTargetingComponent_SpecifiedCardsInPile::ValidateAndTransferSelectedCard);
}

void UTargetingComponent_SpecifiedCardsInPile::ValidateAndTransferSelectedCard(ACardBase* Card)
{
	CardSelectorList->OnReturnSelectedCard.RemoveDynamic(this, &UTargetingComponent_SpecifiedCardsInPile::ValidateAndTransferSelectedCard);

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->PlayerUI->WBP_CardHand->bSelectionMode = false;

	TArray<AActor*> Cards;
	Cards.Add(Card);

	OnInputTargetsReceived.Broadcast(Cards);
}
