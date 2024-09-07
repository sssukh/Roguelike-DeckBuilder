// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ChanceManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UChanceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChanceManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	 * 가중치에 따라 태그를 랜덤으로 선택하여 반환합니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param InAmount 선택할 태그의 개수
	 * @return 가중치 기반으로 선택된 태그 배열
	 */
	TArray<FGameplayTag> GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags, int InAmount);

	/**
	 * 가중치 총합을 기반으로 랜덤한 태그를 선택합니다.
	 * @param WeightedTags 태그와 가중치로 이루어진 맵
	 * @param TotalWeight 가중치들의 총합
	 * @param OutPickedTag 선택된 태그 (출력 파라미터)
	 * @return 태그를 성공적으로 선택했는지 여부 (true/false)
	 */
	bool PickTagFromWeightAndWeightSum(TMap<FGameplayTag, float> WeightedTags, float TotalWeight, FGameplayTag& OutPickedTag);

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
};
