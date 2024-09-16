#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardAction.h"
#include "ActionBase.generated.h"


/* Action Manager가 대기열에 추가한 작업을 재생하는 역할을 담당합니다.
 * 액션을 생성하면 자동으로 액션 대기열에 추가되어 이전에 대기열에 추가된 액션의 재생이 완료되는 즉시 해당 액션이 재생됩니다.
 * 액션은 일반적으로 애니메이션과 게임플레이 로직과 관련이 없는 것들에 사용되어야 합니다.
 */
UCLASS()
class CROWNOFSIN_API AActionBase : public AActor ,public IInterface_CardAction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	/*
	 * 하위 청사진을 재정의하는 데 사용됩니다. 액션이 생성되면 즉시 설정 코드를 실행합니다.
	 * 액션이 대기열에 추가될 때와 실행될 때 사이에 소멸될 수 있는 액터의 변수에 액세스하려는 경우 유용합니다.
	 * 그렇다면 여기에서 액세스하여 액션을 플레이할 때 사용할 변수에 저장할 수 있습니다.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Action Event")
	void SetUpAction();
	
	/*
	 * 하위 블루프린트에서 재정의되었습니다.
	 * 여기에서 발생하는 작업을 표시하려는 모든 동작을 실행합니다.
	 * PlayAction은 일반적으로 게임플레이 관련 코드를 포함하지 않아야 하며 플레이어에게 사물을 시각화하는 데에만 사용되어야 합니다.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Action Event")
	void PlayAction();


	/*========================================================================================
	*	IInterface_CardAction
	=========================================================================================*/
public:
	virtual void AnimateAction_Implementation(UObject* CallingActionManager) override;
	
	virtual void EndAction_Implementation() override;

	virtual bool AttemptDestroyAction_Implementation() override;
	


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Base|Component")
	USceneComponent* DefaultSceneRoot;
	
	/*PlayAction 종료와 다음 작업을 재생하려는 시점 사이의 초 단위 지연입니다. 값이 0이면 지연 시간이 표시됩니다. 0 미만의 지연은 지연을 제공하지 않습니다.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Base", meta=(ExposeOnSpawn="true"))
	float EndDelay = -1.0f;

	UPROPERTY(BlueprintReadWrite, Category="Action Base")
	UObject* ActionManagerRef;
};
