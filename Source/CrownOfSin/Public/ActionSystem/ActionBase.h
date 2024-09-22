#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardAction.h"
#include "ActionBase.generated.h"


/**
 * Action Manager가 대기열에 추가한 작업을 재생하는 역할을 담당합니다.
 * 액션을 생성하면 자동으로 액션 대기열에 추가되어 이전에 대기열에 추가된 액션의 재생이 완료되는 즉시 해당 액션이 재생됩니다.
 * 액션은 일반적으로 애니메이션과 게임플레이 로직과 관련이 없는 것들에 사용되어야 합니다.
 */
UCLASS()
class CROWNOFSIN_API AActionBase : public AActor, public IInterface_CardAction
{
	GENERATED_BODY()

public:
	AActionBase();

protected:
	virtual void BeginPlay() override;

public:
	/*========================================================================================
	*	IInterface_CardAction
	=========================================================================================*/
public:
	virtual void SetUpAction_Implementation() override;

	virtual void PlayAction_Implementation() override;

	virtual void EndAction_Implementation() override;

	virtual void AnimateAction_Implementation(UObject* CallingActionManager) override;

	virtual bool AttemptDestroyAction_Implementation() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Base|Component")
	USceneComponent* DefaultSceneRoot;

public:
	/*PlayAction 종료와 다음 작업을 재생하려는 시점 사이의 초 단위 지연입니다.
	 *값이 0이면 지연 시간이 표시됩니다.
	 *0 미만의 지연은 지연을 제공하지 않습니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Base", meta=(ExposeOnSpawn="true"))
	float EndDelay = -1.0f;

private:
	FTimerHandle EndDelayTimerHandle;
};
