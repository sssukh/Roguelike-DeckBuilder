#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_StatusBar.generated.h"

class UUW_StatusIcon;
class UVerticalBox;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StatusBar : public UUserWidget, public IInterface_StatusWidget
{
	GENERATED_BODY()

public:
	UUW_StatusBar(const FObjectInitializer& Initializer);

public:
	UFUNCTION(BlueprintCallable, Category="UW Status Bar Event")
	void ClearAllStatuses();

	UFUNCTION(BlueprintPure, Category="UW Status Bar Event")
	int32 GetStatusCount();

	UFUNCTION(BlueprintCallable, Category="UW Status Bar Event")
	bool GetNumberOfVisibleRewards(int32& OutVisibleRewardCount);

	UFUNCTION(BlueprintCallable, Category="UW Status Bar Event")
	void InteractWithAllStatuses();


	/*========================================================================================
	*	Interface_StatusWidget
	=========================================================================================*/
public:
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Bar|Designer", meta=(BindWidget))
	UHorizontalBox* StatusBoxHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Bar|Designer", meta=(BindWidget))
	UVerticalBox* StatusBoxVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Bar|Config|Setup")
	TSubclassOf<UUW_StatusIcon> WBP_StatusIconClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Bar", meta=(ExposeOnSpawn="true"))
	bool bVertical = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Bar", meta=(ExposeOnSpawn="true"))
	FGameplayTag StatusBarTag;
};