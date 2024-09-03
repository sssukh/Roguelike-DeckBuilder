
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapEventComponent.generated.h"


UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapEventComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void RunMapEvent(FDataTableRowHandle EventData);
	
};
