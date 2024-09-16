// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "MinionBase.generated.h"

class AMinionTrack;
class UGameplayTagComponent;

UCLASS()
class CROWNOFSIN_API AMinionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:
	UFUNCTION(BlueprintPure,Category="Minion")
	FGameplayTagContainer GetGameplayTags();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion|Component")
	UGameplayTagComponent* GameplayTagComponent;
	
public:
	UPROPERTY( BlueprintReadOnly, Category="Minion")
	bool bDead=false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta=(ExposeOnSpawn="true"))
	FMinion MinionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta=(ExposeOnSpawn="true"))
	TObjectPtr<AMinionTrack> MinionTrack;
};
