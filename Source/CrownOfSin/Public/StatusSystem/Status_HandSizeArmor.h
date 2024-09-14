// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_HandSizeArmor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_HandSizeArmor : public UStatusComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_HandSizeArmor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
