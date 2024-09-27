
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Health.generated.h"


UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Health : public UStatusComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Health();
	
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
	
};
