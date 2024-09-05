// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CardEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardResolved);

class AAction_Effect;
class UTargetingComponent;
class ACardBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool ResolveCardEffect();
	/*========================================================================================
	*	Field Members
	=========================================================================================*/

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	int32 EffectValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	TSubclassOf<UActorComponent> TargetComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	TObjectPtr<ACardBase> ParentCard;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	bool bTargeted = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	bool bImmediated = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	TSubclassOf<UTargetingComponent> TargetingClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	TSubclassOf<UTargetingComponent> DefaultTargetingClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	FGameplayTag HeroAnim;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	TSubclassOf<AAction_Effect> EffectAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	FDataTableRowHandle UsedData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	bool bInterrupt;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Card Effect | Variable")
	FString Identifier;

	/*========================================================================================
	*	Delegate
	=========================================================================================*/
	FOnCardResolved OnCardResolved;
};
