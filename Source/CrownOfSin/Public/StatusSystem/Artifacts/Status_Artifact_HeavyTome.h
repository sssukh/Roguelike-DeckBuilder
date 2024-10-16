﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Status_Artifact.h"
#include "Status_Artifact_HeavyTome.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact_HeavyTome : public UStatus_Artifact
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Artifact_HeavyTome();

	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;

};
