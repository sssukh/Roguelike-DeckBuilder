// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/Artifacts/Status_Artifact.h"
#include "Status_Artifact_BagOfHolding.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact_BagOfHolding : public UStatus_Artifact
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Artifact_BagOfHolding();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
