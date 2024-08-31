
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalDispatcherHub.generated.h"

class UDispatcherHubComponent;

/*DispatcherHubComponent를 보유하는 빈 액터입니다. 모든 이벤트가 바인딩되거나 호출될 수 있어야 하는 이벤트에 대해 전역적으로 액세스할 수 있도록 존재합니다.*/
UCLASS()
class CROWNOFSIN_API AGlobalDispatcherHub : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGlobalDispatcherHub();

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
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Global Dispatcher Hub|Component")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Global Dispatcher Hub|Component")
	UDispatcherHubComponent* DispatcherHubComponent;
};
