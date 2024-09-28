#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Utilities/FlowControllerLibiary.h"
#include "Status_Venomous.generated.h"

/*상태 소유자의 공격으로 피해를 입은 모든 하수인에게 X 독을 추가합니다(방어력을 뺀 후).*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Venomous : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Venomous();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

public:
	UPROPERTY()
	FGate RunEventGate = FGate(true);

	UPROPERTY(BlueprintReadWrite,Category="Status Venomous")
	float SplashDelay = 0.1f;
};
