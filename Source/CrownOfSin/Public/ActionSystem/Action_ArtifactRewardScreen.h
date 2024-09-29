// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_ArtifactRewardScreen.generated.h"

class UStatusComponent;

UCLASS()
class CROWNOFSIN_API AAction_ArtifactRewardScreen : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_ArtifactRewardScreen();

	virtual void PlayAction_Implementation() override;

	// MemberVariable
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Artifact Reward Screen", meta = (ExposeOnSpawn = true))
	TObjectPtr<UStatusComponent> Artifact;
};
