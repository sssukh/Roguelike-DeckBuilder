#include "CardSystem/ChanceManagerComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "Libraries/AssetTableRef.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UChanceManagerComponent::UChanceManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...

	if (UDataTable* DT_Cards_Rewards = FAssetReferenceUtility::LoadAssetFromDataTable<UDataTable>(AssetRefPath::DataTablePath, FName(TEXT("DT_Cards_Rewards"))))
	{
		CardRewardDataRowHandle.DataTable = DT_Cards_Rewards;
		CardRewardDataRowHandle.RowName = FName(TEXT("ConsolationPrize"));
	}
}

void UChanceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags, int InAmount)
{
	// 가중치 값을 저장할 배열 생성
	TArray<float> WeightValues;
	WeightedTags.GenerateValueArray(WeightValues);

	// 최소 및 최대 가중치 계산
	float MinWeight = FMath::Min(WeightValues);
	float MaxWeight = FMath::Max(WeightValues);

	TArray<FGameplayTag> PickedTags;

	// 모든 가중치가 동일한 경우 랜덤으로 태그를 선택
	if (FMath::IsNearlyEqual(MinWeight, MaxWeight))
	{
		PickedTags = GetRandomTagsWithoutWeight(WeightedTags, InAmount);
	}
	else
	{
		// 가중치 합을 계산하고, 가중치에 따라 태그 선택
		float TotalWeight = GetTotalWeight(WeightValues);
		PickedTags = GetRandomTagsWithWeight(WeightedTags, InAmount, TotalWeight);
	}

	return PickedTags;
}

bool UChanceManagerComponent::PickTagFromWeightAndWeightSum(TMap<FGameplayTag, float> WeightedTags, float TotalWeight, FGameplayTag& OutPickedTag)
{
	// 기준값: 0과 가중치 총합 사이에서 랜덤 값 선택
	float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);

	// 누적 가중치
	float AccumulatedWeight = 0.0f;

	// 가중치 맵을 순회하여 태그 선택
	for (const TPair<FGameplayTag, float>& TagPair : WeightedTags)
	{
		AccumulatedWeight += TagPair.Value;

		// 누적된 가중치가 랜덤 값에 도달하면 해당 태그를 선택
		if (AccumulatedWeight >= RandomWeight)
		{
			OutPickedTag = TagPair.Key;
			return true;
		}
	}

	// 만약 태그를 선택하지 못한 경우 (논리적으로는 실행되지 않음)
	return false;
}

bool UChanceManagerComponent::FindPickedTagAmongFilteredCards(FGameplayTag PickedCardTag, TArray<FCard>& FilteredCards, bool bPreventDuplicates, FCard& OutPickedCard)
{
	for (int i = FilteredCards.Num() - 1; i >= 0; --i)
	{
		if (!UBlueprintGameplayTagLibrary::HasTag(FilteredCards[i].CardTags, PickedCardTag, true))
		{
			continue;
		}

		if (bPreventDuplicates)
		{
			FilteredCards.RemoveAt(i);
		}

		OutPickedCard = FilteredCards[i];

		return true;
	}

	return false;
}

TArray<FCard> UChanceManagerComponent::GetFilteredCardsWithAllTags(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch)
{
	TArray<FCard> FilteredCards;

	for (const FCard& Card : Cards)
	{
		if (UBlueprintGameplayTagLibrary::HasAllTags(Card.CardTags, Filter, bExactMatch))
		{
			FilteredCards.Add(Card);
		}
	}
	return FilteredCards;
}

TArray<FCard> UChanceManagerComponent::GetFilteredCardsWithAnyTag(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch)
{
	TArray<FCard> FilteredCards;

	for (const FCard& Card : Cards)
	{
		if (UBlueprintGameplayTagLibrary::HasAnyTags(Card.CardTags, Filter, bExactMatch))
		{
			FilteredCards.Add(Card);
		}
	}
	return FilteredCards;
}

