#include "Libraries/DelayHelper.h"
#include "Utilities/CosLog.h"


UDelayHelper::UDelayHelper(): Interval(0), LoopIndex(0), CachedWorld(nullptr)
{
}

void UDelayHelper::DelayWhile(const FConditionDelegate& Condition, const FOnLoopDelegate& OnLoop, const FOnCompleteDelegate& OnComplete, float CheckInterval, bool bAuToDestroy)
{
	CachedWorld = GetWorld();
	if (!CachedWorld)
	{
		COS_LOG_SCREEN_ERROR(TEXT("DelayWhile: 유효한 UWorld를 가져올 수 없습니다!"));
		return;
	}

	// 델리게이트를 멤버 변수에 바인딩
	ConditionDelegate = Condition;
	OnLoopDelegate = OnLoop;
	OnCompleteDelegate = OnComplete;

	// 루프 횟수 초기화
	LoopIndex = 0;
	bLoopClear = bAuToDestroy;

	// 최소 지연 시간 설정
	Interval = (CheckInterval <= 0.0f) ? 0.017f : CheckInterval;

	// 타이머 설정
	CachedWorld->GetTimerManager().SetTimer(LoopTimerHandle, this, &UDelayHelper::OnTick, Interval, true);
}

void UDelayHelper::Reset()
{
	// LoopIndex를 초기화하고 타이머를 정리합니다.
	LoopIndex = 0;

	if (GetWorld()->GetTimerManager().IsTimerActive(LoopTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LoopTimerHandle);
	}

	// 델리게이트 초기화
	ConditionDelegate.Unbind();
	OnLoopDelegate.Unbind();
	OnCompleteDelegate.Unbind();
}

void UDelayHelper::OnTick()
{
	if (ConditionDelegate.IsBound() && ConditionDelegate.Execute())
	{
		if (OnLoopDelegate.IsBound())
		{
			OnLoopDelegate.Execute(LoopIndex);
		}
		LoopIndex++;
	}
	else
	{
		if (OnCompleteDelegate.IsBound())
		{
			OnCompleteDelegate.Execute();
		}

		// 타이머 정리 및 상태 초기화
		if (bLoopClear)
		{
			Reset();
		}
	}
}

UWorld* UDelayHelper::GetWorld() const
{
	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
	return nullptr;
}
