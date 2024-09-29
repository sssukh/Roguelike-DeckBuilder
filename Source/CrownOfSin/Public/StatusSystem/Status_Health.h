
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Health.generated.h"

/*0에 도달하면 소유자가 사망합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Health : public UStatusComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Health();

	/**
	 * 상태 컴포넌트에 의해 체력 값을 변경합니다. 체력이 감소할 때 특정 이벤트를 발생시키고, 체력이 0 이하로 떨어지면 해당 액터를 게임에서 제거합니다.
	 * 
	 * @param InAmount 체력의 변경량
	 * @param bShowSplashNumber 피해량 표시 여부
	 * @param bShowSplashIcon 아이콘 표시 여부
	 * @param bRefreshAppearance 외관을 새로 고칠지 여부
	 * @param InPayload 추가 데이터
	 * @return 변경 후의 체력 값
	 */	
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
	
};
