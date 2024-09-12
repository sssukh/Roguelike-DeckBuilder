#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_Interrupt.h"
#include "CardBase.generated.h"

class UUseRule_StatCost;
class UGameplayTagComponent;
class UDispatcherHubLocalComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThisCardModified);


/* ACardBase 클래스는 게임 내 카드 시스템의 핵심 액터입니다.
 * 툴킷의 중심 행위자.
 * 카드 액터는 눈에 보이지 않지만 WBP_Card를 통해 시각화되고 입력을 받습니다.
 * 카드 속성은 데이터 테이블(예: DT_Cards)에 정의됩니다.
 * 사용되면 TargetingComponent를 통해 대상을 찾고 최종적으로 이러한 대상에 대한 CardEffect를 해결하기 전에 모든 CardUseRules가 유효한지 확인합니다.
 * 이 클래스는 카드 데이터를 초기화하고, 카드 사용 규칙을 확인하며, 카드 효과를 실행하는 역할을 담당합니다.
 * 또한, 인터페이스를 구현하여 카드 사용 도중 중단(interrupt) 등의 상황을 처리할 수 있습니다.
 *
 * 주요 기능:
 * 1. 카드 데이터 초기화: 데이터 테이블에서 카드를 로드하고 필요한 컴포넌트 및 데이터를 설정합니다.
 * 2. 카드 사용: 카드 사용 가능 여부를 확인하고, 사용 시 발생하는 규칙 및 효과를 처리합니다.
 * 3. 카드 효과 처리: 다수의 타겟에 대한 카드 효과를 처리하고, 반복되는 카드 효과를 지원합니다.
 * 4. 인터럽트 처리: 카드 사용 중단 및 중단 취소 기능을 제공합니다.
 * 5. 카드 관련 유틸리티 함수: 카드의 이름, 설명, 태그 등의 정보를 가져오는 다양한 유틸리티 함수가 포함되어 있습니다.
 */
UCLASS()
class CROWNOFSIN_API ACardBase : public AActor, public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	ACardBase();

protected:
	virtual void BeginPlay() override;

	/*
 	 * 카드 데이터 테이블에서 Deck 데이터를 가져와 카드를 초기화합니다.
 	 * 카드 데이터를 설정한 후, 카드 사용 규칙과 상태 컴포넌트를 설정하고 카드의 희귀도 및 타입을 결정합니다.
 	 */
	void InitializeFromData();

	/* 
 	 * 주어진 데이터 테이블 핸들을 통해 카드 데이터를 찾아 초기화합니다.
 	 * 데이터가 유효하지 않거나 존재하지 않을 경우, 에러 처리를 수행하고 초기화를 중단합니다.
 	 */
	bool InitializeCardDataFromRow(const FDataTableRowHandle& CardDataRowHandle);

	/* 
 	 * 유효하지 않은 카드 데이터를 처리하고, 필요한 경우 에러 메시지를 출력하며 기본 데이터를 설정합니다.
 	 */
	bool HandleInvalidCardData(const FDataTableRowHandle& CardDataRowHandle);

	/* 
 	 * 카드의 Pile, Hand 데이터를 초기화하며, 해당 카드에 대한 GameplayTags를 설정합니다.
 	 */
	void InitializeCardData();

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


	/*========================================================================================
	 *	Card Usage
	 =========================================================================================*/
public:
	/*
	 * 카드가 재생 가능한지 확인합니다. 그렇다면 UseCard가 호출됩니다. 그렇지 않으면 실패 메시지를 기록합니다.
	 * 타겟 목록과 여러 옵션을 통해 카드를 사용하려고 시도합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	bool AttemptUseCard(TArray<AActor*> Targets, bool SkipPlayableCheck, bool SkipConsequences, bool AutoPlay);

	/**
	 * 타겟이 없는(UnTargeted) 카드를 시도하는 함수.
	 * 카드 사용 가능 여부를 확인하고, 타겟이 필요 없는 카드를 사용한다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	bool AttemptUseCardUnTargeted(bool bSkipPlayableCheck, bool bSkipConsequences, bool bAutoPlay);

	/*시그널 카드 사용 및 사용 결과 해결  카드를 사용하여 카드에 정의된 행동을 실행합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void UseCard(bool bSkipConsequences, bool bAutoPlay);

	/*카드 사용이 완료된 후, 후속 이벤트를 호출합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void EndCardUse();

	/*카드가 사용 가능한지 여부를 확인합니다. 사용 가능할 경우 true, 그렇지 않으면 false를 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	bool CheckIfPlayable(FString& OutFailMessage);

	/*카드 사용 규칙에 따른 결과를 적용합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ResolveUseRuleConsequences();

	/*카드 사용 시 설정된 반복 횟수를 관리하고, 반복이 남아 있을 경우 다음 효과로 진행합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ContinueToNextRepetition();

	/*현재 카드 효과를 처리하고, 다음 카드 효과로 이동합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ContinueToNextEffect();

	/*카드의 현재 타겟을 처리하고, 다음 타겟 또는 다음 카드 효과로 넘어가는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ContinueToNextTarget();

	/*카드 사용 시, 타겟 입력이 필요한 경우 유효한 타겟들을 받아 저장하고, 이후 카드의 다음 타겟을 처리하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Usage")
	void ProceedOnInputTargetsReceived(TArray<AActor*> Targets);


	/*========================================================================================
	*	CardEffects And Targeting
	=========================================================================================*/
