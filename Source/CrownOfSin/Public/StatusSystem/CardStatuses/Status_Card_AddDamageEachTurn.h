#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardBase.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/FlowControllerLibiary.h"
#include "Status_Card_AddDamageEachTurn.generated.h"


/*매 턴 종료 시, 소유 카드에 있는 모든 공격 태그 카드 효과의 효과 값에 X를 추가합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Card_AddDamageEachTurn : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Card_AddDamageEachTurn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

public:
	UPROPERTY()
	int32 CurrentModification = 0;

	UPROPERTY(BlueprintReadWrite, Category="Status Card Add DamageEachTurn")
	TObjectPtr<ACardBase> OwningCard;

	UPROPERTY()
	FGate RunEventGate = FGate(true);
};
