// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ShopCard.generated.h"

class ACardBase;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ShopCard : public UUserWidget
{
	GENERATED_BODY()


public:
	/*ToDo 구현해야합니다.*/
	void UpdateFromCoinAmount(int32 CoinCount)
	{
		
	}
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW ShopCard",meta=(ExposeOnSpawn="true"))
	ACardBase* CardActor;
	
	
};