protected:
	/*카드 효과 컴포넌트를 초기화하는 함수.*/
	void InitializeCurrentCardEffect(const FCardEffect& CardEffect);

	/*즉시 실행 카드 효과를 처리하는 함수.*/
	void HandleImmediateCardEffect();

	/*카드 효과 액션을 실행하는 함수.*/
	void ExecuteEffectAction();

	/*주어진 타겟 액터가 UInterface_CardTarget 인터페이스를 구현하는지 확인하고, 해당 퍼펫을 반환하는 함수.*/
	AActor* GetValidTargetPuppet(AActor* TargetActor) const;

	/*잠재적인 타겟들을 찾아 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Targeting")
	TArray<AActor*> FindAllPotentialTargets();

	/*타겟팅 클래스에 액세스하거나 새로운 타겟팅 컴포넌트를 생성합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Targeting")
	UTargetingComponent* AccessTargetingClassLazy(AActor* TargetingHolderActor, TSubclassOf<UTargetingComponent> TargetingClass);


	/*========================================================================================
	*	Card Data Getter
	=========================================================================================*/
public:
	/*주어진 카드 데이터 타입에 해당하는 카드 데이터를 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Data")
	FCard GetCardByCardDataType(ECardDataType InCardType);

	/*주어진 카드 데이터 타입에 해당하는 카드 데이터를 참조로 반환합니다.*/
	FCard& GetCardByCardDataTypeRef(ECardDataType InCardType);

	/*카드 데이터 테이블 행 핸들을 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FDataTableRowHandle GetCardDataRowHandle(ECardDataType Type);

	/*카드 이름을 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FText GetCardName(ECardDataType Type);

	/*카드의 게임플레이 태그 컨테이너를 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FGameplayTagContainer GetCardTags(ECardDataType Type);

	/*카드의 시작 상태 데이터를 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	TArray<FStatusData> GetCardStartingStatuses(ECardDataType Type);

	/*카드의 PostUse 이벤트 태그를 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Data")
	FGameplayTag GetPostUseEvent(ECardDataType Type);

	/*카드의 효과 리스트를 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Data")
	TArray<FCardEffect> GetCardEffects(ECardDataType Type);

	/*카드의 반복 횟수를 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	int32 GetCardRepetitions(ECardDataType Type);

	/*카드의 사용 규칙 리스트를 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	TArray<FUseRule> GetCardUseRules(ECardDataType Type);

	/*카드의 설명을 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FText GetCardDescription(ECardDataType InCardDataType);

	/*카드 소유자의 고유 ID를 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FString GetCardOwnerID(ECardDataType InCardDataType);

	/*종료 턴 이벤트를 반환하는 함수.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FGameplayTag GetCardEndTurnEvent(ECardDataType InCardDataType);

	/*카드의 모든 게임플레이 태그를 반환합니다.*/
	UFUNCTION(BlueprintPure, Category = "Card|Data")
	FGameplayTagContainer GetGameplayTags();

	/*카드 효과의 값을 가져오는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Data")
	bool GetCardEffectValue(FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, ECardDataType InCardType, int32& OutEffectValue);

	/*카드 사용 규칙 중 상태 컴포넌트에 해당하는 비용을 가져오는 함수.*/
	bool GetStatusCostValueFromUseRules(ECardDataType InPile, const TSubclassOf<UStatusComponent>& StatusClass, int32& OutStatusCost, int32& OutUseRuleIndex);

	/*카드가 현재 '손'에 있는지 확인하는 함수.*/
	UFUNCTION(BlueprintPure, Category = "Card|State")
	bool IsInHand();


	/*========================================================================================
	*	카드 이벤트 및 동작 관련 함수
	=========================================================================================*/
public:
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void ContinueAfterCardResolved();

	/*카드 효과에 따른 액션을 큐에 추가하여 실행합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void QueueCardEffectAction(AActor* TargetActor, AActor* SourcePuppet, UCardEffectComponent* CardEffect, bool bAnimateSourcePuppet);

	/*카드에 대해 로컬 이벤트를 호출하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	bool CallLocalEventOnCard(const FGameplayTag& EventTag, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter);

	/*카드를 버리는 동작을 처리합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void Discard(FGameplayTagContainer CallTags);

	/*카드 효과의 값을 수정하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void ModifyCardEffectValues(int32 Modification, ECardDataType InCardType, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags);

	/*카드가 소모될 때 발생하는 이벤트를 호출합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void Exhaust();

	/*ResetInHandModifications 함수는 카드의 Hand 상태를 초기화합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void ResetInHandModifications();

	/*카드가 손에 있을 때 종료 턴 이벤트를 실행하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	void RunEndTurnEvent(FGameplayTagContainer CallTags);

	/*로컬 카드 이벤트를 바인딩하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Event")
	bool BindLocalCardEvents(UObject* EventHolder, FGameplayTagContainer EventTags);


	/*========================================================================================
	*	Set Card Properties
	=========================================================================================*/
