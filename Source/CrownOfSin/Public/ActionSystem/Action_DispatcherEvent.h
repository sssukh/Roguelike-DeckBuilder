
#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Core/CosEnumStruct.h"
#include "Action_DispatcherEvent.generated.h"

class UDispatcherHubComponent;

/*조치 대기열의 일부로 호출되도록 설정된 Dispatcher Hub에서 이벤트를 호출하는 경우.
일반적으로 뽑혀지는 카드를 보여주는 것과 같이 순전히 시각적인 이벤트를 호출하는 데 사용됩니다(액션 내에서 처리하면 안 되는 핸드 파일의 카드 배열에 추가되는 카드의 게임플레이 로직과 반대).*/
UCLASS()
class CROWNOFSIN_API AAction_DispatcherEvent : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_DispatcherEvent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayAction_Implementation() override;
	

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	UDispatcherHubComponent* DispatcherHubReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	FGameplayTag Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	UObject* CallingObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	UObject* CallSpecificObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	ECallGlobal AlsoCallGlobal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	UObject* PayLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Dispatcher", meta=(ExposeOnSpawn="true"))
	FGameplayTagContainer CallTags;

	
};
