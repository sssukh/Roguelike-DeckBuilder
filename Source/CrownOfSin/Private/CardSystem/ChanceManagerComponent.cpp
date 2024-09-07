
#include "CardSystem/ChanceManagerComponent.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"


// Sets default values for this component's properties
UChanceManagerComponent::UChanceManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
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


TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsWithoutWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount)
{
	TArray<FGameplayTag> RandomTags;
	TArray<FGameplayTag> Keys;
	WeightedTags.GetKeys(Keys);

	// ShuffleArray를 이용해 태그 배열 섞기
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