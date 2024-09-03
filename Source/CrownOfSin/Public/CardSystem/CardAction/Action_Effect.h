// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardAction.h"
#include "GameplayTagContainer.h"
#include "Core/CosEnumStruct.h"
#include "Action_Effect.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_Effect : public ACardAction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_Effect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Action Effect")
	AActor* Target;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Action Effect")
	AActor* SourcePuppet;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Action Effect")
	FGameplayTagContainer AnimTags;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Action Effect")
	FGameplayTag HeroAnim;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Action Effect")
	FCardEffect Effect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Action Effect")
	float StartDelayIfNoAnim;
};
