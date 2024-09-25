#include "UI/UW_TargetingBezier.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Utility.h"
#include "UI/UW_TargetingBezierPoint.h"
#include "Utilities/CosLog.h"

UUW_TargetingBezier::UUW_TargetingBezier(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), BezierPanel(nullptr), ControlPointPanel(nullptr), ControlPoint_1(nullptr),
                                                                                        ControlPoint_2(nullptr),
                                                                                        ControlPoint_3(nullptr),
                                                                                        BezierPointTexture(nullptr)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_TargetingBezierPoint(*AssetPath::Blueprint::WBP_TargetingBezierPoint_C);
	if (WBP_TargetingBezierPoint.Succeeded())
	{
		WBP_TargetingBezierPointClass = WBP_TargetingBezierPoint.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_TargetingBezierPoint를 로드하지 못했습니다."));
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_TargetArrow(*AssetPath::Texture::T_TargetArrow);
	if (T_TargetArrow.Succeeded())
	{
		BezierPointTexture = T_TargetArrow.Object;
	}
	else
	{
		COS_LOG_ERROR(TEXT("T_TargetArrow를 로드하지 못했습니다."));
	}

	ControlPoint1 = FVector2D(860.0f, 928.0f);
	ControlPoint2 = FVector2D(880.0f, 436.0f);
	ControlPoint3 = FVector2D(1345.0f, 394.0f);
}

void UUW_TargetingBezier::NativePreConstruct()
{
	Super::NativePreConstruct();

	COS_IF_CHECK(WBP_TargetingBezierPointClass, TEXT("UUW_TargetingBezier 에서 WBP_TargetingBezierPointClass를 설정해주세요"));

	// 기존의 BezierPoints 배열을 비웁니다.
	BezierPoints.Empty();

	// NumBezierPoints 수만큼 새로운 Bezier 포인트 위젯을 생성하고 설정
	for (int32 PointIndex = 0; PointIndex < NumBezierPoints; ++PointIndex)
	{
		// 새로운 Bezier 포인트 위젯 생성
		UUW_TargetingBezierPoint* NewBezierPoint = CreateWidget<UUW_TargetingBezierPoint>(GetWorld(), WBP_TargetingBezierPointClass);
		NewBezierPoint->Texture = BezierPointTexture;

		// Bezier 패널에 새 위젯 추가
		BezierPanel->AddChild(NewBezierPoint);
		BezierPoints.Add(NewBezierPoint);

		// 캔버스 슬롯에 사이즈와 정렬 설정
		UCanvasPanelSlot* NewPointSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(NewBezierPoint);
		FVector2D NewPointSize((PointIndex * ArrowSizeIndexScale) + BaseArrowSize);
		NewPointSlot->SetSize(NewPointSize);
		NewPointSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	}

	// 마지막 Bezier 포인트의 크기 설정
	UUW_TargetingBezierPoint* LastBezierPoint = BezierPoints.Last();
	UCanvasPanelSlot* LastPointSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LastBezierPoint);
	FVector2D LastPointSize(BaseArrowSize * FrontArrowScale);
	LastPointSlot->SetSize(LastPointSize);

	// 각 제어 포인트(Control Point)의 초기 위치 설정
	UCanvasPanelSlot* ControlPoint1Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_1);
	ControlPoint1Slot->SetPosition(ControlPoint1);

	UCanvasPanelSlot* ControlPoint2Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_2);
	ControlPoint2Slot->SetPosition(ControlPoint2);

	UCanvasPanelSlot* ControlPoint3Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_3);
	ControlPoint3Slot->SetPosition(ControlPoint3);

	// Bezier 포인트 위치 업데이트
	UpdateBezierPoints();
}

void UUW_TargetingBezier::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateControlPointsByMousePosition();

	UpdateBezierPoints();
}

void UUW_TargetingBezier::UpdateBezierPoints()
{
	// BezierPoints 배열을 순회하면서 각 포인트의 위치와 각도를 업데이트
	for (int32 PointIndex = 0; PointIndex < BezierPoints.Num(); ++PointIndex)
	{
		// 현재 Bezier 포인트의 캔버스 슬롯 가져오기
		UCanvasPanelSlot* CurrentBezierSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(BezierPoints[PointIndex]);

		// 포인트의 위치와 각도를 계산하기 위한 변수들
		FVector2D CalculatedPosition;
		float CalculatedAngle;

		// Bezier 곡선상의 현재 포인트의 비율 계산 (Alpha)
		float BezierAlpha = PointIndex / static_cast<float>(NumBezierPoints);

		// 주어진 Alpha 값에 따라 위치와 각도를 계산
		GetPositionAtBezierAlpha(BezierAlpha, CalculatedPosition, CalculatedAngle);

		// 계산된 위치를 현재 Bezier 포인트의 캔버스 슬롯에 설정
		CurrentBezierSlot->SetPosition(CalculatedPosition);

		// 계산된 각도를 현재 Bezier 포인트에 설정
		BezierPoints[PointIndex]->SetRenderTransformAngle(CalculatedAngle);
	}

	// 첫 번째 Bezier 포인트의 가시성을 숨김 처리
	if (bHideFirstPoint && BezierPoints.Num() > 0)
	{
		BezierPoints[0]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUW_TargetingBezier::UpdateControlPointsByMousePosition()
{
	// 현재 뷰포트에서 마우스 위치를 가져옴
	FVector2D CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// ControlPoint_2의 CanvasSlot을 가져와 위치를 구함
	UCanvasPanelSlot* ControlPoint2Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_2);
	FVector2D ControlPoint2CurrentPosition = ControlPoint2Slot->GetPosition();

	// 마우스 위치와 설정된 오프셋을 이용해 새로운 Y 좌표 계산
	float OffsetY = CurrentMousePosition.Y + BezierHeightOffset;
	float AdjustedXOffset = FMath::Abs(CurrentMousePosition.X - ControlPoint2CurrentPosition.X) * BezierXOffsetMultiplier;
	float UpdatedYPosition = OffsetY - AdjustedXOffset;

	// 새로운 위치로 ControlPoint_2의 Y 좌표 업데이트
	FVector2D UpdatedControlPoint2Position(ControlPoint2CurrentPosition.X, UpdatedYPosition);
	ControlPoint2Slot->SetPosition(UpdatedControlPoint2Position);

	// ControlPoint_3의 CanvasSlot을 가져와 마우스 위치로 설정
	UCanvasPanelSlot* ControlPoint3Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_3);
	ControlPoint3Slot->SetPosition(CurrentMousePosition);
}

