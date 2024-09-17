#include "ActionSystem/Action_LoadMap.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetTableRef.h"
#include "UI/UW_ScreenFade.h"

AAction_LoadMap::AAction_LoadMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	if (TSubclassOf<UUW_ScreenFade> WBP_ScreenFade_Ref = FAssetReferenceUtility::LoadBlueprintClass<UUW_ScreenFade>(FName("WBP_ScreenFade")))
	{
		WBP_ScreenFadeClass = WBP_ScreenFade_Ref;
	}
}

void AAction_LoadMap::BeginPlay()
{
	Super::BeginPlay();

	UUW_ScreenFade* WBP_ScreenFade = CreateWidget<UUW_ScreenFade>(GetWorld(), WBP_ScreenFadeClass);
	WBP_ScreenFade->AddToViewport();
	WBP_ScreenFade->PlayAnimation(WBP_ScreenFade->Occlude);

	float OccludeEndTime = WBP_ScreenFade->Occlude->GetEndTime() + 0.01f;
	FTimerHandle OccludeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OccludeTimerHandle, [this]()
	{
		UGameplayStatics::OpenLevel(this, FName(*Level));
	}, OccludeEndTime, false);
}
