
#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileHandComponent.generated.h"


UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileHandComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPileHandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
};
