
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ScreenFade.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ScreenFade : public UUserWidget
{
	GENERATED_BODY()


public:
	UUW_ScreenFade(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Transient, meta=(BindWidgetAnim),Category="UW Screen Fade")
	TObjectPtr<UWidgetAnimation> Occlude;

};
