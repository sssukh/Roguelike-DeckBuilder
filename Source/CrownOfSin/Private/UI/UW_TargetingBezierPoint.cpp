#include "UI/UW_TargetingBezierPoint.h"

#include "Components/Image.h"
#include "Libraries/AssetTableRef.h"

UUW_TargetingBezierPoint::UUW_TargetingBezierPoint(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), PointImage(nullptr), Bounce(nullptr), Texture(nullptr)
{
	if (UTexture2D* T_TargetArrow = FAssetReferenceUtility::LoadAssetFromDataTable<UTexture2D>(AssetRefPath::TexturesPath, FName(TEXT("T_TargetArrow"))))
	{
		Texture = T_TargetArrow;
	}
}

void UUW_TargetingBezierPoint::NativePreConstruct()
{
	Super::NativePreConstruct();

	PointImage->SetBrushFromTexture(Texture);
}

void UUW_TargetingBezierPoint::FocusTargetArrow_Implementation()
{
	PlayAnimation(Bounce);
}
