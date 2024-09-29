// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Status_Artifact.h"
#include "Status_Artifact_JackOLantern.generated.h"


class UStatus_Health;
class ACardPlayer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact_JackOLantern : public UStatus_Artifact
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Artifact_JackOLantern();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
	
	// Member Variables
public:
	UPROPERTY(EditAnywhere)
	int32 ActivationThreshold = 20;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<ACardPlayer> CardPlayerRef;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStatus_Health> AffectedHealthComponent;
	
};
