
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_MinionUiTop.generated.h"

class UHorizontalBox;
class UUW_StatusIcon;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_MinionUiTop : public UUserWidget ,public IInterface_StatusWidget
{
	GENERATED_BODY()
public:
	UUW_MinionUiTop(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	/*========================================================================================
	*	IInterface_StatusWidget
	=========================================================================================*/
public:
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;

public:
	UPROPERTY(BlueprintReadWrite,Category="UW MinionUi Top|Designer",meta=(BindWidget))
	TObjectPtr<UHorizontalBox> ActiveStatusBox;
	
	UPROPERTY(BlueprintReadWrite,Category="UW MinionUi Top|Designer",meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> ActiveStatusWobble;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW MinionUi Top|Config|Setup")
	TSubclassOf<UUW_StatusIcon> WBP_StatusIconClass;
};