public:
	/*카드의 희귀도를 설정하고 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Attributes")
	FGameplayTag SetCardRarityFromTags();

	/*함수는 카드의 유형을 설정합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Attributes")
	FGameplayTag SetCardTypeFromTags();

	/*카드의 특정 타입에 대해 새로운 카드 효과 리스트를 설정하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Attributes")
	void SetCardEffects(ECardDataType InCardType, const TArray<FCardEffect>& NewCardEffects);

	/*카드의 특정 더미에 대해 새로운 사용 규칙 리스트를 설정하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Attributes")
	void SetCardUseRules(ECardDataType Pile, const TArray<FUseRule>& NewUseRules);

	/*카드 더미에 대해 사용 규칙의 비용을 설정하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Attributes")
	bool SetUseRuleCost(TSubclassOf<UUseRule_StatCost> UseRule, TSubclassOf<UStatusComponent> Status, int32 NewValue, ECardDataType Pile, bool bAddIfNotFound);

	/*카드 데이터를 복사하는 함수.*/
	UFUNCTION(BlueprintCallable, Category = "Card|Attributes")
	void SetCardDataFromOtherData(ECardDataType From, ECardDataType To);


	/*========================================================================================
	*	IInterface_Interrupt
	=========================================================================================*/
public:
	virtual void Interrupt_Implementation() override;

	virtual void CancelInterruption_Implementation() override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	// 카드 이벤트 호출을 위한 로컬 디스패처 허브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UDispatcherHubLocalComponent> DispatcherHubLocal;

	// 카드의 게임플레이 태그를 관리하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;

protected:
	// 현재 카드 효과를 처리하는 컴포넌트
	UPROPERTY(BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UCardEffectComponent> CurrentCardEffectComponent;

	// 현재 카드의 타겟팅을 관리하는 컴포넌트
	UPROPERTY(BlueprintReadWrite, Category = "Card|Components")
	TObjectPtr<UTargetingComponent> CurrentTargetingComponent;

	// 카드 데이터 관련 멤버 변수
public:
	// 입력된 타겟 리스트
	UPROPERTY(BlueprintReadWrite, Category = "Card|Data")
	TArray<AActor*> InputTargets;

	// 손에 있는 카드 데이터
	UPROPERTY(BlueprintReadWrite, Category = "Card|Data")
	FCard CardDataHand;

	// 덱에 있는 카드 데이터 (스폰 시 노출)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Data", meta=(ExposeOnSpawn="true"))
	FCard CardDataDeck;

	// 기본 카드 데이터
	UPROPERTY(BlueprintReadWrite, Category = "Card|Data")
	FCard CardDataBase;

	// Pile 상태의 카드 데이터
	UPROPERTY(BlueprintReadWrite, Category = "Card|Data")
	FCard CardDataPile;

	// 카드 상태 및 진행 관련 멤버 변수
public:
	// 카드 효과의 반복 횟수
	UPROPERTY(BlueprintReadWrite, Category = "Card|State")
	int32 CurrentRepetitions;

	// 카드 효과 루프의 현재 인덱스
	UPROPERTY(BlueprintReadWrite, Category = "Card|State")
	int32 EffectLoopIndex;

	// 타겟 루프의 현재 인덱스
	UPROPERTY(BlueprintReadWrite, Category = "Card|State")
	int32 TargetLoopIndex;

	// 현재 유효한 타겟 리스트
	UPROPERTY(BlueprintReadWrite, Category = "Card|State")
	TArray<AActor*> CurrentValidTargets;

	// 카드 사용 중단 플래그
	UPROPERTY(BlueprintReadWrite, Category = "Card|State")
	bool bInterrupt;


	// 카드 특성 관련 멤버 변수
public:
	// 현재 카드가 속한 Pile
	UPROPERTY(BlueprintReadWrite, Category = "Card|Attributes")
	FGameplayTag CurrentPile;

	// 카드의 희귀도
	UPROPERTY(BlueprintReadWrite, Category = "Card|Attributes")
	FGameplayTag Rarity;

	// 카드의 타입 (공격, 스킬 등)
	UPROPERTY(BlueprintReadWrite, Category = "Card|Attributes")
	FGameplayTag CardType;

	// 사용 규칙 관련 멤버 변수
public:
	// 카드 사용 규칙을 관리하는 컴포넌트 맵
	UPROPERTY(BlueprintReadWrite, Category = "Card|UseRules")
	TMap<TSubclassOf<UUseRuleComponent>, UUseRuleComponent*> UseRuleInstances;


	// 이벤트 및 델리게이트
public:
	// 카드가 수정되었을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Card|Delegate")
	FOnThisCardModified OnThisCardModified;
};
