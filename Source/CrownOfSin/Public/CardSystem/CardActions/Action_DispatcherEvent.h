
#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Core/CosEnumStruct.h"
#include "Action_DispatcherEvent.generated.h"

class UDispatcherHubComponent;

/*ToDo 구현해야합니다*/
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
	UDispatcherHubComponent* DispatcherHub;
	
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
