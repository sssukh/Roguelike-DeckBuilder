#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileDestroyComponent.generated.h"

/**
 * UPileDestroyComponent 클래스는 카드 게임에서 파괴된 카드들을 관리하는 컴포넌트입니다.
 * 이 클래스는 일정 시간 후에 카드 더미에 있는 모든 카드를 파괴하고, 더미를 비우는 역할을 수행합니다.
 * 
 * 주요 기능:
 * - Tick 이벤트를 통해 주기적으로 더미에 있는 카드 상태를 확인합니다.
 * - 더미에 카드가 있을 경우, 일정 시간이 지나면 카드를 파괴하고 더미를 비웁니다.
 * - 카드를 파괴한 후, 지연 시간이 지나면 다시 카드를 파괴할 수 있도록 상태를 초기화합니다.
 * 
 * 이 클래스는 파괴된 카드가 더 이상 필요하지 않을 때 안전하게 제거하는 역할을 하며, 
 * 게임 내 카드의 소멸을 처리하는 데 사용됩니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileDestroyComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileDestroyComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Pile Destroy Component")
	float ResetDelay = 4.0f;

private:
	bool bDoOnce = true;
};
