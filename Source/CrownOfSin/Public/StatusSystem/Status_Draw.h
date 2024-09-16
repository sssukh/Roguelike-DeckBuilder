
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Draw.generated.h"

class UPileHandComponent;

/* 
 * UStatus_Draw 클래스는 카드 플레이어의 손에 새로운 카드를 추가하는 역할을 담당하는 상태 컴포넌트입니다.
 * 이 클래스는 동일한 소유자(주로 카드 플레이어)에게 연결된 UPileHandComponent를 참조하여,
 * 턴 시작 시 카드를 뽑고, 턴 종료 시 손에 있는 카드를 버립니다.
 * 이벤트(Event_TurnStart, Event_TurnEnd)에 따라 카드 드로우 및 핸드 정리를 자동으로 처리합니다.
 */
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Draw : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Draw();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

protected:
	UPROPERTY(BlueprintReadWrite,Category="Status Draw")
	UPileHandComponent* PileHandReference;


};
