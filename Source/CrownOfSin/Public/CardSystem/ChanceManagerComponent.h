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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*ToDo:구현해야 로직이 실행됩니다.*/
	TArray<FGameplayTag> GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags, int InAmount);

	bool PickTagFromWeightAndWeightSum(TMap<FGameplayTag,float> WeightedTags, float WeightSum, FGameplayTag& PickedTag);
};
