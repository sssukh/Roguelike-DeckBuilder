#include "ActionSystem/Action_ScreenShake.h"

#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosLog.h"


AAction_ScreenShake::AAction_ScreenShake()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> BP_ShakeClass(*AssetPath::Blueprint::BP_ScreenShake_C);
	if (BP_ShakeClass.Succeeded())
	{
		ShakeClass = BP_ShakeClass.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("BP_ShakeClass를 로드하지 못했습니다."));
	}
}

void AAction_ScreenShake::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_ScreenShake::PlayAction_Implementation()
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraShake(ShakeClass, Scale, ECameraShakePlaySpace::World);
	Execute_EndAction(this);
}
