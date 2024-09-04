#include "Libraries/RetriggerAbleDelay.h"

URetriggerAbleDelay::URetriggerAbleDelay(): DelayDuration(0)
{
}

void URetriggerAbleDelay::RetriggerDelay(float Duration, TFunction<void()> InCompletionCallback)
{
	DelayDuration = Duration;
	CompletionCallback = InCompletionCallback;

	// 기존 타이머를 초기화하고 새로 시작
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
		StartDelay(DelayDuration);
	}
}

void URetriggerAbleDelay::StartDelay(float Duration)
{
	if (GetWorld())
	{
		if (Duration <= 0.0f)
		{
			// Duration이 0.0이거나 0보다 작은 경우, 다음 틱에서 CompletionCallback 호출
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				if (CompletionCallback)
				{
					CompletionCallback();
				}

				OnDelayCompleted();

				GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
			});
		}
		else
		{
			// 타이머를 시작하여 일정 시간 후 CompletionCallback 호출
			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [this]()
			{
				if (CompletionCallback)
				{
					CompletionCallback();
				}

				OnDelayCompleted();

				GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
			}, Duration, false);
		}
	}
}

void URetriggerAbleDelay::OnDelayCompleted()
{
	if (OnDelayCompletedDelegate.IsBound())
		OnDelayCompletedDelegate.Broadcast();
}
