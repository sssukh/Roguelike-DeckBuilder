// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_Discover.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_CardRewardScreen.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_CardRewardScreen.h"
#include "UI/UW_Layout_Cos.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UTargetingComponent_Discover::UTargetingComponent_Discover()
{
	bRequiresInput = true;
}

bool UTargetingComponent_Discover::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	if (bPreview)
		return false;

	if (!CardEffect.TargetComponent->IsChildOf(UPileComponent::StaticClass()))
	{
		return false;
	}
	


	TArray<FCard> CardOptions;

	CurrentCardOption.Reset();

	if (IsValid(CardEffect.UsedData.DataTable))
	{
		// CardEffect가 요구하는 조건이 있다면(Flag_Specific)
		if (CardEffect.GameplayTags.HasTagExact(CosGameTags::Flag_Specific))
		{
			// 찾아서 추가
			CardOptions.Add(*CardEffect.UsedData.DataTable->FindRow<FCard>(CardEffect.UsedData.RowName,TEXT("Fcard in TargetingComponent Discover")));
		}
		else
		{
			CardOptions = SetCardOptionsWithRarity(CardEffect);
		}
	}
	else
	{
		CardOptions = SetCardOptions(CardEffect);
	}

	for (FCard CardOption : CardOptions)
	{
		ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(),
		                                                               FTransform::Identity, Card->GetOwner(), nullptr,
		                                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn, ESpawnActorScaleMethod::OverrideRootScale);

		NewCard->CardDataDeck = CardOption;

		NewCard->FinishSpawning(FTransform::Identity);

		CurrentCardOption.Add(NewCard);
	}

	
	
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_CardRewardScreen>([&](AAction_CardRewardScreen* Action_CardRewardScreen)
	{
		Action_CardRewardScreen->CardOptions = CurrentCardOption;

		Action_CardRewardScreen->ScreenText = CardEffect.GameplayTags.HasTagExact(CosGameTags::Flag_Specific)? FText::FromString(TEXT("Card Granted")) : FText::FromString(TEXT("Choose a Card"));

		Action_CardRewardScreen->bAllowSkip = CardEffect.GameplayTags.HasTagExact(CosGameTags::Flag_Skippable);
	});
	
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	BindToCardConfirm(CardPlayer->PlayerUI->WBP_CardRewardScreen);

	return true;
}

void UTargetingComponent_Discover::BindToCardConfirm(UUW_CardRewardScreen* InCardRewardScreen)
{
	CardRewardScreen = InCardRewardScreen;

	InCardRewardScreen->OnReturnSelectedCardInRewardScreen.AddDynamic(this, &UTargetingComponent_Discover::ValidateAndTransferSelectedCard);
}

void UTargetingComponent_Discover::ValidateAndTransferSelectedCard(bool bSkipped, ACardBase* Card)
{
	CardRewardScreen->OnReturnSelectedCardInRewardScreen.RemoveDynamic(this, &UTargetingComponent_Discover::ValidateAndTransferSelectedCard);

	for (ACardBase* CardOption : CurrentCardOption)
	{
		if (Card != CardOption && IsValid(CardOption))
		{
			CardOption->Destroy();
		}
	}

	if (IsValid(Card))
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

		CardPlayer->PlayerUI->WBP_CardHand->bSelectionMode = false;

		TArray<AActor*> InputTarget;

		InputTarget.Add(Card);

		OnInputTargetsReceived.Broadcast(InputTarget);
	}
	else
	{
		OnInputTargetsReceived.Broadcast(EmptyArray);
	}
}

TArray<FCard> UTargetingComponent_Discover::SetCardOptions(const FCardEffect& CardEffect)
{
	// 필터가능한지 확인
	if (CardEffect.GameplayTags.HasTag(CosGameTags::Flag_FilterToAllowed))
	{
		// 필터 가능하면 필터링 된 카드들 랜덤으로 가져옴
		FGameplayTagContainer Tags = IInterface_CardGameInstance::Execute_GetAllowedCardRewardsFromInstance(UFunctionLibrary_Singletons::GetCardGameInstance(this));

		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

		FGameplayTagContainer RequiredTags;
		RequiredTags.AddTag(CosGameTags::Rarity);

		return CardPlayer->ChanceManagerComponent->GetRandomFilteredCards(3, Tags, RequiredTags);
	}
	else
	{
		// 필터 불가능하면 그냥 랜덤으로 카드들 가져옴
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

		return CardPlayer->ChanceManagerComponent->GetTrueRandomCardsOfAnyRarity(3);
	}
}

TArray<FCard> UTargetingComponent_Discover::SetCardOptionsWithRarity(const FCardEffect& CardEffect)
{
	// 받아온 데이터에서 RarityWeights 꺼냄
	FRarityWeights* RarityWeightsFound = CardEffect.UsedData.DataTable->FindRow<FRarityWeights>(CardEffect.UsedData.RowName,TEXT("FRarityWeights in TargetingComponent Discover"));

	if (RarityWeightsFound)
	{
		TMap<FGameplayTag, float> RarityWeightsMap = RarityWeightsFound->RarityWeights;

		FGameplayTagContainer AllowedCardRewards;

		// 필터링 허용되면 GetRandomCardsByTagWeights의 결과에 영향을 주는 PossibleTags를 추가한다.
		// 확률에 따라 포함될 수 있는 태그를 포함한 카드가 나오게 된다.
		if (CardEffect.GameplayTags.HasTag(CosGameTags::Flag_FilterToAllowed))
		{
			AllowedCardRewards = IInterface_CardGameInstance::Execute_GetAllowedCardRewardsFromInstance(UFunctionLibrary_Singletons::GetCardGameInstance(this));
		}

		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

		return CardPlayer->ChanceManagerComponent->GetRandomCardsByTagWeights(RarityWeightsMap, AllowedCardRewards, FGameplayTagContainer(), true, 3);
	}
	else
	{
		return SetCardOptions(CardEffect);
	}
}
