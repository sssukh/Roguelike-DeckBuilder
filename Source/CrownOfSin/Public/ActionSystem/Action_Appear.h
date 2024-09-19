// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_Appear.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_Appear : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_Appear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Appear", meta=(ExposeOnSpawn="true"))
	bool bDisappear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Appear", meta=(ExposeOnSpawn="true"))
	float FadeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Appear", meta=(ExposeOnSpawn="true"))
	AActor* Puppet;
};
