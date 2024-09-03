#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_RewardScreen.generated.h"

class UTextBlock;
class UUW_StatusBar;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelection, bool, bIsSkipped, UStatusComponent*, InArtifact);

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_RewardScreen : public UUserWidget, public IInterface_StatusWidget
{
	GENERATED_BODY()

public:
	UUW_RewardScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category="UW Reward Screen Event")
	void Appear();

protected:
	UFUNCTION()
	void OnClicked_TakeAllButton();

	UFUNCTION()
	void OnClicked_SkipButton();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UButton* TakeAllButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UButton* SkipButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UTextBlock* ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UUW_StatusBar* WBP_ArtifactBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen")
	bool bCloseWhenEmpty = true;

	UPROPERTY(Blueprintable, BlueprintCallable, Category="UW Reward Screen|Delegate")
	FOnReturnSelection OnReturnSelection;
};
