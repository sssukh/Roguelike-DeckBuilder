// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffect_StoreTargetAsPayload.h"
#include "Components/ActorComponent.h"
#include "PayloadHolderComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPayloadHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPayloadHolderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/*ToDo:구현해야함*/
	void AddPayload(const FString& Identifier, UObject* Payload)
	{
		
	}

	// TODO : 구현 필요
	bool AccessPayload(FString Identifier, bool bDestroyAfterAccess, UObject*& Payload)
	{
		return true;	
	}
};
