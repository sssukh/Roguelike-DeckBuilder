// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Armor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Armor : public UStatusComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Armor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Armor")
	int32 OldArmor;
};
