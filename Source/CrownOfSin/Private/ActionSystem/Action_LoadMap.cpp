#include "ActionSystem/Action_LoadMap.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_ScreenFade.h"
#include "Utilities/CosLog.h"

AAction_LoadMap::AAction_LoadMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ScreenFade(*AssetPath::Blueprint::WBP_ScreenFade_C);
	if (WBP_ScreenFade.Succeeded())
	{
		WBP_ScreenFadeClass = WBP_ScreenFade.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ScreenFade 를 로드하지 못했습니다."));
	}
}

void AAction_LoadMap::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_LoadMap::PlayAction_Implementation()
{
	UUW_ScreenFade* WBP_ScreenFade = CreateWidget<UUW_ScreenFade>(GetWorld(), WBP_ScreenFadeClass);
	WBP_ScreenFade->AddToViewport();
	WBP_ScreenFade->PlayAnimation(WBP_ScreenFade->Occlude);

	float OccludeEndTime = WBP_ScreenFade->Occlude->GetEndTime() + 0.01f;
	FTimerHandle OccludeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OccludeTimerHandle, [this]()
	{
		UGameplayStatics::OpenLevel(this, FName(*Level));
	}, OccludeEndTime, false);

	Execute_EndAction(this); 
}
