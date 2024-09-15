#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Mana.generated.h"

/* 
 * UStatus_Mana 클래스는 카드 플레이어가 보유하는 마나 시스템을 관리하는 상태 컴포넌트입니다.
 * 이 마나는 카드의 플레이에 필요한 자원으로 사용되며, 카드 데이터의 UseRules에서 BP_UseRule_StatCost와 연동됩니다.
 * 마나 값이 변경될 때마다 전역 이벤트가 호출되어 관련 시스템에 반영됩니다.
 */
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Mana : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Mana();

	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
};
