// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "UW_DispatcherHubDebug.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_DispatcherHubDebug : public UUserWidget,public IInterface_EventHolder
{
	GENERATED_BODY()
};
