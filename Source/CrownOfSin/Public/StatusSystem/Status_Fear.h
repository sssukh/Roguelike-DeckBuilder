#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Fear.generated.h"


UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Fear : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Fear();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
