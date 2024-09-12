#include "Libraries/DelayHelper.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	TimerDelegate.BindLambda([this, Condition, OnLoop, OnComplete]()
	{
		OnTick(Condition, OnLoop, OnComplete, LoopIndex);
	});

	if (Delay <= 0.0f)
	{
		Delay=0.017f;
	}

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

void UDelayHelper::OnTick(const TFunction<bool()>& Condition, const TFunction<void(int32 Index)>& LoopBody, const TFunction<void()>& Completed, int32& OutLoopIndex)
{
	if (Condition())
	{
		Completed();
		OutLoopIndex = 0;
		GetWorld()->GetTimerManager().ClearTimer(LoopTimerHandle);
	}
	else
	{
		LoopBody(OutLoopIndex);
		OutLoopIndex++;
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
