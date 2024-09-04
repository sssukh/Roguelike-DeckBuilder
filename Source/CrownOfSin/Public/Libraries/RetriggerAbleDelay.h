// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RetriggerAbleDelay.generated.h"

// 델리게이트 선언 (필요한 경우)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelayCompletedDelegate);

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API URetriggerAbleDelay : public UObject
{
	GENERATED_BODY()

public:
	URetriggerAbleDelay();

public:
	// 딜레이를 재설정하는 함수
	void RetriggerDelay(float Duration, TFunction<void()> InCompletionCallback);


private:
	// 딜레이를 시작하는 함수
	void StartDelay(float Duration);

	// 딜레이 완료 후 호출될 함수
	void OnDelayCompleted();

private:
	// 딜레이 시간 (초)
	float DelayDuration;

	// 람다식 또는 함수 포인터로 저장된 완료 콜백
	TFunction<void()> CompletionCallback;
	
	// 타이머 핸들
	FTimerHandle DelayTimerHandle;

public:
	// 딜레이 완료 이벤트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Retriggerable Delay")
	FDelayCompletedDelegate OnDelayCompletedDelegate;
};
