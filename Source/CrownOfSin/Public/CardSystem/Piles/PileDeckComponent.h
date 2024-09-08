// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileDeckComponent.generated.h"


UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileDeckComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPileDeckComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
