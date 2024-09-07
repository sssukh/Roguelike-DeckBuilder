#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"
#include "UW_StoryButton.generated.h"


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

public:
	UFUNCTION(BlueprintCallable, Category="UW Story Button Event")
	void RefreshPlayability();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Story Button",meta=(BindWidget))
	UButton* StoryButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="UW Story Button", meta=(ExposeOnSpawn="true"))
	FCard EncounterCard;

	UPROPERTY(BlueprintReadWrite,Category="UW Story Button")
	ACardBase* SpawnedCard;


	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Story Button|Delegate")
	FOnEmptyButtonClicked OnEmptyButtonClicked;
};
