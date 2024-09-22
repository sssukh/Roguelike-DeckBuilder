#include "ActionSystem/ActionManagerSubsystem.h"
#include "Interfaces/Interface_CardAction.h"
#include "ActionSystem/Action_Delay.h"
#include "Libraries/DelayHelper.h"

UActionManagerSubsystem::UActionManagerSubsystem() : ActionsThisTick(0), MaxActionsPerTick(5), CurrentAction(nullptr), PlayNextActionDelayHelper(nullptr)
{
}

void UActionManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UActionManagerSubsystem::Deinitialize()
{
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

		if (!bIsActionInProgress)
		{
			AttemptToPlayNextAction();
		}
	}
}

void UActionManagerSubsystem::QueueDelay(float InDelay)
{
	CreateAndQueueAction<AAction_Delay>([InDelay](AAction_Delay* Action_Delay)
	{
		Action_Delay->EndDelay = InDelay;
	});
}

void UActionManagerSubsystem::AttemptToPlayNextAction()
{
	if (bIsActionInProgress) return;

	if (ActionQueue.IsEmpty())
	{
		PlayNextActionDelayHelper->Reset();
		return;
	}

	if (!PlayNextActionDelayHelper)
	{
		PlayNextActionDelayHelper = NewObject<UDelayHelper>(this);
	}

	// 기존 타이머를 리셋
	PlayNextActionDelayHelper->Reset();

	// 델리게이트 생성 및 바인딩
	FConditionDelegate ConditionDelegate;
	ConditionDelegate.BindUObject(this, &UActionManagerSubsystem::OnShouldProceedToNextAction);

	FOnLoopDelegate OnLoopDelegate;
	OnLoopDelegate.BindUObject(this, &UActionManagerSubsystem::OnAttemptLoopProgress);

	FOnCompleteDelegate OnCompleteDelegate;
	OnCompleteDelegate.BindUObject(this, &UActionManagerSubsystem::OnNextPlayAction);

	// DelayWhile 실행
	PlayNextActionDelayHelper->DelayWhile(ConditionDelegate, OnLoopDelegate, OnCompleteDelegate, 0.0f, false);
}

int32 UActionManagerSubsystem::GetCurrentIndex()
{
	if (ActionQueue.IsEmpty())
		CurrentSpawnIndex = 0;

	return CurrentSpawnIndex++;
}

bool UActionManagerSubsystem::OnShouldProceedToNextAction()
{
	return ActionsThisTick >= MaxActionsPerTick || IsValid(CurrentAction);
}

void UActionManagerSubsystem::OnAttemptLoopProgress(int32 Index)
{
}

void UActionManagerSubsystem::OnNextPlayAction()
{
	if (ActionQueue.Dequeue(CurrentAction))
	{
		if (CurrentAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
		{
			bIsActionInProgress = true;
			ActionsThisTick++;
			IInterface_CardAction::Execute_AnimateAction(CurrentAction, this);
		}
	}
}

void UActionManagerSubsystem::ProceedFromOngoingAction(UObject* OngoingAction)
{
	if (OngoingAction == CurrentAction)
	{
		if (OngoingAction->GetClass()->ImplementsInterface(UInterface_CardAction::StaticClass()))
		{
			IInterface_CardAction::Execute_AttemptDestroyAction(OngoingAction);
		}

		CurrentAction = nullptr;
		bIsActionInProgress = false;
		AttemptToPlayNextAction();
	}
}

TStatId UActionManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UActionManagerSubsystem, STATGROUP_Tickables);
}
