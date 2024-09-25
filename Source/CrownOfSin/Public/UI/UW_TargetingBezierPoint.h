#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_TargetArrow.h"
#include "UW_TargetingBezierPoint.generated.h"

class UImage;

/**
 * UUW_TargetingBezierPoint 클래스는 타겟팅 시스템에서 베지어 곡선의 특정 지점을 나타내는 UI 위젯입니다.
 * 주로 타겟팅 커서나 화살표가 특정 지점을 가리킬 때 사용되며, 이미지와 애니메이션을 통해 
 * 시각적인 피드백을 제공합니다. 이 위젯은 IInterface_TargetArrow 인터페이스를 구현하여 
 * 타겟팅 관련 인터랙션을 처리합니다.
 * 
 * 주요 기능:
 * - 이미지 위젯(PointImage)을 통해 타겟 지점을 시각적으로 표시합니다.
 * - FocusTargetArrow_Implementation 함수는 타겟팅 화살표가 집중될 때 애니메이션을 재생하여 사용자에게 시각적 피드백을 제공합니다.
 * - 베지어 곡선의 각 지점에 대한 커스터마이징된 이미지를 설정할 수 있으며, 이 이미지는 Texture를 통해 관리됩니다.
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
	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier Point|Desinger", meta=(BindWidget))
	UImage* PointImage;

	UPROPERTY(BlueprintReadWrite, Category="UW Targeting Bezier Point|Desinger", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Bounce;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Targeting Bezier Point", meta=(ExposeOnSpawn="true"))
	UTexture2D* Texture;
};