TArray<FCard> UChanceManagerComponent::GetRandomCardsByTagWeights(TMap<FGameplayTag, float> TagWeights, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags,
                                                                  bool bExactMatch, int32 InAmount)
{
	TArray<FCard> SelectedCards;
	TArray<FCard> FilteredCards = AllCards;

	// 필수 태그로 카드 필터링
	FilterCardsByRequiredTags(FilteredCards, RequiredTags, bExactMatch);

	// 가능한 태그로 카드 필터링
	FilterCardsByPossibleTags(FilteredCards, PossibleTags, bExactMatch);

	// 필터링된 카드가 있는 경우
	if (FilteredCards.Num() > 0)
	{
		// 태그 가중치가 있을 경우
		if (TagWeights.Num() > 0)
		{
			return ProcessCardsWithWeights(FilteredCards, TagWeights, InAmount);
		}
		else
		{
			// 태그 가중치가 없을 경우, 무작위로 카드를 선택
			return PickRandomCardsWithoutWeights(FilteredCards, InAmount);
		}
	}
	else
	{
		// 필터링된 카드가 없을 경우 보상 카드 처리
		return HandleNoFilteredCards(InAmount);
	}
}

TArray<FCard> UChanceManagerComponent::GetRandomFilteredCards(int32 InAmount, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch)
{
	TMap<FGameplayTag, float> EmptyWeights;
	return GetRandomCardsByTagWeights(EmptyWeights, PossibleTags, RequiredTags, bExactMatch, InAmount);
}

TArray<FCard> UChanceManagerComponent::GetTrueRandomCardsOfAnyRarity(int32 InAmount)
{
	TArray<FCard> PickedCards;

	FGameplayTag RequiredTag = CosGameTags::Rarity;
	FGameplayTagContainer RequiredTags = RequiredTag.GetSingleTagContainer();
	TArray<FCard> RandomCards = GetRandomFilteredCards(InAmount, FGameplayTagContainer(), RequiredTags, false);

	if (InAmount > 1)
	{
		for (int i = 0; i < InAmount; ++i)
		{
			PickedCards.Add(RandomCards[i]);
		}

		return PickedCards;
	}


	PickedCards.Add(RandomCards[0]);
	return PickedCards;
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsWithoutWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount)
{
	TArray<FGameplayTag> RandomTags;
	TArray<FGameplayTag> Keys;

	WeightedTags.GetKeys(Keys);
	UFunctionLibrary_ArrayUtils::ShuffleArray(Keys);

	// 태그를 임의로 선택하여 결과에 추가
	while (RandomTags.Num() < InAmount)
	{
		RandomTags.Add(Keys[FMath::RandRange(0, Keys.Num() - 1)]);
	}

	return RandomTags;
}

float UChanceManagerComponent::GetTotalWeight(const TArray<float>& WeightValues)
{
	float TotalWeight = 0.0f;
	for (float Weight : WeightValues)
	{
		TotalWeight += Weight;
	}
	return TotalWeight;
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsWithWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount, float TotalWeight)
{
	TArray<FGameplayTag> WeightedTagsResult;

	while (WeightedTagsResult.Num() < InAmount)
	{
		FGameplayTag PickedTag;
		if (PickTagFromWeightAndWeightSum(WeightedTags, TotalWeight, PickedTag))
		{
			WeightedTagsResult.Add(PickedTag);
		}
	}

	return WeightedTagsResult;
}

void UChanceManagerComponent::FilterCardsByRequiredTags(TArray<FCard>& Cards, const FGameplayTagContainer& RequiredTags, bool bExactMatch)
{
	if (RequiredTags.Num() > 0)
	{
		Cards = GetFilteredCardsWithAllTags(RequiredTags, Cards, bExactMatch);
	}
}

void UChanceManagerComponent::FilterCardsByPossibleTags(TArray<FCard>& Cards, const FGameplayTagContainer& PossibleTags, bool bExactMatch)
{
	if (PossibleTags.Num() > 0)
	{
		Cards = GetFilteredCardsWithAnyTag(PossibleTags, Cards, bExactMatch);
	}
}

// 태그 가중치가 있는 경우 처리
TArray<FCard> UChanceManagerComponent::ProcessCardsWithWeights(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights, int32 InAmount)
{
	// 필터링된 카드에 태그 가중치 적용
	ApplyTagWeightsToFilteredCards(FilteredCards, TagWeights);

	// 태그 가중치 기반으로 무작위 태그 선택
	TArray<FGameplayTag> SelectedTags = GetRandomTagsByWeights(TagWeights, InAmount);

	// 선택된 태그에 해당하는 카드 선택
	TArray<FCard> SelectedCards = PickCardsByTags(FilteredCards, SelectedTags);

	// 선택된 카드 수가 부족할 경우 중복 카드를 추가
	if (SelectedCards.Num() > 0)
	{
		return AddDuplicateCardsToFillAmount(SelectedCards, InAmount);
	}
	else
	{
		// 카드가 없으면 보상 카드 처리
		return HandleNoFilteredCards(InAmount);
	}
}

