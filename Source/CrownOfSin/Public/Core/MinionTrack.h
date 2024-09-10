// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinionTrack.generated.h"

class AMinionBase;

UCLASS()
class CROWNOFSIN_API AMinionTrack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinionTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Member Variables
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MinionTrack")
	AMinionTrack* OpposingTrack;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MinionTrack")
	TArray<AMinionBase*> Minions;
};
