// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Interrupt.h"
#include "GameEffectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UGameEffectComponent : public UActorComponent , public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeEffect();
	
	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffect();

public:
	// Interface_Interrupt
	virtual void Interrupt_Implementation();
	
	virtual void CancelInterruption_Implementation() override;
};
