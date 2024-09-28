#include "ActionSystem/Action_SplashText.h"

#include "Interfaces/Interface_CardPuppet.h"
#include "Libraries/AssetPath.h"
#include "UI/Actors/TextSplash.h"
#include "Utilities/CosLog.h"


AAction_SplashText::AAction_SplashText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor> BP_TextSplash_C(*AssetPath::Blueprint::BP_TextSplash_C);
	if (BP_TextSplash_C.Succeeded())
	{
		BP_TextSplashClass = BP_TextSplash_C.Class;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("BP_TextSplashClass를 로드하지 못했습니다."));
	}
}

void AAction_SplashText::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_SplashText::PlayAction_Implementation()
{
	COS_IF_CHECK_VOID(Puppet, TEXT("Puppet이 유효하지 않습니다."));

	COS_IF_CHECK_VOID(Puppet->Implements<UInterface_CardPuppet>(), TEXT("Puppet은 UInterface_CardPuppet를 상속받지 못했습니다."));

	// Puppet의 중심 좌표를 가져옴
	FVector PuppetCenter = IInterface_CardPuppet::Execute_GetPuppetRelativeCenter(Puppet);

	// 스폰할 위치 계산 (Puppet의 위치에 상대적 중심 좌표와 오프셋을 추가)
	FVector SpawnLocation = (PuppetCenter + Puppet->GetActorLocation()) + FVector(0, 0, 70);

	// Splash 텍스트 클래스를 설정하지 않았다면 함수 종료
	COS_IF_CHECK_VOID(BP_TextSplashClass, TEXT("BP_TextSplashClass를 설정해주세요."));

	// 스폰할 Transform 설정
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(SpawnLocation);

	// Splash 텍스트 액터를 지연 스폰하고, 설정된 텍스트와 색상 적용
	if (ATextSplash* NewTextSplash = GetWorld()->SpawnActorDeferred<ATextSplash>(BP_TextSplashClass, SpawnTransform,
	                                                                             nullptr, nullptr,
	                                                                             ESpawnActorCollisionHandlingMethod::Undefined,
	                                                                             ESpawnActorScaleMethod::OverrideRootScale))
	{
		// Splash 텍스트의 표시 내용과 색상 설정
		NewTextSplash->DisplayText = DisplayText;
		NewTextSplash->Color = Color.ToFColor(true);

		// 스폰 완료
		NewTextSplash->FinishSpawning(SpawnTransform);
	}

	// 액션 종료
	Execute_EndAction(this);
}

bool AAction_SplashText::AttemptDestroyAction_Implementation()
{
	return true;
}
