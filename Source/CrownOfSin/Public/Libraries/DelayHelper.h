﻿#pragma once

#include "CoreMinimal.h"
#include "DelayHelper.generated.h"


/**
 * 반복적인 조건 확인 및 함수 실행을 위한 헬퍼 클래스
 */
UCLASS()
class CROWNOFSIN_API UDelayHelper : public UObject
{
	GENERATED_BODY()

public:
	UDelayHelper();

	/**
	 * @brief 지정된 조건이 충족될 때까지 주기적으로 조건을 확인하고 함수를 실행합니다.
	 * @param Condition 확인할 조건 함수입니다. true를 반환하면 OnComplete 함수가 실행됩니다.
	 * @param OnLoop 조건 함수가 false를 반환할 때마다 호출할 반복 함수입니다. 현재 루프 인덱스를 매개변수로 받습니다.
	 * @param OnComplete 조건 함수가 true를 반환하면 호출할 함수입니다.
	 * @param CheckInterval 조건 함수 확인 사이의 지연 시간(초)입니다.
	 */
	void DelayWhile(TFunction<bool()> Condition, TFunction<void(int32)> OnLoop, TFunction<void()> OnComplete, float CheckInterval);

	/**
	 * @brief DelayHelper의 상태를 초기화하고 타이머를 정리합니다.
	 */
	void Reset();

protected:
	void OnTick();

	/** UObject에서 UWorld를 가져오는 함수 오버라이드 */
	virtual UWorld* GetWorld() const override;

private:
	FTimerHandle LoopTimerHandle; // 단일 타이머 핸들 사용

	int32 LoopIndex; // 반복 횟수 추적을 위한 멤버 변수

	UPROPERTY()
	UWorld* CachedWorld; // UWorld 캐싱

	// 함수 포인터를 멤버 변수로 저장
	TFunction<bool()> ConditionFunc;

	TFunction<void(int32)> OnLoopFunc;

	TFunction<void()> OnCompleteFunc;

	float Interval;
};
