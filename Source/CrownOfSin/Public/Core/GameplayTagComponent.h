
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameplayTagComponent.generated.h"


/*다양한 액터 클래스의 게임플레이 태그에 액세스하는 일반적인 방법을 활성화하기 위해 GameplayTag를 보유하는 구성 요소입니다.*/
UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UGameplayTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameplayTagComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gameplay Tag",meta=(ExposeOnSpawn="true"))
	FGameplayTagContainer GameplayTags;
};
