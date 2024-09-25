#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_TargetArrow.h"
#include "UW_TargetingBezier.generated.h"

class UImage;
class UCanvasPanel;
class UUW_TargetingBezierPoint;

/**
 * UUW_TargetingBezier 클래스는 타겟팅 시스템에서 베지어 곡선을 기반으로 타겟팅 화살표를 표시하는 UI 위젯입니다.
 * 이 위젯은 세 개의 제어 포인트(Control Points)를 사용해 베지어 곡선을 생성하며, 곡선 위에 화살표 또는 포인트들을 표시합니다.
 * 플레이어가 타겟을 선택하거나 특정 지점을 지시할 때 동적인 베지어 곡선을 통해 시각적인 피드백을 제공합니다.
 * 
 * 주요 기능:
 * - 제어 포인트를 기반으로 곡선을 계산하여 화살표를 배치하고 회전 각도를 설정합니다.
 * - 마우스 위치에 따라 제어 포인트를 업데이트하여 실시간으로 베지어 곡선을 수정합니다.
 * - 곡선의 각 지점에서 애니메이션을 통해 시각적인 포인트를 표시합니다.
 */
UCLASS()
class CROWNOFSIN_API UUW_TargetingBezier : public UUserWidget, public IInterface_TargetArrow
{
	GENERATED_BODY()

public:
	UUW_TargetingBezier(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category="UW Targeting Bezier")
	void UpdateBezierPoints();

	UFUNCTION(BlueprintCallable, Category="UW Targeting Bezier")
	void UpdateControlPointsByMousePosition();

	UFUNCTION(BlueprintCallable, Category="UW Targeting Bezier")
	void UpdateTargetArrowFunction(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus);

	UFUNCTION(BlueprintCallable, Category="UW Targeting Bezier")
	void GetPositionAtBezierAlpha(float InAlpha, FVector2D& OutPosition, float& OutAngle);

	/*========================================================================================
	*	IInterface_TargetArrow
	=========================================================================================*/
public:
	virtual void UpdateTargetArrow_Implementation(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UCanvasPanel* BezierPanel;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UCanvasPanel* ControlPointPanel;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UImage* ControlPoint_1;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UImage* ControlPoint_2;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier|Desinger", meta=(BindWidget))
	UImage* ControlPoint_3;

public:
	// 베지어 곡선상의 각 포인트에 대한 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier|Config|Setup")
	TSubclassOf<UUW_TargetingBezierPoint> WBP_TargetingBezierPointClass;

	// 첫 번째 포인트를 숨길지 여부를 나타내는 플래그
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	bool bHideFirstPoint = true;

	// 생성할 베지어 포인트의 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	int32 NumBezierPoints = 14;

	// 화살표의 크기를 스케일링하는 값
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float ArrowSizeIndexScale = 2.0f;

	// 베지어 곡선의 X축 오프셋을 조정하는 값
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float BezierXOffsetMultiplier = 0.3f;

	// 베지어 곡선의 Y축 오프셋 값
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float BezierHeightOffset = 15.0f;

	// 곡선 끝부분의 화살표 크기 스케일
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float FrontArrowScale = 3.0f;

	// 기본 화살표 크기
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	float BaseArrowSize = 30.0f;

	// 첫 번째 제어 포인트의 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	FVector2D ControlPoint1;

	// 두 번째 제어 포인트의 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	FVector2D ControlPoint2;

	// 세 번째 제어 포인트의 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	FVector2D ControlPoint3;

	// 베지어 포인트에 사용할 텍스처
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier", meta=(ExposeOnSpawn="true"))
	UTexture2D* BezierPointTexture;

	// 베지어 곡선 위의 포인트들을 저장하는 배열
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier")
	TArray<UUW_TargetingBezierPoint*> BezierPoints;
};
