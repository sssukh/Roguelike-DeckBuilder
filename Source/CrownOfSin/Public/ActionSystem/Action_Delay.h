
#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_Delay.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_Delay : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_Delay();

protected:
	virtual void BeginPlay() override;

public:
	
};
