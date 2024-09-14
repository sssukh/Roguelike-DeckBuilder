﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Mana.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Mana : public UStatusComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Mana();

	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
};
