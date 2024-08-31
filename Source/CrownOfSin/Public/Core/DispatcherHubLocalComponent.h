#pragma once

#include "CoreMinimal.h"
#include "DispatcherHubComponent.h"
#include "DispatcherHubLocalComponent.generated.h"


UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UDispatcherHubLocalComponent : public UDispatcherHubComponent
{
	GENERATED_BODY()

public:
	UDispatcherHubLocalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
