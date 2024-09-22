// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UseRuleComponent.h"
#include "UseRule_Unplayable.generated.h"

/*ToDo:COS*/
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
