#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Initiative.generated.h"


/* 
 * UStatus_Initiative 클래스는 이니셔티브(행동 순서) 시스템을 관리하는 상태 컴포넌트입니다.
 * 소유자가 이니셔티브에 따라 더 높은 우선순위를 가진 상태일수록 더 빠르게 행동할 수 있도록 설정됩니다.
 * 이 상태가 아닌 미니언은 이니셔티브 시스템에 포함되지 않습니다.
 * TurnManager의 설정이 완료되면 이벤트가 발생하며, 지연 로직을 통해 설정 완료를 기다립니다.
 */
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Initiative : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Initiative();

protected:
	virtual void BeginPlay() override;
};
