#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_GameOver.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_GameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_GameOver(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category="UW Game Over")
	void ShowGameOver(const FText& InText, FLinearColor InColorAndOpacitySpecifiedColor);

protected:
	UFUNCTION()
	void OnClicked_MenuButton();

	UFUNCTION()
	void OnClicked_QuitButton();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Game Over|Designer", meta=(BindWidget))
	UTextBlock* GameOverText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Game Over|Designer", meta=(BindWidget))
	UButton* MenuButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Game Over|Designer", meta=(BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Game Over|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Game Over|Config|Setup")
	TSoftObjectPtr<UWorld> Level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Game Over",meta=(ExposeOnSpawn="true"))
	FText TextToShow;
};
