#pragma once

#include "CoreMinimal.h"
#include "CardEffects/CardEffectComponent.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_Interrupt.h"
#include "CardBase.generated.h"

class UGameplayTagComponent;
class UDispatcherHubLocalComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThisCardModified);

/*툴킷의 중심 행위자.
 *카드 액터는 눈에 보이지 않지만 WBP_Card를 통해 시각화되고 입력을 받습니다.
 *카드 속성은 데이터 테이블(예: DT_Cards)에 정의됩니다.
 *사용되면 TargetingComponent를 통해 대상을 찾고 최종적으로 이러한 대상에 대한 CardEffect를 해결하기 전에 모든 CardUseRules가 유효한지 확인합니다.*/
UCLASS()
class CROWNOFSIN_API ACardBase : public AActor, public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardBase();

protected:
	virtual void BeginPlay() override;

	// Deck카드 데이터를 통해 필요한 컴포넌트들과 변수들을 세팅합니다.
	// Story 선택지로서는 RunMapEvent_Story->CardPlayer->Layout_Cos->StoryEncounter->StoryButton으로 이어지는 흐름에서
	// 받아온 Encounter Card 정보로 카드를 생성할 때 Deck에 값을 넣어주기 때문에 그 값으로 세팅을 하는것 같다.
	void InitializeFromData();

	// 카드 데이터 테이블에서 데이터를 찾고, 초기화하는 함수
	bool InitializeCardDataFromRow(const FDataTableRowHandle& CardDataRowHandle);

	// 유효하지 않은 카드 데이터를 처리하는 함수
	bool HandleInvalidCardData(const FDataTableRowHandle& CardDataRowHandle);

	// 카드 데이터를 초기화하는 함수
	void InitializeCardData();

	// 카드 사용 규칙 컴포넌트를 설정하는 함수
	void SetupUseRuleComponents();

	// 카드의 초기 상태 컴포넌트를 설정하는 함수
	void SetupStatusComponents();

public:
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
	void UseCard(bool bSkipConsequences, bool bAutoPlay);

	/*
	 * 이 카드를 사용할 수 있는지 체크하는 함수이다.
	 *  @ Params FailMessage : 에러 메세지를 받아오기위한 매개변수 
	 */
	UFUNCTION(BlueprintCallable, Category = "Card")
	bool CheckIfPlayable(FString& OutFailMessage);

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
	
	FCard& GetCardDataByCardDataTypeRef(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	FDataTableRowHandle GetCardDataRowHandle(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	FText GetCardName(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	FGameplayTagContainer GetCardTags(ECardDataType Type);

	UFUNCTION(BlueprintCallable, Category = "Card")
	TArray<FStatusData> GetCardStartingStatuses(ECardDataType Type);

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
	bool CallLocalEventOnCard(const FGameplayTag& EventTag, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter);

	UFUNCTION(BlueprintCallable, Category = "Card")
	void Discard(FGameplayTagContainer CallTags);
	
	UFUNCTION(BlueprintCallable, Category = "Card")
	void ModifyCardEffectValues(int32 Modification, ECardDataType InCardType, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags);

	UFUNCTION(BlueprintCallable, Category = "Card")
	void Exhaust();

public:
	
	// 카드의 Rarity를 알아야할 때 모든 게임플레이 태그를 순회하지 않도록 하는 캐싱함수입니다.
	// rarity tag들만 바로 순회해서 Rarity 멤버변수를 세팅하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Card")
	FGameplayTag SetCardRarityFromTags();

	// 카드의 타입을 알아야할 때 모든 게임플레이 태그를 순회하지 않도록 하는 캐싱함수입니다.
	// effect tag들만 바로 순회해서 Type 멤버변수를 세팅하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Card")
	FGameplayTag SetCardTypeFromTags();

	
	void SetCardEffects(ECardDataType InCardType, const TArray<FCardEffect>& NewCardEffects);

public:
	UFUNCTION(BlueprintCallable, Category = "Card")
	FGameplayTagContainer GetGameplayTags();

	UFUNCTION(BlueprintPure, Category = "Card")
	FText GetCardDescription(ECardDataType InCardDataType);
	


protected:
	void InitializeCurrentCardEffect(const FCardEffect& CardEffect);

	void HandleImmediateCardEffect();

	void ExecuteEffectAction();

	AActor* GetValidTargetPuppet(AActor* TargetActor) const;


	/*========================================================================================
	*	IInterface_Interrupt
	=========================================================================================*/
public:
	virtual void Interrupt_Implementation() override;;
	virtual void CancelInterruption_Implementation() override;

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

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FGameplayTag Rarity;

	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	FGameplayTag CardType;

	// 확인 필요 <class 정보, 객체> 맵이다.
	UPROPERTY(BlueprintReadWrite, Category = "Variables | Card")
	TMap<TSubclassOf<UUseRuleComponent>, UUseRuleComponent*> UseRuleInstances;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Card|Delegate")
	FOnThisCardModified OnThisCardModified;
};
