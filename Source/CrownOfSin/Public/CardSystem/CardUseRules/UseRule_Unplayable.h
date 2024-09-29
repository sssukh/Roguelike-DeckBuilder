
#pragma once

#include "CoreMinimal.h"
#include "UseRuleComponent.h"
#include "UseRule_Unplayable.generated.h"

/*카드가 재생되는 것을 방지합니다.*/
UCLASS(ClassGroup=("COS|Rule"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UUseRule_Unplayable : public UUseRuleComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUseRule_Unplayable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	virtual bool CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage) override;
};
