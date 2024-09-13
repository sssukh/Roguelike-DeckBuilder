
#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Status_Artifact.generated.h"


UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Artifact();

protected:
	virtual void BeginPlay() override;

public:
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
	
	/*========================================================================================
	*	IInterface_Utility
	=========================================================================================*/
public:
	virtual bool Interact_Implementation(const FGameplayTagContainer& Tags) override;
	
};
