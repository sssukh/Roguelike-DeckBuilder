// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UseRuleComponent.generated.h"


class ACardBase;
struct FUseRule;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UUseRuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUseRuleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual bool CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage);

	UFUNCTION(BlueprintCallable)
	virtual bool ResolveUseConsequence(FUseRule UseRuleData);
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variable | Use Rule")
	ACardBase* ParentCard;
};
