// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Layout_RLDB.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_Layout_RLDB : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_Layout_RLDB(const FObjectInitializer& ObjectInitializer);


	UFUNCTION()
	bool ShouldNodeMapBeBlocked();
};




