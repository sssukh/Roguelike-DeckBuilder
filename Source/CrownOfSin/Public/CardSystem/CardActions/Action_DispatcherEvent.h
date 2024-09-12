
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
	// Sets default values for this actor's properties
	AAction_DispatcherEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
