// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputTargetsReceived, TArray<AActor*>,Targets);

class ACardBase;
struct FCardEffect;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category="Targeting Component")
	bool FindValidTargets(TArray<AActor*> SpecifiedTargets, FCardEffect CardEffect,ACardBase* Card,bool Preview, TArray<AActor*>& ValidTargets);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bRequiresInput;

	/*========================================================================================
	*	Delegates
	=========================================================================================*/
	FOnInputTargetsReceived OnInputTargetsReceived;
};
