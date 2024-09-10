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

	/**
 	 * @brief 선택된 태그에 해당하는 아티팩트를 필터링된 아티팩트 리스트에서 찾습니다.
 	 * 
 	 * 주어진 태그와 일치하는 아티팩트를 필터링된 리스트에서 검색하며, 중복을 방지할 수 있습니다.
 	 * @param PickedTag 선택된 태그
 	 * @param FilteredArtifacts 필터링된 아티팩트 리스트
 	 * @param bPreventDuplicates 중복된 항목을 제거할지 여부
 	 * @param OutPickedArtifact 찾은 아티팩트를 저장할 변수
 	 * @return 아티팩트를 찾았는지 여부
 	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool FindPickedTagAmongFilteredArtifacts(FGameplayTag PickedTag, TArray<FStatusData>& FilteredArtifacts, bool bPreventDuplicates, FStatusData& OutPickedArtifact);

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

	/**
 	 * @brief 게임 인스턴스에서 이미 획득한 아티팩트를 제외한 나머지 아티팩트를 반환합니다.
 	 * 
 	 * 이 함수는 모든 아티팩트 리스트에서 이미 게임 인스턴스에 등록된 아티팩트를 제외한 나머지 아티팩트를 반환합니다. 
 	 * 게임 진행 중에 이미 획득한 아티팩트를 추적하고, 남은 아티팩트들만 다시 제공하기 위한 용도로 사용됩니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FStatusData> GetRemainingArtifacts();

	/**
 	 * @brief 가중치가 부여된 태그에 따라 무작위로 아티팩트를 선택하여 반환합니다.
 	 * 
 	 * 이 함수는 주어진 가중치 맵을 기반으로 태그를 무작위로 선택한 후, 해당 태그와 일치하는 아티팩트를 반환합니다. 
 	 * 선택된 아티팩트의 개수가 부족할 경우 기본 아티팩트를 추가하여 필요한 개수를 충족시킵니다.
 	 * @param TagWeights 가중치가 부여된 태그 맵
 	 * @param InAmount 선택할 아티팩트의 수
 	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
 	 * @param OutPickedArtifacts 선택된 아티팩트를 담을 배열
 	 * @return 선택 작업이 성공했는지 여부
 	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool GetRandomArtifactsByCustomTagWeights(TMap<FGameplayTag, float> TagWeights, int32 InAmount, bool bExactMatch, TArray<FStatusData>& OutPickedArtifacts);

	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool GetRandomArtifactsByDefaultRarityWeights(int32 InAmount, TArray<FStatusData>& OutPickedArtifacts);

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
	UPROPERTY(BlueprintReadWrite, Category="Chance Manager")
	TArray<FCard> AllCards;

	UPROPERTY(BlueprintReadWrite, Category="Chance Manager")
	TArray<FStatusData> AllArtifacts;

	UPROPERTY(BlueprintReadWrite, Category="Chance Manager")
	TMap<FGameplayTag, float> DefaultArtifactRarityWeights;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chance Manager|Config|Setup")
	FDataTableRowHandle CardRewardDataRowHandle;
};
