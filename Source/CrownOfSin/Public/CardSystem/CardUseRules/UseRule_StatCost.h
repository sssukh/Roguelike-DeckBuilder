#pragma once

#include "CoreMinimal.h"
#include "UseRuleComponent.h"
#include "UseRule_StatCost.generated.h"

/*CardPlayer의 지정된 상태 값이 지정된 비용과 동일하지 않으면 카드가 재생되는 것을 방지합니다.*/
UCLASS(ClassGroup=("COS|Rule"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UUseRule_StatCost : public UUseRuleComponent
{
	GENERATED_BODY()

public:
	UUseRule_StatCost();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage) override;

	virtual bool ResolveUseConsequence(FUseRule UseRuleData) override;
};
