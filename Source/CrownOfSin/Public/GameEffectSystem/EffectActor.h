// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

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
	void ProcessCurrentEffect();

	UFUNCTION(BlueprintCallable, Category = "Effect Actor")
	void HandleInterruption();

	/*사용 규칙에 따른 결과를 적용합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ResolveUseRuleConsequences();

	
	// Member Variable
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UGameEffectComponent> EffectComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effect Actor")
	TObjectPtr<UEffectState> EffectState;

	// 이벤트 호출을 위한 로컬 디스패처 허브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UDispatcherHubLocalComponent> DispatcherHubLocal;
};
