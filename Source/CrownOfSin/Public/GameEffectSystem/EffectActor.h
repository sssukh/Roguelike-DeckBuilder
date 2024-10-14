// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UDispatcherHubLocalComponent;
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

	// Initialize
public:
	/*
	  * 카드 데이터 테이블에서 Deck 데이터를 가져와 카드를 초기화합니다.
	  * 카드 데이터를 설정한 후, 카드 사용 규칙과 상태 컴포넌트를 설정하고 카드의 희귀도 및 타입을 결정합니다.
	  */
	void InitializeFromData();

	/* 
	  * 주어진 데이터 테이블 핸들을 통해 카드 데이터를 찾아 초기화합니다.
	  * 데이터가 유효하지 않거나 존재하지 않을 경우, 에러 처리를 수행하고 초기화를 중단합니다.
	  */
	bool InitializeEffectDataFromRow(const FDataTableRowHandle& EffectDataRowHandle);

	/* 
	  * 유효하지 않은 카드 데이터를 처리하고, 필요한 경우 에러 메시지를 출력하며 기본 데이터를 설정합니다.
	  */
	bool HandleInvalidData(const FDataTableRowHandle& CardDataRowHandle);

	/* 
	  * 카드의 Pile, Hand 데이터를 초기화하며, 해당 카드에 대한 GameplayTags를 설정합니다.
	  */
	// void InitializeCardData();

	/*
	  * 카드 사용 규칙에 해당하는 컴포넌트들을 설정합니다.
	  * 각 카드의 사용 규칙에 맞는 UUseRuleComponent를 생성하고 등록한 후, 카드의 사용 규칙 인스턴스에 추가합니다.
	  */
	void SetupUseRuleComponents();

	/*
	  * 카드의 초기 상태를 설정합니다.
	  * 카드가 게임에서 시작될 때 필요한 상태 컴포넌트를 생성하고 등록합니다.
	  */
	void SetupStatusComponents();
	
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
	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void ResolveUseRuleConsequences();

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void ProcessCurrentEffect(const FCardEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void InitializeEffectComponent(const FCardEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void ContinueAfterTakingEffect();

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void HandleImmediateEffect();

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
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
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UGameEffectComponent> EffectComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TObjectPtr<UEffectState> EffectState;
	
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

	// 카드 사용 규칙을 관리하는 컴포넌트 맵
	UPROPERTY(BlueprintReadWrite, Category = "Effect Actor")
	TMap<TSubclassOf<UUseRuleComponent>, UUseRuleComponent*> UseRuleInstances;

	UPROPERTY(BlueprintReadWrite, Category = "Effect Actor")
	FCard EffectData;

	// 카드 이벤트 호출을 위한 로컬 디스패처 허브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UDispatcherHubLocalComponent> DispatcherHubLocal;

	// 카드의 게임플레이 태그를 관리하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;
};
