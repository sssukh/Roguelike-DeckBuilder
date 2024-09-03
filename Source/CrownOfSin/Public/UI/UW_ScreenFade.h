
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Screen Fade", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Occlude;
};
