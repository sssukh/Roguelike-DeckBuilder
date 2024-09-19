// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_Initiative.h"
#include "MinionBase.generated.h"

class UDispatcherHubLocalComponent;
class AMinionTrack;
class UGameplayTagComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartTurn, UObject*, InSource);

UCLASS()
class CROWNOFSIN_API AMinionBase : public AActor, public IInterface_CardTarget, public IInterface_EventHolder ,public  IInterface_CardPuppet ,public IInterface_Initiative
{
	GENERATED_BODY()

public:
	AMinionBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Minion")
	void RemoveFromGame();

public:
	UFUNCTION(BlueprintPure, Category="Minion")
	FGameplayTagContainer GetGameplayTags();

public:
	UFUNCTION(BlueprintPure, Category="Minion")
	UObject* AccessPuppetUI(EStatusSlot InSlot);


	/*========================================================================================
	*	IInterface_CardTarget
	=========================================================================================*/
public:
	virtual void UnMarkTarget_Implementation() override;

	virtual bool AttemptMarkTarget_Implementation(const FGameplayTagContainer& ValidTargets) override;

	virtual bool MarkTarget_Implementation() override;

	virtual bool CheckIfValidTarget_Implementation(const FGameplayTagContainer& ValidTargets) override;

	virtual bool GetStatusValue_Implementation(TSubclassOf<UStatusComponent> InStatus, int32& OutStatusValue) override;

	virtual FString GetMinionUniqueID_Implementation() override;

	virtual bool SetStatusMax_Implementation(TSubclassOf<UStatusComponent> InStatus, int32 NewMax) override;

	virtual bool GetStatusMax_Implementation(TSubclassOf<UStatusComponent> InStatus, int32& OutMax) override;

	virtual int32 SubtractFromStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad) override;

	virtual  AActor* GetPuppet_Implementation() override;

	virtual int32 AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad) override;

	/*========================================================================================
	*	IInterface_CardPuppet
	=========================================================================================*/
public:
	virtual FVector GetPuppetRelativeCenter_Implementation() override;

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	virtual FText GetFriendlyName_Implementation() override;

	virtual float GetPriority_Implementation() override;

	/*========================================================================================
	*	IInterface_Initiative
	=========================================================================================*/
public:
	virtual float GetInitiative_Implementation() override;

	
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion|Component")
	UDispatcherHubLocalComponent* DispatcherHubLocalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion|Component")
	UGameplayTagComponent* GameplayTagComponent;

public:
	UPROPERTY(BlueprintReadOnly, Category="Minion")
	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta=(ExposeOnSpawn="true"))
	bool bSpawnedDuringGameplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta=(ExposeOnSpawn="true"))
	FMinion MinionData;

	UPROPERTY(BlueprintReadWrite, Category="Minion")
	AActor* Puppet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta=(ExposeOnSpawn="true"))
	AMinionTrack* MinionTrack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta=(ExposeOnSpawn="true"))
	TArray<UStatusComponent*> StartTurnStatusEvents;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Minion|Delegate")
	FOnStartTurn OnStartTurn;
};
