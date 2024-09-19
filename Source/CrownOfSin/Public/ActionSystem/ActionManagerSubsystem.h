#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "ActionManagerSubsystem.generated.h"

/**
 * 액션 매니저 서브시스템입니다.
 * 월드 수준에서 액션을 관리합니다.
 */
UCLASS()
class CROWNOFSIN_API UActionManagerSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UActionManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	// FTickableGameObject 인터페이스 구현
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

public:
	/** 액션을 큐에 추가합니다 */
	UFUNCTION(BlueprintCallable, Category = "Action Manager SubSystem")
	void QueueAction(UObject* Action);

	/** 딜레이를 큐에 추가합니다 */
	UFUNCTION(BlueprintCallable, Category = "Action Manager SubSystem")
	void QueueDelay(float InDelay);

	/** 진행 중인 액션으로부터 호출됩니다 */
	void ProceedFromOngoingAction(UObject* OngoingAction);

	/** 다음 액션을 실행합니다 */
	UFUNCTION(BlueprintCallable, Category="Action Manager SubSystem")
	void AttemptToPlayNextAction();

private:
	/** 액션 실행 조건을 확인합니다 */
	bool ShouldPlayNextAction() const;

public:
	template <typename T>
	T* CreateAndQueueAction(TFunction<void(T*)> InitFunction,
	                        ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
	                        ESpawnActorScaleMethod ScaleMethod = ESpawnActorScaleMethod::OverrideRootScale)
	{
		if (UWorld* World = GetWorld())
		{
			FTransform SpawnTransform = FTransform::Identity;

			// 지정된 클래스와 스폰 파라미터를 사용하여 액터를 스폰 (지연 방식)
			if (T* NewAction = World->SpawnActorDeferred<T>(T::StaticClass(),
			                                                SpawnTransform,
			                                                nullptr,
			                                                nullptr,
			                                                CollisionHandlingMethod,
			                                                ScaleMethod))
			{
				// InitFunction을 사용하여 액션의 변수를 설정
				InitFunction(NewAction);

				// 액터 스포닝 완료
				NewAction->FinishSpawning(SpawnTransform);

				// 액션을 큐에 추가
				QueueAction(NewAction);

				return NewAction;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* CreateAndQueueActionWithClass(UClass* ActionClass,
	                                 TFunction<void(T*)> InitFunction,
	                                 ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
	                                 ESpawnActorScaleMethod ScaleMethod = ESpawnActorScaleMethod::OverrideRootScale)
	{
		if (UWorld* World = GetWorld())
		{
			FTransform SpawnTransform = FTransform::Identity;

			// 지정된 클래스를 사용하여 액터를 스폰 (지연 방식)
			if (T* NewAction = Cast<T>(World->SpawnActorDeferred<AActor>(ActionClass,
			                                                             SpawnTransform,
			                                                             nullptr,
			                                                             nullptr,
			                                                             CollisionHandlingMethod,
			                                                             ScaleMethod)))
			{
				// InitFunction을 사용하여 액션의 변수를 설정
				InitFunction(NewAction);

				// 액터 스포닝 완료
				NewAction->FinishSpawning(SpawnTransform);

				// 액션을 큐에 추가
				QueueAction(NewAction);

				return NewAction;
			}
		}
		return nullptr;
	}


	//사용 예시
	// UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	// AAction_ModifyStatus* NewAction = ActionManagerSubsystem->CreateAndQueueAction<AAction_ModifyStatus>([](AAction_ModifyStatus* Action_ModifyStatus)
	// {
	// 	Action_ModifyStatus->NewValue = 0;
	// 	Action_ModifyStatus->bShowSplashIcon = false;
	// 	Action_ModifyStatus->bShowSplashNumber = false;
	// 	Action_ModifyStatus->bRefreshAppearance = true;
	// 	Action_ModifyStatus->TextOverride = FText::FromString(TEXT("HideReward"));
	// 	Action_ModifyStatus->bCanBeZero = false;
	// 	Action_ModifyStatus->EndDelay = -1.0f;
	// });

private:
	/** 액션 진행 상태 */
	UPROPERTY(BlueprintReadWrite, Category="Action Manager SubSystem", meta=(AllowPrivateAccess="true"))
	bool bIsActionInProgress;

	/** 현재 틱에서 실행된 액션 수 */
	UPROPERTY(BlueprintReadWrite, Category="Action Manager SubSystem", meta=(AllowPrivateAccess="true"))
	int32 ActionsThisTick;

	/** 최대 액션 실행 횟수 */
	UPROPERTY(BlueprintReadWrite, Category="Action Manager SubSystem", meta=(AllowPrivateAccess="true"))
	int32 MaxActionsPerTick;

	/** 현재 실행 중인 액션 */
	UPROPERTY(BlueprintReadWrite, Category="Action Manager SubSystem", meta=(AllowPrivateAccess="true"))
	UObject* CurrentAction;

	/** 액션 큐 */
	TQueue<UObject*> ActionQueue;

	/** 액션 실행을 지연시키기 위한 타이머 */
	UPROPERTY(BlueprintReadWrite, Category="Action Manager SubSystem", meta=(AllowPrivateAccess="true"))
	FTimerHandle ActionTimerHandle;
};
