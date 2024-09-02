// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "CardBase.generated.h"

class UDispatcherHubComponent;

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


	UFUNCTION(BlueprintCallable,Category = "Card")
	bool AttemptUseCard(TArray<AActor*> Targets,bool SkipPlayableCheck, bool SkipConsequences, bool AutoPlay);

	// TODO 구현 필요 
	UFUNCTION(BlueprintCallable,Category = "Card")
	void UseCard(bool SkipConsequences, bool AutoPlay);

	UFUNCTION(BlueprintCallable,Category = "Card")
	bool CheckIfPlayable(FString& FailMessage);

	UFUNCTION(BlueprintCallable,Category = "Card")
	void ResolveUseRuleConsequences();

	UFUNCTION(BlueprintCallable,Category = "Card")
	void ContinueToNextRepetition();
	
	UFUNCTION(BlueprintCallable,Category = "Card")
	void EndCardUse();

	UFUNCTION(BlueprintCallable,Category = "Card")
	void ContinueToNextEffect();
	
	UFUNCTION(BlueprintCallable,Category = "Card")
	void ContinueToNextTarget();
	
	UFUNCTION(BlueprintCallable,Category = "Card")
	FCard GetCardDataByCardDataType(ECardDataType Type);
	
	UFUNCTION(BlueprintCallable,Category = "Card")
	int32 GetCardRepetitions(ECardDataType Type);

	UFUNCTION(BlueprintCallable,Category = "Card")
	TArray<FUseRule> GetCardUseRules(ECardDataType Type);
	
	UFUNCTION(BlueprintCallable,Category = "Card")
	FGameplayTag GetPostUseEvent(ECardDataType Type);

	UFUNCTION(BlueprintCallable,Category = "Card")
	TArray<FCardEffect> GetCardEffects(ECardDataType Type);

	UFUNCTION(BlueprintCallable,Category = "Card")
	UTargetingComponent* AccessTargetingClassLazy(AActor* TargetingHolderActor, TSubclassOf<UTargetingComponent> TargetingClass);

	UFUNCTION(BlueprintCallable,Category = "Card")
	void ProceedOnInputTargetsReceived(TArray<AActor*> Targets);

	UFUNCTION(BlueprintCallable,Category = "Card")
	void ContinueAfterCardResolved();

	UFUNCTION(BlueprintCallable,Category = "Card")
	void QueueCardEffectAction(AActor* TargetActor, AActor* SourcePuppet, UCardEffectComponent* CardEffect,bool bAnimateSourcePuppet);
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	TArray<AActor*> InputTargets;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	FCard CardDataHand;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	FCard CardDataDeck;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	FCard CardDataBase;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	FCard CardDataPile;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	int32 CurrentRepetitions;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	int32 EffectLoopIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	int32 TargetLoopIndex;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Components")
	TObjectPtr<UDispatcherHubComponent> DispatcherHubComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Components")
	TObjectPtr<UCardEffectComponent> CurrentCardEffect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Components")
	TObjectPtr<UTargetingComponent> CurrentTargeting;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	TArray<AActor*> CurrentValidTargets;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	bool bInterrupt;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	FCardEffect TempCardEffect;
	
	// 확인 필요 <class 정보, 객체> 맵이다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Variables | Card")
	TMap<TSubclassOf<UUseRuleComponent>,UUseRuleComponent*> UseRuleInstances;

	
};
