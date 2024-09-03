#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_TargetArrow.h"
#include "UW_TargetingBezier.generated.h"

class UImage;
class UCanvasPanel;
class UUW_TargetingBezierPoint;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_TargetingBezier : public UUserWidget,public IInterface_TargetArrow
{
	GENERATED_BODY()

public:
	UUW_TargetingBezier(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	UFUNCTION(BlueprintCallable,Category="UW Targeting Bezier Event")
	void UpdateBezierPoints();
	
	UFUNCTION(BlueprintCallable,Category="UW Targeting Bezier Event")
	void UpdateControlPointsByMousePosition();
	
	UFUNCTION(BlueprintCallable,Category="UW Targeting Bezier Event")
	void UpdateTargetArrowFunction(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus);
	
	UFUNCTION(BlueprintCallable,Category="UW Targeting Bezier Event")
	void GetPositionAtBezierAlpha(float InAlpha,FVector2D& OutPosition,float& OutAngle);

	/*========================================================================================
	*	IInterface_TargetArrow
	=========================================================================================*/
public:
	virtual void UpdateTargetArrow_Implementation(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UCanvasPanel* BezierPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UCanvasPanel* ControlPointPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UImage* ControlPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UImage* ControlPoint_2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UImage* ControlPoint_3;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Config|Setup")
	TSubclassOf<UUW_TargetingBezierPoint> WBP_TargetingBezierPointClass;

	
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	bool bHideFirstPoint =true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	int32 NumBezierPoints = 14;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	UTexture2D* BezierPointTexture;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float ArrowSizeIndexScale = 2.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float BezierXOffsetMultiplier =0.3f;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float BezierHeightOffset =15.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float FrontArrowScale = 3.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float BaseArrowSize = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	FVector2D ControlPoint1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	FVector2D ControlPoint2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	FVector2D ControlPoint3;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	TArray<UUW_TargetingBezierPoint*> BezierPoints;
};