void UChanceManagerComponent::ApplyTagWeightsToFilteredCards(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights)
{
	TArray<FGameplayTag> TagWeightKeys;
	TagWeights.GetKeys(TagWeightKeys);

	// 가중치가 있는 태그로 추가 필터링
	FGameplayTagContainer TagWeightKeyContainer = FGameplayTagContainer::CreateFromArray(TagWeightKeys);
	FilteredCards = GetFilteredCardsWithAnyTag(TagWeightKeyContainer, FilteredCards, true);

	// 필터링된 카드를 섞어줌
	UFunctionLibrary_ArrayUtils::ShuffleArray(FilteredCards);
}

TArray<FCard> UChanceManagerComponent::PickCardsByTags(TArray<FCard>& FilteredCards, const TArray<FGameplayTag>& SelectedTags)
{
	TArray<FCard> PickedCards;

	// 각 선택된 태그에 대해 해당하는 카드 선택
	for (const FGameplayTag& Tag : SelectedTags)
	{
		FCard PickedCard;
		if (FindPickedTagAmongFilteredCards(Tag, FilteredCards, true, PickedCard))
		{
			PickedCards.Add(PickedCard);
		}
	}

	return PickedCards;
}

TArray<FCard> UChanceManagerComponent::AddDuplicateCardsToFillAmount(const TArray<FCard>& SelectedCards, int32 InAmount)
{
	TArray<FCard> CardsWithDuplicates = SelectedCards;

	while (CardsWithDuplicates.Num() < InAmount)
	{
		int32 RandomIndex;
		FCard RandomCard = UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(SelectedCards, RandomIndex);
		CardsWithDuplicates.Add(RandomCard);
	}

	return CardsWithDuplicates;
}

TArray<FCard> UChanceManagerComponent::PickRandomCardsWithoutWeights(TArray<FCard>& FilteredCards, int32 InAmount)
{
	UFunctionLibrary_ArrayUtils::ShuffleArray(FilteredCards);
	TArray<FCard> RandomCards;

	// InAmount만큼 무작위로 카드 선택
	for (int32 i = 0; i < InAmount; ++i)
	{
		int32 Index = i % FilteredCards.Num();
		RandomCards.Add(FilteredCards[Index]);
	}

	return RandomCards;
}

TArray<FCard> UChanceManagerComponent::HandleNoFilteredCards(int32 InAmount)
{
	TArray<FCard> FallbackCards;

	if (!CardRewardDataRowHandle.IsNull())
	{
		if (FCard* FoundCard = CardRewardDataRowHandle.DataTable->FindRow<FCard>(CardRewardDataRowHandle.RowName, TEXT("")))
		{
			FCard RewardsCard = *FoundCard;
			RewardsCard.DataRow = CardRewardDataRowHandle;

			// 보상 카드로 InAmount만큼 채우기
			while (FallbackCards.Num() < InAmount)
			{
				FallbackCards.Add(RewardsCard);
			}

			return FallbackCards;
		}
		else
		{
			COS_LOG_SCREEN(TEXT("오류: 유효한 카드 옵션이 충분하지 않으며 유효한 백업 옵션이 없습니다."));
		}
	}
	else
	{
		COS_LOG_SCREEN(TEXT("DataTableRowHandle_CardsRewards을 반드시 설정해주세요!"));
	}

	return FallbackCards;
}


