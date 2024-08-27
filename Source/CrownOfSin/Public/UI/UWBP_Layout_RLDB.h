// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWBP_Layout_RLDB.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUWBP_Layout_RLDB : public UUserWidget
{
	GENERATED_BODY()
public:
	UUWBP_Layout_RLDB();


	UFUNCTION()
	bool ShouldNodeMapBeBlocked();
};
