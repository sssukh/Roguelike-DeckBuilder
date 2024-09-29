// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "UseRuleComponent.generated.h"


class ACardBase;


UCLASS(ClassGroup=("COS|Rule"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UUseRuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUseRuleComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage);

	UFUNCTION(BlueprintCallable)
	virtual bool ResolveUseConsequence(FUseRule UseRuleData);

	UFUNCTION(BlueprintCallable)
	void InitializeUseRule();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category = "Variable | Use Rule")
	TObjectPtr<ACardBase> ParentCard;
};
