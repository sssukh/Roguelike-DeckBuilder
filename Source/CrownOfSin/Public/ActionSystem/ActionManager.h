#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardActionManager.h"
#include "Libraries/DelayHelper.h"
#include "ActionManager.generated.h"

/*액션 큐 처리를 담당합니다.
 *생성된 모든 BP_CardAction은 작업 대기열에 추가되고 작업 관리자는 이러한 작업을 순서대로 실행하여 더 이상 사용되지 않으면 제거합니다.
 *작업은 작업을 생성하는 게임플레이 로직과 비동기적으로 발생하므로 게임플레이 로직이 아닌 시각화에 사용되는 몇 가지 예외가 있습니다.*/
UCLASS()
class CROWNOFSIN_API AActionManager : public AActor, public IInterface_CardActionManager
{
	GENERATED_BODY()

public:
	AActionManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	/**
     * @brief 큐에 대기 중인 다음 액션을 시도하여 실행합니다. 
     *        지정된 조건이 충족될 때까지 반복적으로 확인하며, 조건이 충족되면 해당 액션을 실행합니다.
     * 
     * @details 이 함수는 큐에 대기 중인 액션을 실행하기 위해 호출됩니다. 
     *          조건이 충족되지 않으면 반복적인 체크를 수행하고, 조건이 충족되면 액션을 실행하며 큐 카운터를 업데이트합니다.
     */
	UFUNCTION(BlueprintCallable, Category="Action Manager Event")
	void AttemptToPlayNextAction();

	/**
     * @brief 주어진 액션을 큐에 추가합니다. 
     *        큐에 빈 자리가 있으면 해당 위치에 액션을 삽입하고, 그렇지 않으면 오류를 로그로 출력합니다.
     * 
     * @details 이 함수는 액션 큐에 새로운 액션을 추가하려고 시도합니다. 
     *          큐에 공간이 있으면 액션을 삽입하고, 큐가 가득 찼다면 "Action Overflow"라는 오류 메시지를 로그로 출력합니다.
     * 
     * @param Action 큐에 추가할 액션 객체입니다.
     */
	UFUNCTION(BlueprintCallable, Category="Action Manager Event")
	void QueueAction(UObject* Action);
	
	UFUNCTION(BlueprintCallable, Category="Action Manager Event")
	void QueueDelay(float InDelay);

	UFUNCTION(BlueprintPure, Category="Action Manager Event")
	int32 GetNextQueueIndex(int32 InCurrentIndex);


	/*========================================================================================
	*	Interface_CardActionManager
	=========================================================================================*/
public:
	virtual void ProceedFromOngoingAction_Implementation(UObject* OngoingAction) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Action Manager|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(BlueprintReadWrite, Category="Action Manager")
	int32 FillCounter = -1;

	UPROPERTY(BlueprintReadWrite, Category="Action Manager")
	int32 ActionsThisTick = 0;

	UPROPERTY(BlueprintReadWrite, Category="Action Manager")
	int32 MaxActionsPerTick = 5;

	UPROPERTY(BlueprintReadWrite, Category="Action Manager")
	int32 QueueCounter = -1;

	UPROPERTY(BlueprintReadWrite, Category="Action Manager")
	int32 QueueSize = 256;

	UPROPERTY(BlueprintReadWrite, Category="Action Manager")
	TArray<UObject*> ActionQueue;

private:
	UPROPERTY()
	UDelayHelper* DelayHelper_NextAction;
};
