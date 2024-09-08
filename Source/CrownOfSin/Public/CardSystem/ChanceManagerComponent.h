#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "ChanceManagerComponent.generated.h"

/*태그 필터 및 확률 가중치를 기반으로 카드 보상, 아티팩트 및 스토리 이벤트 반환을 담당합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UChanceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChanceManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * 가중치에 따라 태그를 랜덤으로 선택하여 반환합니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param InAmount 선택할 태그의 개수
	 * @return 가중치 기반으로 선택된 태그 배열
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FGameplayTag> GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags, int InAmount);

	/**
	 * 가중치 총합을 기반으로 랜덤한 태그를 선택합니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param TotalWeight 가중치들의 총합
	 * @param OutPickedTag 선택된 태그 (출력 파라미터)
	 * @return 태그를 성공적으로 선택했는지 여부 (true/false)
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool PickTagFromWeightAndWeightSum(TMap<FGameplayTag, float> WeightedTags, float TotalWeight, FGameplayTag& OutPickedTag);


	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool FindPickedTagAmongFilteredCards(FGameplayTag PickedCardTag, TArray<FCard>& FilteredCards, bool bPreventDuplicates, FCard& OutPickedCard);


	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetFilteredCardsWithAllTags(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch);

	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetFilteredCardsWithAnyTag(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch);

	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetRandomCardsByTagWeights(TMap<FGameplayTag, float> TagWeights, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch, int32 InAmount);

	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetRandomFilteredCards(int32 InAmount, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch = false);

	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetTrueRandomCardsOfAnyRarity(int32 InAmount);

protected:
	/**
	 * 가중치가 모두 동일한 경우 랜덤하게 태그를 선택합니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param InAmount 선택할 태그의 개수
	 * @return 랜덤으로 선택된 태그 배열
	 */
	TArray<FGameplayTag> GetRandomTagsWithoutWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount);

	/**
	 * 가중치 배열의 총합을 계산하여 반환합니다.
	 * @param WeightValues 가중치 값들의 배열
	 * @return 가중치들의 총합
	 */
	float GetTotalWeight(const TArray<float>& WeightValues);

	/**
	 * 가중치에 따라 태그를 랜덤으로 선택하는 함수입니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param InAmount 선택할 태그의 개수
	 * @param TotalWeight 가중치들의 총합
	 * @return 가중치 기반으로 선택된 태그 배열
	 */
	TArray<FGameplayTag> GetRandomTagsWithWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount, float TotalWeight);

	// 필수 태그로 카드 필터링
	void FilterCardsByRequiredTags(TArray<FCard>& Cards, const FGameplayTagContainer& RequiredTags, bool bExactMatch);

	// 가능한 태그로 카드 필터링
	void FilterCardsByPossibleTags(TArray<FCard>& Cards, const FGameplayTagContainer& PossibleTags, bool bExactMatch);

	// 태그 가중치가 있는 경우 처리
	TArray<FCard> ProcessCardsWithWeights(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights, int32 InAmount);

	// 필터링된 카드에 태그 가중치 적용
	void ApplyTagWeightsToFilteredCards(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights);
	
	TArray<FCard> PickCardsByTags(TArray<FCard>& FilteredCards, const TArray<FGameplayTag>& SelectedTags);

	// 선택된 카드 수가 부족할 경우 중복 카드 추가
	TArray<FCard> AddDuplicateCardsToFillAmount(const TArray<FCard>& SelectedCards, int32 InAmount);
	
	// 무작위로 카드를 선택하는 함수 (가중치 없이)
	TArray<FCard> PickRandomCardsWithoutWeights(TArray<FCard>& FilteredCards, int32 InAmount);

	// 필터링된 카드가 없을 경우 보상 카드 처리
	TArray<FCard> HandleNoFilteredCards(int32 InAmount);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chance Manager|Config|Setup")
	FDataTableRowHandle CardRewardDataRowHandle;
	
	UPROPERTY(BlueprintReadWrite, Category="Chance Manager")
	TArray<FCard> AllCards;
};
