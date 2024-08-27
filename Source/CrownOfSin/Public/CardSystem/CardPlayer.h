// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardPlayer.generated.h"

class UChanceManagerComponent;
class UUWBP_Layout_RLDB;

UCLASS()
class CROWNOFSIN_API ACardPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	TObjectPtr<UUWBP_Layout_RLDB> PlayerUI; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UChanceManagerComponent* ChanceManagerComponent;
};
