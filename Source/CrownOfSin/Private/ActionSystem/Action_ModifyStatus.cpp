#include "ActionSystem/Action_ModifyStatus.h"

#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Widget/Interface_StatusIcon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/Actors/IconSplash.h"
#include "UI/Actors/TextSplash.h"
#include "Utilities/CosLog.h"


AAction_ModifyStatus::AAction_ModifyStatus(): StatusReference(nullptr), bRefreshAppearance(false), StatusIndicator(nullptr), StatusOwner(nullptr), IconImage(nullptr), Puppet(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AIconSplash> BP_IconSplash(*AssetPath::Blueprint::BP_IconSplash_C);
	if (BP_IconSplash.Succeeded())
	{
		BP_AIconSplashClass = BP_IconSplash.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("BP_IconSplash 클래스를 로드하지 못했습니다."));
	}

	static ConstructorHelpers::FClassFinder<ATextSplash> BP_TextSplash(*AssetPath::Blueprint::BP_TextSplash_C);
	if (BP_TextSplash.Succeeded())
	{
		BP_TextSplashClass = BP_TextSplash.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("BP_TextSplash 클래스를 로드하지 못했습니다."));
	}
}

void AAction_ModifyStatus::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_ModifyStatus::SetUpAction_Implementation()
{
	StatusAppearance = StatusReference->MakeAppearanceStruct();
	StatusOwner = StatusReference->GetOwner();
	StatusIndicator = StatusReference->StatusIndicator;

	if (StatusOwner && StatusOwner->Implements<UInterface_CardTarget>())
	{
		Puppet = IInterface_CardTarget::Execute_GetPuppet(StatusOwner);
	}
}

void AAction_ModifyStatus::PlayAction_Implementation()
{
	// 상태 아이콘을 변경할 수 있는지 확인
	if (StatusIndicator && StatusIndicator->Implements<UInterface_StatusIcon>())
	{
		// 상태 위젯에 상태를 반영
		IInterface_StatusIcon::Execute_ModifyStatusWidget(StatusIndicator, NewValue, TextOverride, bRefreshAppearance, StatusAppearance, bCanBeZero);
	}

	// 스플래시 아이콘을 화면에 표시해야 하는지 확인
	if (bShowSplashIcon && Puppet && Puppet->Implements<UInterface_CardTarget>())
	{
		// 대상 위치와 카메라 위치 계산
		FVector PuppetCenter = IInterface_CardTarget::Execute_GetPuppetRelativeCenter(Puppet);
		FVector TargetLocation = PuppetCenter + Puppet->GetActorLocation();
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();

		// 카메라에서 대상 위치로의 보간 위치 계산
		FVector SplashLocation = UKismetMathLibrary::VLerp(CameraLocation, TargetLocation, 0.2f);

		// 스플래시 아이콘 스폰 위치 설정
		FTransform SplashTransform = FTransform::Identity;
		SplashTransform.SetLocation(SplashLocation);

		// 스플래시 아이콘을 스폰
		if (AIconSplash* IconSplash = GetWorld()->SpawnActorDeferred<AIconSplash>(
			BP_AIconSplashClass, SplashTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			IconSplash->Icon = StatusAppearance.Icon;
			IconSplash->Color = StatusAppearance.Tint;
			IconSplash->FinishSpawning(SplashTransform);
		}
	}

	// 스플래시 텍스트를 표시해야 하는지 확인
	if (!SplashText.IsEmpty() && bShowSplashNumber && Puppet && Puppet->Implements<UInterface_CardTarget>())
	{
		// 텍스트 스플래시를 위한 위치 계산
		FVector PuppetCenter = IInterface_CardTarget::Execute_GetPuppetRelativeCenter(Puppet);
		FVector TargetLocation = PuppetCenter + Puppet->GetActorLocation();
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
		FVector TextSplashLocation = UKismetMathLibrary::VLerp(CameraLocation, TargetLocation, 0.2f) + FVector(0, 0, 70.0f);

		// 스플래시 텍스트 스폰 위치 설정
		FTransform TextSplashTransform = FTransform::Identity;
		TextSplashTransform.SetLocation(TextSplashLocation);

		// 스플래시 텍스트를 스폰
		if (ATextSplash* TextSplash = GetWorld()->SpawnActorDeferred<ATextSplash>(
			BP_TextSplashClass, TextSplashTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			TextSplash->DisplayText = SplashText;
			TextSplash->Color = StatusAppearance.Tint.ToFColor(true);
			TextSplash->FinishSpawning(TextSplashTransform);
		}
	}

	// 액션을 종료
	Execute_EndAction(this);
}
