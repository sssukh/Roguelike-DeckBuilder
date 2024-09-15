#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_SpecifiedCardsInPile.generated.h"


class UUW_CardSelectorList;

/*
 *	손에 있는 카드들 중 지정된 타겟 컴포넌트 더미에서 지정된 GameplayTags가 있는 카드들을 대상으로 삼습니다.
 *	만약 태그가 지정되지 않으면 해당 더미의 모든 카드들을 대상으로 삼습니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_SpecifiedCardsInPile : public UTargetingComponent
{
	GENERATED_BODY()

public:
	UTargetingComponent_SpecifiedCardsInPile();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;

	UFUNCTION(BlueprintCallable, Category="TargetingSystem SpecifiedCardsInPile")
	void BindToCardConfirm(UUW_CardSelectorList* CardList);

	UFUNCTION(BlueprintCallable, Category="TargetingSystem SpecifiedCardsInPile")
	void ValidateAndTransferSelectedCard(ACardBase* Card);

	// Member Variable
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetingSystem SpecifiedCardsInPile")
	UUW_CardSelectorList* CardSelectorList;
};
