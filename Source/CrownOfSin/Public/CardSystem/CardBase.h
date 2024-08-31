// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "CardBase.generated.h"

UCLASS()
class CROWNOFSIN_API ACardBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
	/*========================================================================================
	 *	Field Members
	 =========================================================================================*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card", meta=(ExposeOnSpawn="true"))
	FCard CardDataDeck;
};
