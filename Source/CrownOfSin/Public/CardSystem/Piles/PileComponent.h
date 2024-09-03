#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PileComponent.generated.h"


class ACardBase;


/*데크와 카드 패를 포함한 모든 카드 더미의 상위 클래스입니다. 더미에 있는 모든 카드에 대한 참조와 더미에서 카드를 추가하고 제거하는 방법을 보유합니다.*/
UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/*========================================================================================
	*	FiledMembers
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	FGameplayTag PileTag;

	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	TArray<ACardBase*> Cards;
};
