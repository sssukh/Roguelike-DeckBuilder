// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_Card.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_Card : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Function Library|Card")
	static FCard MakeCardStructFromCardData(FDataTableRowHandle CardData);
};
