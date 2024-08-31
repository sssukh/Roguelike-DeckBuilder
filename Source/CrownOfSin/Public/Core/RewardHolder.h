#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "RewardHolder.generated.h"

class UDispatcherHubComponent;

/*RewardScreen에 표시될 보상을 보유합니다. CardPlayer와 마찬가지로 이 액터에 상태를 추가할 수 있습니다.*/
UCLASS()
class CROWNOFSIN_API ARewardHolder : public AActor, public IInterface_EventHolder, public IInterface_CardTarget
{
	GENERATED_BODY()

public:
	ARewardHolder();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/*========================================================================================
	*	Interface_Event Holder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reward Holder|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reward Holder|Component")
	UDispatcherHubComponent* DispatcherHubComponentLocal;
};
