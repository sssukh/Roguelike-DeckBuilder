// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_Discover.generated.h"


class UUW_CardRewardScreen;

/*
 *	카드 이펙트의 대상으로 선정할 카드들을 나타냅니다.
 *	카드 발견효과, 스토리 보상카드 그리고 전투 보상에 사용됩니다.
 *	UsedData나 gameplayTag가 지정되지 않으면 3개의 카드를 무작위로 가져옵니다.
 *	UsedData가 지정되지 않고 GameplayTag에 Flag.FilterToAllowed가 지정되면 게임인스턴스의 AllowedCardRewards에서 선택됩니다.
 *	특정한 카드를 나타내려면 GameplayTags에 Flag.Specific을 추가하고 UsedData를 Specific Card로 설정하세요.
 *	가중치가 적용된 카드들을 나타내려면 UsedData를 FRarityWeight기반 테이블로 설정하세요
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_Discover : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_Discover();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;

	UFUNCTION(BlueprintCallable, Category = "TargetingComponent Discover")
	void BindToCardConfirm(UUW_CardRewardScreen* InCardRewardScreen);

	UFUNCTION(BlueprintCallable, Category = "TargetingComponent Discover")
	void ValidateAndTransferSelectedCard(bool bSkipped, ACardBase* Card);

	TArray<FCard> SetCardOptions(const FCardEffect& CardEffect);

	TArray<FCard> SetCardOptionsWithRarity(const FCardEffect& CardEffect);

public:
	// Member Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetingComponent Discover")
	TArray<ACardBase*> CurrentCardOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetingComponent Discover")
	FCard CardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetingComponent Discover")
	TArray<AActor*> EmptyArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetingComponent Discover")
	TObjectPtr<UUW_CardRewardScreen> CardRewardScreen;
};
