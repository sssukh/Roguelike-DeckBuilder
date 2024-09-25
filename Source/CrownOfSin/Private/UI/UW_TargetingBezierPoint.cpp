#include "UI/UW_TargetingBezierPoint.h"

#include "Components/Image.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosLog.h"

UUW_TargetingBezierPoint::UUW_TargetingBezierPoint(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), PointImage(nullptr), Bounce(nullptr), Texture(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_TargetArrow(*AssetPath::Texture::T_TargetArrow);
	if (T_TargetArrow.Succeeded())
	{
		Texture = T_TargetArrow.Object;
	}
	else
	{
		COS_LOG_ERROR(TEXT("T_TargetArrow를 로드하지 못했습니다."));
	}
}

void UUW_TargetingBezierPoint::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Texture)
		PointImage->SetBrushFromTexture(Texture);
}

void UUW_TargetingBezierPoint::FocusTargetArrow_Implementation()
{
	PlayAnimation(Bounce);
}
