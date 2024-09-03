#include "Libraries/DelayHelper.h"

#include "Utilities/CosLog.h"


UDelayHelper::UDelayHelper(): LoopIndex(0)
{
}

void UDelayHelper::DelayWhile(TFunction<bool()> Condition, TFunction<void(int32)> OnLoop, TFunction<void()> OnComplete, float Delay)
{
	UWorld* World = GetWorldFromContextObject(this);
	if (!World)
	{
		COS_LOG_ERROR(TEXT("ExecuteLoop: 잘못된 세계입니다!"));
		return;
	}

	// 타이머가 이미 존재하면 삭제하여 중복 실행을 방지
	if (World->GetTimerManager().IsTimerActive(LoopTimerHandle))
	{
		World->GetTimerManager().ClearTimer(LoopTimerHandle);
	}

	LoopIndex = 0; // 루프 횟수 초기화

	// 지연 루프를 처리하도록 람다 함수 설정
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, Condition, OnComplete, OnLoop, World]()
	{
		if (Condition())
		{
			// 조건이 참이면 OnComplete 함수를 호출하고 지연 루프를 종료합니다.
			OnComplete();

			LoopIndex = 0;
			World->GetTimerManager().ClearTimer(LoopTimerHandle); // 타이머 종료
		}
		else
		{
			// 조건이 거짓이면 OnLoop 함수를 호출하고 반복 횟수를 증가시킵니다.
			OnLoop(LoopIndex);
			LoopIndex++;
		}
	});

	// 타이머 설정: 주어진 지연 시간 동안 루프 실행
	World->GetTimerManager().SetTimer(LoopTimerHandle, TimerDelegate, Delay, true);
}

void UDelayHelper::Reset()
{
	// LoopIndex를 초기화하고 타이머를 정리합니다.
	LoopIndex = 0;

	if (UWorld* World = GetWorldFromContextObject(this))
	{
		World->GetTimerManager().ClearTimer(LoopTimerHandle);
	}
}


UWorld* UDelayHelper::GetWorldFromContextObject(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		return World;
	}

	AActor* Actor = Cast<AActor>(WorldContextObject);
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}
