// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Action_AdvAnim.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_AdvAnim : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_AdvAnim();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Adv Anim", meta=(ExposeOnSpawn="true"))
	FGameplayTag Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Adv Anim", meta=(ExposeOnSpawn="true"))
	AActor* Puppet;
};