// TArray<FCard> UChanceManagerComponent::GetRandomCardsByTagWeights(TMap<FGameplayTag, float> TagWeights, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch,
//                                                                   int32 InAmount)
// {
// 	TArray<FCard> LocalPickedCards;
// 	TArray<FCard> LocalFilteredCards = AllCards;
//
// 	{
// 		TArray<FGameplayTag> RequiredTagsArray;
// 		RequiredTags.GetGameplayTagArray(RequiredTagsArray);
//
// 		if (RequiredTagsArray.Num() > 0)
// 		{
// 			LocalFilteredCards = GetFilteredCardsWithAllTags(RequiredTags, LocalFilteredCards, bExactMatch);
// 		}
// 	}
//
// 	{
// 		TArray<FGameplayTag> PossibleTagsArray;
// 		PossibleTags.GetGameplayTagArray(PossibleTagsArray);
//
// 		if (PossibleTags.Num() > 0)
// 		{
// 			LocalFilteredCards = GetFilteredCardsWithAnyTag(PossibleTags, LocalFilteredCards, bExactMatch);
// 		}
// 	}
//
//
// 	if (LocalFilteredCards.Num() > 0)
// 	{
// 		if (TagWeights.Num() > 0)
// 		{
// 			TArray<FGameplayTag> TagWeightsKeyArray;
// 			TagWeights.GetKeys(TagWeightsKeyArray);
// 			FGameplayTagContainer TagWeightKeyContainer = FGameplayTagContainer::CreateFromArray(TagWeightsKeyArray);
// 			LocalFilteredCards = GetFilteredCardsWithAnyTag(TagWeightKeyContainer, LocalFilteredCards, bExactMatch);
// 			UFunctionLibrary_ArrayUtils::ShuffleArray(LocalFilteredCards);
//
// 			TArray<FGameplayTag> PickedTags = GetRandomTagsByWeights(TagWeights, InAmount);
//
// 			for (const FGameplayTag& PickedTag : PickedTags)
// 			{
// 				FCard PickedCard;
// 				if (FindPickedTagAmongFilteredCards(PickedTag, LocalFilteredCards, true, PickedCard))
// 				{
// 					LocalPickedCards.Add(PickedCard);
// 				}
// 			}
//
//
// 			if (LocalPickedCards.Num() > 0)
// 			{
// 				TArray<FCard> LocalPickedCardsWithDuplicates = LocalPickedCards;
//
// 				while (LocalPickedCardsWithDuplicates.Num() < InAmount)
// 				{
// 					int32 RandomIndex;
// 					FCard RandomCard = UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(LocalPickedCards, RandomIndex);
// 					LocalPickedCardsWithDuplicates.Add(RandomCard);
// 				}
//
// 				return LocalPickedCardsWithDuplicates;
// 			}
// 			else
// 			{
// 				//구현해야함 로직이 똑같음
// 				if (!DataTableRowHandle_CardsRewards.IsNull())
// 				{
// 					if (FCard* FoundCard = DataTableRowHandle_CardsRewards.DataTable->FindRow<FCard>(DataTableRowHandle_CardsRewards.RowName,TEXT("")))
// 					{
// 						FCard RewardsCard = *FoundCard;
// 						RewardsCard.DataRow = DataTableRowHandle_CardsRewards;
//
// 						while (LocalPickedCards.Num() < InAmount)
// 						{
// 							LocalPickedCards.Add(RewardsCard);
// 						}
//
// 						return LocalPickedCards;
// 					}
// 					else
// 					{
// 						COS_LOG_SCREEN(TEXT("오류: 유효한 카드 옵션이 충분하지 않으며 유효한 백업 옵션이 없습니다."));
// 					}
//
// 					return LocalPickedCards;
// 				}
// 				else
// 				{
// 					COS_LOG_SCREEN(TEXT("DataTableRowHandle_CardsRewards을 반드시 설정해주세요!"));
// 					return LocalPickedCards;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			UFunctionLibrary_ArrayUtils::ShuffleArray(LocalFilteredCards);
// 			for (int32 i = 1; i < InAmount; ++i)
// 			{
// 				int32 Index = i % LocalFilteredCards.Num();
// 				LocalPickedCards.Add(LocalFilteredCards[Index]);
// 			}
// 			return LocalPickedCards;
// 		}
// 	}
// 	else
// 	{
// 		//구현해야함 로직이 똑같음
// 		if (!DataTableRowHandle_CardsRewards.IsNull())
// 		{
// 			if (FCard* FoundCard = DataTableRowHandle_CardsRewards.DataTable->FindRow<FCard>(DataTableRowHandle_CardsRewards.RowName,TEXT("")))
// 			{
// 				FCard RewardsCard = *FoundCard;
// 				RewardsCard.DataRow = DataTableRowHandle_CardsRewards;
//
// 				while (LocalPickedCards.Num() < InAmount)
// 				{
// 					LocalPickedCards.Add(RewardsCard);
// 				}
//
// 				return LocalPickedCards;
// 			}
// 			else
// 			{
// 				COS_LOG_SCREEN(TEXT("오류: 유효한 카드 옵션이 충분하지 않으며 유효한 백업 옵션이 없습니다."));
// 			}
//
// 			return LocalPickedCards;
// 		}
// 		else
// 		{
// 			COS_LOG_SCREEN(TEXT("DataTableRowHandle_CardsRewards을 반드시 설정해주세요!"));
// 			return LocalPickedCards;
// 		}
// 	}
// }
