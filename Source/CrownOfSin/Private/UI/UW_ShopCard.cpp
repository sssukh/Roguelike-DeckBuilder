// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ShopCard.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "CardSystem/Piles/PileDeckComponent.h"
#include "CardSystem/Piles/PileShopComponent.h"
#include "Components/TextBlock.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Artifacts/Status_Artifact_Coins.h"
#include "UI/UW_CardListCard.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_ShopCard::UUW_ShopCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_ShopCard::NativeConstruct()
{
	Super::NativeConstruct();

	WBP_CardListCard->CardActor = CardActor;

	WBP_CardListCard->Initialize(FGameplayTagContainer());

	switch(CardActor->Rarity)
	{
	case CosGameTags::Rarity_Common:
		Cost = 80;
		break;
	case CosGameTags::Rarity_Rare:
		Cost = 120;
		break;
	case CosGameTags::Rarity_Epic:
		Cost = 160;
		break;
	case CosGameTags::Rarity_Legendary:
		Cost = 200;
		break;
	default:
		Cost = 80;
		break;
	}

	CostText->SetText(FText::FromString(FString::FromInt(Cost)));

	WBP_CardListCard->OnCardClicked.AddDynamic(this, &UUW_ShopCard::AttemptPurchase);
}

void UUW_ShopCard::AttemptPurchase(UUW_CardListCard* CardListCard, ACardBase* CardActor)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UStatus_Artifact_Coins* StatusArtifactCoins = Cast<UStatus_Artifact_Coins>(CardPlayer->GetComponentByClass(UStatus_Artifact_Coins::StaticClass()));

	if(!StatusArtifactCoins || StatusArtifactCoins->StatusValue<Cost)
	{
		COS_SCREEN(TEXT("Not Enough Coins"));
		return;
	}

	if(!UFunctionLibrary_Singletons::GetCardGameInstance(this)->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		return;
	}

	TArray<FMinion> PersistentHeroes = IInterface_CardGameInstance::Execute_GetPersistentHeroesFromInstance(UFunctionLibrary_Singletons::GetCardGameInstance(this));

	CardActor->CardDataDeck.OwnerID =PersistentHeroes[0].UniqueID;

	UPileDeckComponent* CardPlayerPileDeck = Cast<UPileDeckComponent>(CardPlayer->GetComponentByClass(UPileDeckComponent::StaticClass()));

	if(CardPlayerPileDeck->GetClass()->ImplementsInterface(UInterface_Pile::StaticClass()))
	{
		return;
	}

	IInterface_Pile::Execute_AttemptAddExistingCard(CardPlayerPileDeck,CardActor);

	CardActor->DispatcherHubLocal->CallEventWithCallTags(CosGameTags::Event_Card_GenerateCard,CardActor);

	UPileShopComponent* CardPlayerPileShop = Cast<UPileShopComponent>(CardPlayer->GetComponentByClass(UPileShopComponent::StaticClass()));

	if(CardPlayerPileShop->GetClass()->ImplementsInterface(UInterface_Pile::StaticClass()))
	{
		return;
	}

	IInterface_Pile::Execute_RequestRemoveCardFromPile(CardPlayerPileShop,CardActor);

	UStatus_Artifact* StatusArtifact  = CardPlayer->FindComponentByClass<UStatus_Artifact>();

	StatusArtifact->SubtractStatusValue(Cost,false,false,nullptr);

	SetVisibility(ESlateVisibility::Hidden);
}

void UUW_ShopCard::UpdateFromCoinAmount(int32 CoinCount)
{
	if(Cost>=CoinCount)
	{
		CostText->SetColorAndOpacity(CanAffordColor);
	}
	else
	{
		CostText->SetColorAndOpacity(CannotAffordColor);
	}
}

void UUW_ShopCard::UpdateCardWidget_Implementation(ACardBase* CardActor)
{
	if(WBP_CardListCard->GetClass()->ImplementsInterface(UInterface_CardWidget::StaticClass()))
	{
		IInterface_CardWidget::Execute_UpdateCardWidget(WBP_CardListCard,CardActor);
	}
}
