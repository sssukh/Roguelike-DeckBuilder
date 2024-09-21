
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_AttackPattern.generated.h"


UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_AttackPattern : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_AttackPattern();

protected:
	virtual void BeginPlay() override;


};
