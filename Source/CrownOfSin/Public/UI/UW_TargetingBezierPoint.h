#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_TargetArrow.h"
#include "UW_TargetingBezierPoint.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_TargetingBezierPoint : public UUserWidget, public IInterface_TargetArrow
{
	GENERATED_BODY()

public:
	UUW_TargetingBezierPoint(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	/*========================================================================================
	*	Interface_TargetArrow
	=========================================================================================*/
public:
	virtual void FocusTargetArrow_Implementation() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier Point|Desinger", meta=(BindWidget))
	UImage* PointImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier Point|Desinger", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Bounce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier Point", meta=(ExposeOnSpawn="true"))
	UTexture2D* Texture;


};