void UUW_TargetingBezier::UpdateTargetArrowFunction(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus)
{
	// 화살표에 포커스 애니메이션을 적용할지 확인
	if (bIsAnimateFocus)
	{
		UUW_TargetingBezierPoint* LastBezierPoint = BezierPoints.Last();
		IInterface_TargetArrow::Execute_FocusTargetArrow(LastBezierPoint);
	}

	// ControlPointPanel의 가시성 설정
	ESlateVisibility PanelVisibility = bIsVisibility ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
	ControlPointPanel->SetVisibility(PanelVisibility);

	// Bezier 포인트의 색상 업데이트
	for (UUW_TargetingBezierPoint* CurrentBezierPoint : BezierPoints)
	{
		CurrentBezierPoint->PointImage->SetBrushTintColor(InTint);
	}

	// 소스 포인트를 업데이트할지 여부 확인
	if (bIsUpdateSourcePoint)
	{
		// ControlPoint_1의 위치를 InSourcePoint로 설정
		UCanvasPanelSlot* ControlPoint1Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_1);
		ControlPoint1Slot->SetPosition(InSourcePoint);

		// ControlPoint_2의 X 좌표를 InSourcePoint의 X로, Y 좌표는 그대로 유지하여 설정
		UCanvasPanelSlot* ControlPoint2Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_2);
		FVector2D ControlPoint2NewPosition(InSourcePoint.X, ControlPoint2Slot->GetPosition().Y);
		ControlPoint2Slot->SetPosition(ControlPoint2NewPosition);
	}
}

void UUW_TargetingBezier::GetPositionAtBezierAlpha(float InAlpha, FVector2D& OutPosition, float& OutAngle)
{
	// Control points의 CanvasSlot을 가져옴
	UCanvasPanelSlot* ControlPoint1Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_1);
	UCanvasPanelSlot* ControlPoint2Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_2);
	UCanvasPanelSlot* ControlPoint3Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ControlPoint_3);

	// 첫 번째 및 두 번째 제어점 간의 선형 보간
	FVector2D LerpBetweenPoint1And2 = UFunctionLibrary_Utility::LerpVector2D(ControlPoint1Slot->GetPosition(), ControlPoint2Slot->GetPosition(), InAlpha);

	// 두 번째 및 세 번째 제어점 간의 선형 보간
	FVector2D LerpBetweenPoint2And3 = UFunctionLibrary_Utility::LerpVector2D(ControlPoint2Slot->GetPosition(), ControlPoint3Slot->GetPosition(), InAlpha);

	// 위의 두 보간 위치 간의 선형 보간 결과 (최종 Bezier 곡선 위치)
	FVector2D FinalBezierPosition = UFunctionLibrary_Utility::LerpVector2D(LerpBetweenPoint1And2, LerpBetweenPoint2And3, InAlpha);

	// 3D 공간에서의 시작 위치 설정
	FVector StartLocation(FinalBezierPosition.X, FinalBezierPosition.Y, 0);

	// 작은 오프셋을 사용하여 타겟 위치 계산 (알파 값 조정)
	float AdjustedAlpha = InAlpha + 0.001f;

	// 조정된 알파 값을 사용하여 타겟 위치 계산
	FVector2D AdjustedLerpPoint1And2 = UFunctionLibrary_Utility::LerpVector2D(ControlPoint1Slot->GetPosition(), ControlPoint2Slot->GetPosition(), AdjustedAlpha);
	FVector2D AdjustedLerpPoint2And3 = UFunctionLibrary_Utility::LerpVector2D(ControlPoint2Slot->GetPosition(), ControlPoint3Slot->GetPosition(), AdjustedAlpha);
	FVector2D AdjustedFinalBezierPosition = UFunctionLibrary_Utility::LerpVector2D(AdjustedLerpPoint1And2, AdjustedLerpPoint2And3, AdjustedAlpha);

	// 3D 공간에서의 타겟 위치 설정
	FVector TargetLocation(AdjustedFinalBezierPosition.X, AdjustedFinalBezierPosition.Y, 0);

	// 시작 위치에서 타겟 위치로의 회전 각도 계산
	OutAngle = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation).Yaw;

	// 최종 Bezier 위치를 출력 위치로 설정
	OutPosition = FinalBezierPosition;
}

void UUW_TargetingBezier::UpdateTargetArrow_Implementation(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus)
{
	UpdateTargetArrowFunction(bIsVisibility, InTint, bIsUpdateSourcePoint, InSourcePoint, bIsAnimateFocus);
}
