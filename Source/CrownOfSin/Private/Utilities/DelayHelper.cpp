#include "Utilities/DelayHelper.h"

#include "Libraries/CosLog.h"


UDelayHelper::UDelayHelper()
{
}

void UDelayHelper::DelayWhile(TFunction<bool()> IsConditionMet, TFunction<void()> OnDelayFinished, float DelayTime/*=0.0f*/)
{
	const UWorld* World = GetWorldFromContextObject(this);
	if (!World)
	{
		COS_LOG_ERROR(TEXT("DelayWhile: 잘못된 세계입니다!"));
		return;
	}

	// 타이머가 이미 존재하면 삭제하여 중복 실행을 방지
	World->GetTimerManager().ClearTimer(TimerHandle);

	// 지연 루프를 처리하도록 람다 함수 설정
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, IsConditionMet, OnDelayFinished, World]()
	{
		if (IsConditionMet())
		{
			// 조건이 참이면 OnDelayFinished 함수를 호출하고 지연 루프를 종료합니다.
			OnDelayFinished();
			World->GetTimerManager().ClearTimer(TimerHandle); // 타이머 종료
		}
	});

	// 타이머 설정: 주어진 지연 시간 동안 루프 실행
	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayTime, true);
}

void UDelayHelper::WhileLoopDelayed(TFunction<bool()> Condition, float Delay, TFunction<void()> LoopBody, TFunction<void()> Completed,
                                    int32& OutLoopIndex)
{
	const UWorld* World = GetWorldFromContextObject(this);
	if (!World)
	{
		COS_LOG_ERROR(TEXT("LoopWithDelay: 유효하지 않은 세계!"));
		return;
	}

	// 타이머가 이미 존재하면 삭제하여 중복 실행을 방지
	World->GetTimerManager().ClearTimer(TimerHandle);

	OutLoopIndex = 0;

	// 지연 루프를 처리하도록 람다 함수 설정
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, Condition, LoopBody, Completed, &OutLoopIndex]()
	{
		OnTick(Condition, LoopBody, Completed, OutLoopIndex);
	});

	// 타이머 설정: 주어진 지연 시간 동안 루프 실행
	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay, true, -1.f);
}

void UDelayHelper::OnTick(TFunction<bool()> Condition, TFunction<void()> LoopBody, TFunction<void()> Completed, int32& OutLoopIndex)
{
	if (Condition())
	{
		LoopBody();
		OutLoopIndex++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Completed();
	}
}

UWorld* UDelayHelper::GetWorldFromContextObject(UObject* WorldContextObject) const
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	if (UWorld* World = WorldContextObject->GetWorld())
	{
		return World;
	}

	if (AActor* Actor = Cast<AActor>(WorldContextObject))
	{
		return Actor->GetWorld();
	}

	return nullptr;
}
