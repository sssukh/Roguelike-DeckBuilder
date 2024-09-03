#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardHand.generated.h"

class UUW_Anchor;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_CardHand : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_CardHand(const FObjectInitializer& ObjectInitializer);


	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category="UW Card Hand Event")
	void AddAnchorWidgets(const TMap<FGameplayTag, UUserWidget*>& InAnchorWidgets);

	
	UFUNCTION(BlueprintCallable, Category="UW Card Hand Event")
	void UpdatePeriodically();

protected:
	void UpdateCardTransforms();
	
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_Anchor* WBP_HandAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_Anchor* WBP_PlayAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_Anchor* WBP_ReshuffleAnchor;

	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	float UpdatePeriod = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	TMap<FGameplayTag, UUserWidget*> AnchorWidgets;

private:
	// 일정 시간이 지나면 카드를 업데이트하기 위한 타이머 핸들
	FTimerHandle UpdateTimerHandle;
};
