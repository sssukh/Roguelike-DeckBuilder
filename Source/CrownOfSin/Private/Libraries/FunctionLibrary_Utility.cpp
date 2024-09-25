#include "Libraries/FunctionLibrary_Utility.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CosLog.h"

void UFunctionLibrary_Utility::DisplayWarningIfMultipleSingletons(const UObject* WorldContextObject, TSubclassOf<AActor> InClass)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, InClass, OutActors);

	if (OutActors.Num() > 1)
	{
		FString ClassName = UKismetSystemLibrary::GetClassDisplayName(InClass);
		COS_LOG_SCREEN_ERROR(TEXT("경고: 싱글톤 클래스 %s의 여러 액터가 생성되었습니다. 레벨에는 하나만 존재해야 합니다. "), *ClassName);
	}
}

void UFunctionLibrary_Utility::SendScreenLogMessage(const UObject* WorldContextObject, FText Message, FColor Color)
{
	if (ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(WorldContextObject, ACardPlayer::StaticClass())))
		CardPlayer->DisplayScreenLogMessage(Message, Color);
}

FVector2D UFunctionLibrary_Utility::LerpVector2D(const FVector2D& A, const FVector2D& B, float Alpha)
{
	return FVector2D(FMath::Lerp(A.X, B.X, Alpha), // X 좌표의 선형 보간
	                 FMath::Lerp(A.Y, B.Y, Alpha)); // Y 좌표의 선형 보간
}

FName UFunctionLibrary_Utility::GenerateUniqueObjectName(UObject* Outer, UClass* ObjectClass, const FString& Suffix)
{
	// Get the class name of the object
	FString ClassName = GetNameSafe(ObjectClass);

	// Generate a base name using the class name and optional suffix
	FString BaseName = FString::Printf(TEXT("%s_%s"), *ClassName, *Suffix);

	// Use MakeUniqueObjectName to generate a unique name within the given outer context
	FName UniqueName = MakeUniqueObjectName(Outer, ObjectClass, FName(*BaseName));

	return UniqueName;
}

bool UFunctionLibrary_Utility::CheckObjectImplementsInterface(const UObject* WorldContextObject, UObject* Object, TSubclassOf<UInterface> InterfaceClass)
{
	// 객체가 nullptr인지 확인
	COS_IF_CHECK(Object != nullptr, TEXT("객체가 nullptr입니다."), false);

	// 인터페이스 클래스가 nullptr인지 확인
	COS_IF_CHECK(InterfaceClass != nullptr, TEXT("인터페이스 클래스가 nullptr입니다."), false);

	// 객체가 인터페이스를 구현하고 있는지 확인
	if (Object->GetClass()->ImplementsInterface(InterfaceClass))
	{
		return true;
	}

	FString ObjectName = Object->GetName();
	FString InterfaceName = InterfaceClass->GetName();
	COS_LOG_SCREEN_ERROR(TEXT("%s가 %s를 상속받지 못했습니다."), *ObjectName, *InterfaceName);
	return false;
}


FVector2D UFunctionLibrary_Utility::GetWidgetTickSpacePosition(UObject* WorldContextObject, UUserWidget* Widget)
{
	// 위젯의 픽셀 및 뷰포트 위치 계산
	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(WorldContextObject, Widget->GetTickSpaceGeometry(), Widget->GetRenderTransform().Translation, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UFunctionLibrary_Utility::GetPositionAndAngleAtBezierAlphaBetweenWidgets(UObject* WorldContextObject, float Alpha, UUserWidget* StartWidget, UUserWidget* MidWidget,
                                                                                   UUserWidget* EndWidget, float& OutAngle)
{
	// 각 위젯의 뷰포트 상 위치 계산
	FVector2D StartPosition = GetWidgetTickSpacePosition(WorldContextObject, StartWidget);
	FVector2D MidPosition = GetWidgetTickSpacePosition(WorldContextObject, MidWidget);
	FVector2D EndPosition = GetWidgetTickSpacePosition(WorldContextObject, EndWidget);


	// 베지어 곡선의 첫 번째 보간 단계
	FVector2D StartMidLerp = LerpVector2D(StartPosition, MidPosition, Alpha);
	FVector2D MidEndLerp = LerpVector2D(MidPosition, EndPosition, Alpha);
	FVector2D BezierPoint = LerpVector2D(StartMidLerp, MidEndLerp, Alpha);



	// LookAt 계산을 위한 현재 및 타겟 위치 계산
	FVector CurrentVector = FVector(BezierPoint, 0);
	FVector2D TargetStartMidLerp = LerpVector2D(StartPosition, MidPosition, Alpha + 0.001f);
	FVector2D TargetMidEndLerp = LerpVector2D(MidPosition, EndPosition, Alpha + 0.001f);
	FVector2D TargetBezierPoint = LerpVector2D(TargetStartMidLerp, TargetMidEndLerp, Alpha + 0.001f);
	FVector TargetVector = FVector(TargetBezierPoint, 0);

	// LookAt 각도 계산
	OutAngle = UKismetMathLibrary::FindLookAtRotation(CurrentVector, TargetVector).Yaw;

	return BezierPoint;
}