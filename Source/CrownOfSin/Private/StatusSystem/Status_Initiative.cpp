#include "StatusSystem/Status_Initiative.h"

#include "Core/TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/DelayHelper.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UStatus_Initiative::UStatus_Initiative(): BeginDelayHelper(nullptr)
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

	if(AActor* TurnManagerActor = UGameplayStatics::GetActorOfClass(this, ATurnManager::StaticClass()) )
	{
		ATurnManager* TurnManager = Cast<ATurnManager>(TurnManagerActor);

		// 지연을 담당할 DelayHelper 생성
		BeginDelayHelper = NewObject<UDelayHelper>(this);

		// 델리게이트 생성 및 바인딩
		FConditionDelegate ConditionDelegate;
		ConditionDelegate.BindUObject(this, &UStatus_Initiative::CheckTurnManagerSetup);

		FOnLoopDelegate OnLoopDelegate;
		OnLoopDelegate.BindUObject(this, &UStatus_Initiative::LogLoopProgress);

		FOnCompleteDelegate OnCompleteDelegate;
		OnCompleteDelegate.BindUObject(this, &UStatus_Initiative::OnSetupComplete);

		// DelayWhile 함수 호출: 0초 딜레이로 조건을 반복 확인
		BeginDelayHelper->DelayWhile(ConditionDelegate, OnLoopDelegate, OnCompleteDelegate, 0.0f);
	}
		
}

bool UStatus_Initiative::CheckTurnManagerSetup()
{
	ATurnManager* TurnManager = Cast<ATurnManager>(UGameplayStatics::GetActorOfClass(this, ATurnManager::StaticClass()));
	if (!TurnManager)
	{
		return true; // 조건 불충족으로 처리
	}

	return !TurnManager->bSetUpDone;
}

void UStatus_Initiative::LogLoopProgress(int32 LoopIndex)
{
}

void UStatus_Initiative::OnSetupComplete()
{
	AActor* Owner = GetOwner();

	// 글로벌 디스패처 허브에 이벤트 호출
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_TargetSpawned, Owner);

	// BeginDelayHelper를 nullptr로 설정하여 더 이상 참조하지 않도록 함
	BeginDelayHelper = nullptr;
}
