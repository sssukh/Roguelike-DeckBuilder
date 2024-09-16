#include "ActionSystem/ActionManager.h"

#include "ActionSystem/ActionBase.h"
#include "Interfaces/Interface_CardAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/DelayHelper.h"
#include "Utilities/CosLog.h"


AActionManager::AActionManager()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AActionManager::BeginPlay()
{
	Super::BeginPlay();

	ActionQueue.SetNum(QueueSize);

	AttemptToPlayNextAction();
}

void AActionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActionsThisTick = 0;
}

void AActionManager::AttemptToPlayNextAction()
{
	// DelayHelper_NextAction 객체가 이미 존재하면 초기화(Reset)하고, 존재하지 않으면 새로 생성합니다.
	if (DelayHelper_NextAction)
	{
		DelayHelper_NextAction->Reset(); // 기존 DelayHelper의 상태 초기화
	}
	else
	{
		DelayHelper_NextAction = NewObject<UDelayHelper>(this); // 새로운 DelayHelper 객체 생성
	}

	// 큐의 다음 인덱스가 유효한지와 현재 틱에서 허용된 최대 액션 수를 초과하지 않았는지 확인하는 조건 람다 함수
	auto ShouldStopActionLoop = [this]()
	{
		bool bIsQueueInvalid = IsValid(ActionQueue[GetNextQueueIndex(QueueCounter)]); // 다음 큐에 유효한 액션이 있는지 확인합니다.
		bool bPerTickLimitReached = ActionsThisTick < MaxActionsPerTick; // 현재 틱에서 최대 액션 수를 초과했는지 확인합니다.
		return bIsQueueInvalid && bPerTickLimitReached; // 둘 다 참이어야 합니다.
	};

	// 조건이 거짓일 때 반복 호출될 함수
	auto OnLoop = [this](int32 Index)
	{
		// COS_SCREEN(TEXT("이것은 경고가 아닙니다.AttemptToPlayNextAction 함수의 OnLoopBody가 실행되고 있는 지 확인할려고 합니다. 카운트 :  %d"), Index);
	};

	// 조건이 참일 때 호출될 함수
	auto OnCompleted = [this]()
	{
		ActionsThisTick = ActionsThisTick + 1;
		QueueCounter = GetNextQueueIndex(QueueCounter);
		UObject* Action = ActionQueue[QueueCounter];
		IInterface_CardAction::Execute_AnimateAction(Action, this);
	};

	DelayHelper_NextAction->DelayWhile(ShouldStopActionLoop, OnLoop, OnCompleted, 0.0f);
}

void AActionManager::QueueAction(UObject* Action)
{
	// 다음 큐 인덱스를 가져와서 FillCounter를 업데이트합니다.
	FillCounter = GetNextQueueIndex(FillCounter);

	// FillCounter 위치에 액션이 비어 있는지 확인합니다.
	if (!IsValid(ActionQueue[FillCounter]))
	{
		// 액션 큐의 FillCounter 위치에 액션을 삽입합니다.
		// Insert 함수는 지정된 위치에 액션을 삽입하고, 그 뒤의 액션들을 뒤로 이동시킵니다.
		ActionQueue.Insert(Action, FillCounter);
	}
	else
	{
		// 큐에 빈 자리가 없으면 오류 메시지를 로그로 출력합니다.
		COS_SCREEN(TEXT("ERROR: Action Overflow"));
	}
}

void AActionManager::QueueDelay(float InDelay)
{
	FTransform SpawnTransform = FTransform::Identity;

	if (AActionBase* NewAction = GetWorld()->SpawnActorDeferred<AActionBase>(AActionBase::StaticClass(), SpawnTransform,
	                                                                         nullptr, nullptr,
	                                                                         ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		NewAction->EndDelay = InDelay;
		NewAction->FinishSpawning(SpawnTransform);
	}
}

int32 AActionManager::GetNextQueueIndex(int32 InCurrentIndex)
{
	return UKismetMathLibrary::Percent_IntInt(ActionQueue.Num(), (InCurrentIndex + 1));
}

void AActionManager::ProceedFromOngoingAction_Implementation(UObject* OngoingAction)
{
	if (OngoingAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
	{
		IInterface_CardAction::Execute_AttemptDestroyAction(OngoingAction);
		AttemptToPlayNextAction();
	}
	else
	{
		COS_SCREEN(TEXT("OnGoingAction는 InterfaceClass를 상속받지 못했습니다."));
	}
}
