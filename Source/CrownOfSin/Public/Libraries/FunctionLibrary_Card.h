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
	static FCard MakeCardStructFromCardData(const FDataTableRowHandle& CardData);

	UFUNCTION(BlueprintCallable, Category="Function Library|Card",meta = (WorldContext = "WorldContextObject"))
	static void SpawnAndPlayHeroCard(const UObject* WorldContextObject,const FDataTableRowHandle& CardData, TArray<AActor*> Targets, AActor* OwnerOverride);
	
	UFUNCTION(BlueprintCallable, Category="Function Library|Card",meta = (WorldContext = "WorldContextObject"))
	static void SpawnAndPlayHeroCardUnTargeted(const UObject* WorldContextObject,const FDataTableRowHandle& CardData,AActor* OwnerOverride);
};
