#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"
#include "UW_StoryButton.generated.h"


class UUW_ToolTip;
class UButton;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmptyButtonClicked, UUserWidget*, InCallingWidget);

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StoryButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_StoryButton(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClicked_StoryButton();

public:
	UFUNCTION(BlueprintCallable, Category="UW Story Button Event")
	void RefreshPlayability();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Story Button|Desinger", meta=(BindWidget))
	UButton* StoryButton;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Story Button|Config|Setup")
	TSubclassOf<UUW_ToolTip> WBPToolTipClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UW Story Button", meta=(ExposeOnSpawn="true"))
	FCard EncounterCard;

	UPROPERTY(BlueprintReadWrite, Category="UW Story Button")
	ACardBase* SpawnedCard;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Story Button|Delegate")
	FOnEmptyButtonClicked OnEmptyButtonClicked;
};
