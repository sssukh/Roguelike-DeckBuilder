// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectState.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UEffectState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEffectState();

	UFUNCTION(BlueprintCallable, Category = "EffectState")
	void ResetState();
	
	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectState")
	int32 EffectLoopIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectState")
	int32 TargetLoopIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectState")
	int32 Repetition = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectState")
	TArray<AActor*> CurrentValidTargets;

	bool bInterrupt = false;
};
