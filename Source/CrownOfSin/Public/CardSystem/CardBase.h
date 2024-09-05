#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "CardBase.generated.h"

class UGameplayTagComponent;
class UDispatcherHubLocalComponent;


/*툴킷의 중심 행위자.
 *카드 액터는 눈에 보이지 않지만 WBP_Card를 통해 시각화되고 입력을 받습니다.
 *카드 속성은 데이터 테이블(예: DT_Cards)에 정의됩니다.
 *사용되면 TargetingComponent를 통해 대상을 찾고 최종적으로 이러한 대상에 대한 CardEffect를 해결하기 전에 모든 CardUseRules가 유효한지 확인합니다.*/
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

	/*
	 *  CheckIfPlayable()을 통해 Play가능 여부를 확인하고 UseCard를 호출한다.
	 *  SkipPlayableCheck를 통해 가능 여부 확인하는 단계를 스킵할 수 있다.
	 *  
	 */
	UFUNCTION(BlueprintCallable, Category = "Card")
	bool AttemptUseCard(TArray<AActor*> Targets, bool SkipPlayableCheck, bool SkipConsequences, bool AutoPlay);

	/*
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Card")
	void UseCard(bool SkipConsequences, bool AutoPlay);

	/*
	 * 이 카드를 사용할 수 있는지 체크하는 함수이다.
	 *  @ Params FailMessage : 에러 메세지를 받아오기위한 매개변수 
	 */
	UFUNCTION(BlueprintCallable, Category = "Card")
	bool CheckIfPlayable(FString& FailMessage);

	/*
	 * 규칙들을 순회하면서 그 안에 정의된 결과를 적용한다(resolve).
	 */
	UFUNCTION(BlueprintCallable, Category = "Card")
	void ResolveUseRuleConsequences();

	UFUNCTION(BlueprintCallable, Category = "Card")
	void ContinueToNextRepetition();

	UFUNCTION(BlueprintCallable, Category = "Card")
	void EndCardUse();

	UFUNCTION(BlueprintCallable, Category = "Card")
	void ContinueToNextEffect();

	UFUNCTION(BlueprintCallable, Category = "Card")
	void ContinueToNextTarget();

	UFUNCTION(BlueprintCallable, Category = "Card")
	FCard GetCardDataByCardDataType(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	int32 GetCardRepetitions(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	TArray<FUseRule> GetCardUseRules(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	FGameplayTag GetPostUseEvent(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	TArray<FCardEffect> GetCardEffects(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	UTargetingComponent* AccessTargetingClassLazy(AActor* TargetingHolderActor, TSubclassOf<UTargetingComponent> TargetingClass);

	UFUNCTION(BlueprintCallable, Category = "Card")
	void ProceedOnInputTargetsReceived(TArray<AActor*> Targets);

	UFUNCTION(BlueprintCallable, Category = "Card")
	void ContinueAfterCardResolved();

	UFUNCTION(BlueprintCallable, Category = "Card")
	void QueueCardEffectAction(AActor* TargetActor, AActor* SourcePuppet, UCardEffectComponent* CardEffect, bool bAnimateSourcePuppet);


	UFUNCTION(BlueprintCallable, Category = "Card")
	FGameplayTagContainer GetGameplayTags();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | Components")
	TObjectPtr<UDispatcherHubLocalComponent> DispatcherHubLocal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | Components")
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | Components")
	TObjectPtr<UCardEffectComponent> CurrentCardEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | Components")
	TObjectPtr<UTargetingComponent> CurrentTargeting;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	TArray<AActor*> InputTargets;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FCard CardDataHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | Card", meta=(ExposeOnSpawn="true"))
	FCard CardDataDeck;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FCard CardDataBase;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FCard CardDataPile;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	int32 CurrentRepetitions;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	int32 EffectLoopIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	int32 TargetLoopIndex;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	TArray<AActor*> CurrentValidTargets;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	bool bInterrupt;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FCardEffect TempCardEffect;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FGameplayTag CurrentPile;

	// 확인 필요 <class 정보, 객체> 맵이다.
	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	TMap<TSubclassOf<UUseRuleComponent>, UUseRuleComponent*> UseRuleInstances;
};
