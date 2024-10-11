// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UTargetingComponent;
struct FCardEffect;
class UEffectState;
class UGameEffectComponent;

UCLASS()
class CROWNOFSIN_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	virtual void ApplyEffect(bool bSkipConsequences, bool bAutoPlay);

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void ContinueToNextEffect();

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void ContinueToNextTarget();
	
	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void ContinueToNextRepetition();

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void HandleInterruption();

	/*사용 규칙에 따른 결과를 적용합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ResolveUseRuleConsequences();

	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ProcessCurrentEffect(const FCardEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void InitializeEffectComponent(const FCardEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ContinueAfterTakingEffect();

	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void HandleImmediateEffect();

	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void EndEffect();

	/*타겟팅 클래스에 액세스하거나 새로운 타겟팅 컴포넌트를 생성합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Targeting")
	UTargetingComponent* AccessTargetingClassLazy(AActor* TargetingHolderActor, TSubclassOf<UTargetingComponent> TargetingClass);

	UFUNCTION(BlueprintCallable, Category = "Card|Targeting")
	void ProceedOnInputTargetsReceived(TArray<AActor*> Targets);

	// CardEffects
public:
	/*카드 효과 액션을 실행하는 함수.*/
	void ExecuteEffectAction();

public:
	/*주어진 타겟 액터가 UInterface_CardTarget 인터페이스를 구현하는지 확인하고, 해당 퍼펫을 반환하는 함수.*/
	AActor* GetValidTargetPuppet(AActor* TargetActor) const;

	// Effect Event
public:
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void ContinueAfterCardResolved();

	/*카드 효과에 따른 액션을 큐에 추가하여 실행합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void QueueGameEffectAction(AActor* TargetActor, AActor* SourcePuppet, UGameEffectComponent* CardEffect, bool bAnimateSourcePuppet);

	
	// Member Variable
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UGameEffectComponent> EffectComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TObjectPtr<UEffectState> EffectState;

	// 이벤트 호출을 위한 로컬 디스패처 허브
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TObjectPtr<UDispatcherHubLocalComponent> DispatcherHubLocal;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TArray<FCardEffect> OwningEffects;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TObjectPtr<UGameEffectComponent> CurrentEffectComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TObjectPtr<UTargetingComponent> CurrentTargetingComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TArray<AActor*> CurrentValidTargets;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TArray<AActor*> InputTargets;

};
