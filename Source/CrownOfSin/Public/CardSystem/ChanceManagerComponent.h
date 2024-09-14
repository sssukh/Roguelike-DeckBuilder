#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "ChanceManagerComponent.generated.h"

/*태그 필터 및 확률 가중치를 기반으로 카드 보상, 아티팩트 및 스토리 이벤트 반환을 담당합니다.*/
UCLASS(Blueprintable, BlueprintType, ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UChanceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChanceManagerComponent();

protected:
	virtual void BeginPlay() override;

	TArray<FCard> GenerateAllCardsArray();

	TArray<FStatusData> GenerateAllArtifactsArray();

public:
	/**
	 * @brief 가중치에 따라 무작위로 태그를 선택하여 반환합니다.
	 * @param WeightedTags 태그별 가중치 맵
	 * @param InAmount 선택할 태그 수
	 * @return 가중치 기반으로 선택된 태그 배열
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FGameplayTag> GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags, int InAmount);

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

	/**
	 * @brief 모든 태그가 포함된 카드를 필터링하여 반환합니다.
	 * @param Filter 필터링에 사용할 태그 컨테이너
	 * @param Cards 대상 카드 배열
	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
	 * @return 필터링된 카드 배열
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetFilteredCardsWithAllTags(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch);

	/**
	 * @brief 카드 배열에서 지정된 태그들 중 하나라도 일치하는 카드를 필터링합니다.
	 * @param Filter 필터링에 사용할 태그 컨테이너
	 * @param Cards 대상 카드 배열
	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
	 * @return 필터링된 카드 배열
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetFilteredCardsWithAnyTag(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch);

	/**
	 * @brief 가능한 태그와 필수 태그를 기반으로, 가중치를 적용하여 무작위로 카드를 선택합니다.
	 * @param TagWeights 태그별 가중치 맵
	 * @param PossibleTags 가능한 태그들 (필터링 기준)
	 * @param RequiredTags 필수 태그들 (필터링 기준)
	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
	 * @param InAmount 선택할 카드 수
	 * @return 무작위로 선택된 카드 배열
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetRandomCardsByTagWeights(TMap<FGameplayTag, float> TagWeights, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch, int32 InAmount);

	/**
	 * @brief 가능한 태그와 필수 태그를 기반으로, 요청된 수량만큼 필터링된 카드를 무작위로 반환합니다.
	 * @param InAmount 선택할 카드 수
	 * @param PossibleTags 가능한 태그들 (필터링 기준)
	 * @param RequiredTags 필수 태그들 (필터링 기준)
	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
	 * @return 필터링된 카드 배열
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FCard> GetRandomFilteredCards(int32 InAmount, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch = false);

	/**
	 * @brief 주어진 수량만큼 무작위 희귀도의 카드를 선택하여 반환합니다. 
	 *        선택할 수량이 0 이하일 경우, 기본 카드 한 장을 반환합니다.
	 * @param InAmount 선택할 카드 수
	 * @return 무작위로 선택된 카드 배열
	 */
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
 	 * @brief 필터링된 카드 목록에서 선택된 태그와 일치하는 카드를 찾고, 중복을 방지할 수 있습니다.
 	 * @param PickedCardTag 선택된 카드 태그
 	 * @param FilteredCards 필터링된 카드 배열
 	 * @param bPreventDuplicates 중복 방지 여부
 	 * @param OutPickedCard 찾은 카드를 출력으로 전달
 	 * @return 해당 태그를 가진 카드를 찾았는지 여부 (true/false)
 	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool FindPickedTagAmongFilteredCards(FGameplayTag PickedCardTag, TArray<FCard>& FilteredCards, bool bPreventDuplicates, FCard& OutPickedCard);

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
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	TArray<FGameplayTag> GetRandomTagsWithWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount, float TotalWeight);

	/**
	 * 가중치 총합을 기반으로 랜덤한 태그를 선택합니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param TotalWeight 가중치들의 총합
	 * @param OutPickedTag 선택된 태그 (출력 파라미터)
	 * @return 태그를 성공적으로 선택했는지 여부 (true/false)
	 */
	UFUNCTION(BlueprintCallable, Category="Chance Manager Event")
	bool PickTagFromWeightAndWeightSum(TMap<FGameplayTag, float> WeightedTags, float TotalWeight, FGameplayTag& OutPickedTag);

	/**
 	 * @brief 필수 태그를 기준으로 카드 목록을 필터링합니다.
 	 * @param RequiredTags 필수 태그들
 	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
 	 * @param OutCards 필터링할 카드 배열 (참조로 전달됨)
 	 */
	void FilterCardsByRequiredTags(const FGameplayTagContainer& RequiredTags, bool bExactMatch, TArray<FCard>& OutCards);

	/**
 	 * @brief 가능한 태그를 기준으로 카드 목록을 필터링합니다.
 	 * @param PossibleTags 가능한 태그들
 	 * @param bExactMatch 태그 일치 여부에 대한 엄격한 검사 여부
 	 * @param OutCards 필터링할 카드 배열 (참조로 전달됨)
 	 */
	void FilterCardsByPossibleTags(const FGameplayTagContainer& PossibleTags, bool bExactMatch, TArray<FCard>& OutCards);

	/**
	 * @brief 필터링된 카드에 태그 가중치를 적용하여 카드를 무작위로 선택하고, 부족한 수량을 채우기 위해 중복 카드를 추가합니다.
	 * @param FilteredCards 필터링된 카드 배열
	 * @param TagWeights 태그별 가중치 맵
	 * @param InAmount 선택할 카드 수
	 * @return 선택된 카드 배열 (부족한 경우 중복 카드 추가)
	 */
	TArray<FCard> ProcessCardsWithWeights(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights, int32 InAmount);

	/**
	 * @brief 태그 가중치에 해당하는 카드만 필터링한 후, 무작위로 섞습니다.
	 * @param FilteredCards 필터링된 카드 배열 (참조로 전달됨)
	 * @param TagWeights 태그별 가중치 맵
	 */
	void ApplyTagWeightsToFilteredCards(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights);

	/**
	 * @brief 선택된 태그에 해당하는 카드를 필터링된 카드 목록에서 선택하여 반환합니다.
	 * @param FilteredCards 필터링된 카드 배열
	 * @param SelectedTags 선택된 태그 배열
	 * @return 선택된 태그와 일치하는 카드 배열
	 */
	TArray<FCard> PickCardsByTags(TArray<FCard>& FilteredCards, const TArray<FGameplayTag>& SelectedTags);

	/**
	 * @brief 선택된 카드 목록에 중복 카드를 추가하여 요청된 수량만큼 채웁니다.
	 * @param SelectedCards 선택된 카드 배열
	 * @param InAmount 필요한 카드 수량
	 * @return 중복이 추가된 카드 배열
	 */
	TArray<FCard> AddDuplicateCardsToFillAmount(const TArray<FCard>& SelectedCards, int32 InAmount);

	/**
	 * @brief 필터링된 카드 중에서 가중치 없이 무작위로 카드를 선택합니다.
	 * @param FilteredCards 필터링된 카드 배열
	 * @param InAmount 선택할 카드 수
	 * @return 무작위로 선택된 카드 배열
	 */
	TArray<FCard> PickRandomCardsWithoutWeights(TArray<FCard>& FilteredCards, int32 InAmount);

	/**
	 * @brief 필터링된 카드가 없을 때 보상 카드로 채우는 함수입니다.
	 * @param InAmount 필요한 카드 수
	 * @return 보상 카드 배열
	 */
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chance Manager", meta=(ExposeOnSpawn="true", RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.StatusData"))
	TArray<UDataTable*> ArtifactTables;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chance Manager|Config|Setup")
	FDataTableRowHandle CardRewardDataRowHandle;
};
