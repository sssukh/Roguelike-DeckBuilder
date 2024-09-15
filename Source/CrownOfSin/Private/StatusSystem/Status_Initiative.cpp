#include "StatusSystem/Status_Initiative.h"

#include "Core/TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/DelayHelper.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"

UStatus_Initiative::UStatus_Initiative()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
	SlotType = EStatusSlot::None;
}

void UStatus_Initiative::BeginPlay()
{
	Super::BeginPlay();

	ATurnManager* TurnManager = Cast<ATurnManager>(UGameplayStatics::GetActorOfClass(this, ATurnManager::StaticClass()));
	if (!TurnManager) return;

	// 지연을 담당할 DelayHelper 생성
	UDelayHelper* DelayHelper = NewObject<UDelayHelper>(this);

	auto CheckTurnManagerSetup = [TurnManager]() -> bool
	{
		return TurnManager->bSetUpDone;
	};

	auto LogLoopProgress = [](int32 LoopIndex)
	{
	};

	auto OnSetupComplete = [this, &DelayHelper]()
	{
		UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_TargetSpawned, GetOwner());
		DelayHelper = nullptr;
	};

	// DelayWhile 함수 호출: 0초 딜레이로 조건을 반복 확인
	DelayHelper->DelayWhile(CheckTurnManagerSetup, LogLoopProgress, OnSetupComplete, 0.0f);
}
