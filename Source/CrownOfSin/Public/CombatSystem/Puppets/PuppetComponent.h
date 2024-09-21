#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PuppetComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnimationEvent, AActor*, InPuppet, FGameplayTag, InAnimationTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnimationEnd, AActor*, InPuppet, FGameplayTag, InAnimationTag);


/*애니메이션 설정과 함께 작동하기 위해 퍼펫에 추가할 구성 요소*/
UCLASS(ClassGroup=("COS|Core"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPuppetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPuppetComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Puppet Component|Delegate")
	FOnAnimationEvent OnAnimationEvent;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Puppet Component|Delegate")
	FOnAnimationEnd OnAnimationEnd;
};
