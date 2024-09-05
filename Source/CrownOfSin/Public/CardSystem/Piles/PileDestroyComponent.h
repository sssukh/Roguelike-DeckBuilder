
#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileDestroyComponent.generated.h"


UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileDestroyComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPileDestroyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
