// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Layout_Cos.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_Layout_Cos : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="UW Layout")
	bool ShouldNodeMapBeBlocked();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="UW Layout")
	void DisplayScreenLogMessage(const FText& Message, const FColor& Color);
};




