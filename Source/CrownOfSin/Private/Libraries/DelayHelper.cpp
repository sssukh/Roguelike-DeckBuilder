#include "Libraries/DelayHelper.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CosLog.h"


UDelayHelper::UDelayHelper(): LoopIndex(0), CachedWorld(nullptr), Interval(0)
{
}

void UDelayHelper::DelayWhile(TFunction<bool()> Condition, TFunction<void(int32)> OnLoop, TFunction<void()> OnComplete, float CheckInterval)
{
	CachedWorld = GetWorld();
	if (!CachedWorld)
	{
		COS_LOG_ERROR(TEXT("DelayWhile: 유효한 UWorld를 가져올 수 없습니다!"));
		return;
	}

	// 함수 포인터를 멤버 변수로 저장
	ConditionFunc = Condition;
	OnLoopFunc = OnLoop;
	OnCompleteFunc = OnComplete;

	// 최소 지연 시간 설정
	Interval = (CheckInterval <= 0.0f) ? 0.017f : CheckInterval;

	// 타이머가 이미 존재하면 삭제하여 중복 실행을 방지
	if (CachedWorld->GetTimerManager().IsTimerActive(LoopTimerHandle))
	{
		CachedWorld->GetTimerManager().ClearTimer(LoopTimerHandle);
	}

	LoopIndex = 0; // 루프 횟수 초기화

	// 루프 타이머 설정
	CachedWorld->GetTimerManager().SetTimer(LoopTimerHandle, this, &UDelayHelper::OnTick, Interval, true);

}

void UDelayHelper::Reset()
{
	// LoopIndex를 초기화하고 타이머를 정리합니다.
	LoopIndex = 0;

	if (CachedWorld)
	{
		CachedWorld->GetTimerManager().ClearTimer(LoopTimerHandle);
	}

	// 함수 포인터 초기화
	ConditionFunc = nullptr;
	OnLoopFunc = nullptr;
	OnCompleteFunc = nullptr;
}

void UDelayHelper::OnTick()
{
	if (ConditionFunc && ConditionFunc())
	{
		if (OnCompleteFunc)
		{
			OnCompleteFunc();
		}
		LoopIndex = 0;
		if (CachedWorld)
		{
			CachedWorld->GetTimerManager().ClearTimer(LoopTimerHandle);
		}
	}
	else
	{
		if (OnLoopFunc)
		{
			OnLoopFunc(LoopIndex);
		}
		LoopIndex++;
		// 타이머는 루프 중이므로 추가적인 타이머 설정이 필요하지 않습니다.
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
