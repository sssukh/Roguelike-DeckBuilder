#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "TimerManager.h"

template <typename ElementType>
class FDelayedForEachLoopHelper
{
public:
    FDelayedForEachLoopHelper(UWorld* InWorld, const TArray<ElementType>& InArray, int32 InLoopsPerTick, float InDelay,
                              TFunction<void(ElementType, int32)> InLoopBody, TFunction<void()> InCompleted)
        : World(InWorld)
        , Array(InArray)
        , LoopsPerTick(InLoopsPerTick)
        , Delay(InDelay)
        , LoopBody(MoveTemp(InLoopBody))
        , Completed(MoveTemp(InCompleted))
        , CurrentIndex(0)
        , LoopCounter(0)
    {
        StartLoop();
    }

    void StartLoop()
    {
        ProcessNext();
    }

    void ProcessNext()
    {
        if (CurrentIndex >= Array.Num())
        {
            // 루프 완료
            if (Completed)
            {
                Completed();
            }
            // 헬퍼 객체를 삭제합니다.
            delete this;
            return;
        }

        // 현재 요소 처리
        LoopBody(Array[CurrentIndex], CurrentIndex);

        // 인덱스 및 루프 카운터 증가
        CurrentIndex++;
        LoopCounter++;

        // 지연 적용 여부 결정
        if (LoopsPerTick > 0 && LoopCounter % LoopsPerTick == 0 && Delay > 0.0f)
        {
            // 지연 후 다음 요소 처리
            if (World)
            {
                World->GetTimerManager().SetTimer(TimerHandle, [this]()
                {
                    ProcessNext();
                }, Delay, false);
            }
            else
            {
                // 월드가 없으면 즉시 다음 요소 처리
                ProcessNext();
            }
        }
        else
        {
            // 지연 없이 즉시 다음 요소 처리
            ProcessNext();
        }
    }

private:
    UWorld* World;

    TArray<ElementType> Array;

    int32 LoopsPerTick;

    float Delay;

    TFunction<void(ElementType, int32)> LoopBody;

    TFunction<void()> Completed;

    int32 CurrentIndex;

    int32 LoopCounter;

    FTimerHandle TimerHandle;
};
