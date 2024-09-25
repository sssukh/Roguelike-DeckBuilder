// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MessageNotifyBox.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_MessageNotifyBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,Category="UW Message Notify Box Event")
	void ReceiveScreenLogMessage(const FText& Message, FLinearColor Color);
};
