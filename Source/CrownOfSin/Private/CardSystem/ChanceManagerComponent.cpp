// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/ChanceManagerComponent.h"

#include "Libraries/FunctionLibrary_ArrayUtils.h"


// Sets default values for this component's properties
UChanceManagerComponent::UChanceManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChanceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChanceManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags,
	int InAmount)
{
	TArray<float> MapValues;
	
	WeightedTags.GenerateValueArray(MapValues);
	int32 tmp;
	int32 minValue = FMath::Min(MapValues, &tmp);
	int32 maxValue = FMath::Max(MapValues,&tmp);

	TArray<FGameplayTag> LocalPickedTags;
	
	// 모든 확률이 같다면 그냥 랜덤뽑기
	if(maxValue==minValue)
	{
		while(LocalPickedTags.Num()<InAmount) 
		{
			TArray<FGameplayTag> Keys;
			WeightedTags.GetKeys(Keys);

			UFunctionLibrary_ArrayUtils::ShuffleArray(Keys);
			
			LocalPickedTags.Add(Keys[0]);
		}
		return LocalPickedTags;
	}
	else
	{
		float LocalWeightSum=0;
		for (float MapValue :MapValues)
		{
			LocalWeightSum+=MapValue;
		}

		while(LocalPickedTags.Num()<InAmount)
		{
			FGameplayTag PickedTag;
			if(PickTagFromWeightAndWeightSum(WeightedTags,LocalWeightSum,PickedTag))
			{
				LocalPickedTags.Add(PickedTag);
			}

			
		}
		return LocalPickedTags;
	}
}

bool UChanceManagerComponent::PickTagFromWeightAndWeightSum(TMap<FGameplayTag, float> WeightedTags, float WeightSum,
	FGameplayTag& PickedTag)
{
	// 기준값을 설정합니다
	float LocalWeightSelection = FMath::FRandRange(0.0f, WeightSum);

	// WeightedTags의 키값들을 가져옵니다.
	TArray<FGameplayTag> Keys;
	WeightedTags.GetKeys(Keys);

	float LocalWeightCount=0;
	for (FGameplayTag Key : Keys)
	{
		// 가중치들의 합을 누적시킵니다.
		LocalWeightCount +=WeightedTags[Key];

		// 누적한 값이 설정한 기준값보다 높아지면 해당 GameplayTag를 반환합니다.
		// 기준값은 가중치들의 총합보다 같거나 작기때문에 문제가 생기지 않는이상 if문 내부로 들어가게 됩니다.
		if(LocalWeightCount>=LocalWeightSelection)
		{
			PickedTag = Key;
			return true;
		}
	}

	return false;
}

