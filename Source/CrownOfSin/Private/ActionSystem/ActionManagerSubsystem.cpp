#include "ActionSystem/ActionManagerSubsystem.h"
#include "Interfaces/Interface_CardAction.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"
#include "ActionSystem/Action_Delay.h"

UActionManagerSubsystem::UActionManagerSubsystem() : CurrentAction(nullptr), bIsActionInProgress(false), ActionsThisTick(0), MaxActionsPerTick(5)
{
}

void UActionManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UActionManagerSubsystem::Deinitialize()
{
	// 액션 큐 정리
	while (!ActionQueue.IsEmpty())
	{
		UObject* RemainingAction = nullptr;
		if (ActionQueue.Dequeue(RemainingAction) && IsValid(RemainingAction))
		{
			if (RemainingAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
			{
				IInterface_CardAction::Execute_AttemptDestroyAction(RemainingAction);
			}
		}
	}

	// 현재 진행 중인 액션 정리
	if (IsValid(CurrentAction))
	{
		if (CurrentAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
		{
			IInterface_CardAction::Execute_AttemptDestroyAction(CurrentAction);
		}

		CurrentAction = nullptr;
		bIsActionInProgress = false;
	}

	Super::Deinitialize();
}

void UActionManagerSubsystem::Tick(float DeltaTime)
{
	ActionsThisTick = 0;
}

void UActionManagerSubsystem::QueueAction(UObject* Action)
{
	if (IsValid(Action))
	{
		ActionQueue.Enqueue(Action);

		// 현재 액션이 진행 중이 아니면 즉시 시도
		if (!bIsActionInProgress)
		{
			AttemptToPlayNextAction();
		}
	}
}

void UActionManagerSubsystem::QueueDelay(float InDelay)
{
	// 딜레이 액션 생성 및 큐에 추가
	FTransform SpawnTransform = FTransform::Identity;
	if (AAction_Delay* NewAction = GetWorld()->SpawnActorDeferred<AAction_Delay>(AAction_Delay::StaticClass(),
	                                                                             SpawnTransform,
	                                                                             nullptr, nullptr,
	                                                                             ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		NewAction->EndDelay = InDelay;
		NewAction->FinishSpawning(SpawnTransform);
	}
}

void UActionManagerSubsystem::AttemptToPlayNextAction()
{
	// 현재 액션이 진행 중이면 대기
	if (bIsActionInProgress) return;

	// 액션 실행 조건을 확인합니다.
	if (ShouldPlayNextAction())
	{
		if (ActionQueue.Dequeue(CurrentAction))
		{
			if (CurrentAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
			{
				bIsActionInProgress = true;
				ActionsThisTick++;
				IInterface_CardAction::Execute_AnimateAction(CurrentAction, this);
			}
			else
			{
				COS_SCREEN(TEXT("액션이 IInterface_CardAction을 구현하지 않았습니다."));
				CurrentAction = nullptr;
				bIsActionInProgress = false;
				AttemptToPlayNextAction();
			}
		}
	}
	else
	{
		// 조건이 충족되지 않으면 타이머로 재시도
		GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &UActionManagerSubsystem::AttemptToPlayNextAction, 0.17f, false);
	}
}

bool UActionManagerSubsystem::ShouldPlayNextAction() const
{
	// 현재 틱에서 실행된 액션 수가 최대치를 넘지 않았는지 확인하고, 액션 큐에 액션이 있는지 확인
	return ActionsThisTick < MaxActionsPerTick && !ActionQueue.IsEmpty();
}

void UActionManagerSubsystem::ProceedFromOngoingAction(UObject* OngoingAction)
{
	if (OngoingAction == CurrentAction)
	{
		if (OngoingAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
		{
			IInterface_CardAction::Execute_AttemptDestroyAction(OngoingAction);
			CurrentAction = nullptr;
			bIsActionInProgress = false;

			// 다음 액션 시도
			AttemptToPlayNextAction();
		}
		else
		{
			COS_SCREEN(TEXT("OngoingAction이 IInterface_CardAction을 구현하지 않았습니다."));
		}
	}
	else
	{
		COS_SCREEN(TEXT("OngoingAction이 CurrentAction과 일치하지 않습니다."));
	}
}

TStatId UActionManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UActionManagerSubsystem, STATGROUP_Tickables);
}
