#include "CardSystem/CardBase.h"

#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardSystem/CardActions/Action_Effect.h"
#include "CombatSystem/CardUseRules/UseRuleComponent.h"
#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "CombatSystem/TargetSystem/Targeting_UnTargetedComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Core/GameplayTagComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Utility.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


ACardBase::ACardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	DispatcherHubLocal = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubLocal"));

	
}

void ACardBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeFromData();
}

void ACardBase::InitializeFromData()
{
	// 지정 타입의 RowHandle(FCard 정보 들어있음)을 가져옵니다.
	const FDataTableRowHandle CardDataRowHandleStruct = GetCardDataRowHandle(ECardDataType::Deck);

	// 받아온 데이터가 유효한지 확인합니다.
	if(!CardDataRowHandleStruct.IsNull())
	{
		FCard* CardFound = CardDataRowHandleStruct.DataTable->FindRow<FCard>(CardDataRowHandleStruct.RowName,TEXT("FCard in ACardBase"));

		// 받아온 정보를 CardDataBase와 CardDataDeck에 캐싱합니다.
		if(CardFound)
		{
			CardDataBase = *CardFound;

			if(GetCardName(ECardDataType::Deck).IsEmpty())
			{
				CardDataDeck = CardDataBase;
			}
		}
	}

	// CardDataRowHandleStruct이 유효하지 않고 CardFound가 유효하지 않으면(받아온 정보가 유효하지 않으면)
	// 둘 다 유효하다면 위에서 설정해준 상태이다.
	if(GetCardName(ECardDataType::Deck).IsEmpty())
	{
		COS_LOG_SCREEN(TEXT("ERROR: Attempted to initialize invalid card from data table (%s) with name (%s)"),*CardDataRowHandleStruct.DataTable.GetName(),*CardDataRowHandleStruct.RowName.ToString());
		
		return;
	}

	// 받아온 정보가 유효하지 않지만 카드 이름은 있는 경우 받아온 정보를 일단 CardDataBase에 넣어줍니다.
	CardDataBase = CardDataDeck;

	// 카드 Owner ID를 찾아서 설정해줍니다.
	if(IsValid(GetOwner()))
	{
		if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			COS_LOG_SCREEN(TEXT("Owner가 Interface_CardTarget를 상속받지 않았습니다."));
			return;
		}

		FString UniqueID = IInterface_CardTarget::Execute_GetMinionUniqueID(GetOwner());

		CardDataDeck.OwnerID = UniqueID;
	}

	// 나머지 카드 데이터 타입에도 받은 정보를 넣어줍니다.
	CardDataPile = CardDataDeck;
	CardDataHand = CardDataDeck;

	// 컴포넌트의 GameplayTags 변수에 Hand타입 카드 태그들을 넣어줍니다.
	GameplayTagComponent->GameplayTags = GetCardTags(ECardDataType::Hand);

	// 카드의 UseRule들을 가져옵니다.
	TArray<FUseRule> UseRulesArray = GetCardUseRules(ECardDataType::Hand);

	// 카드의 UseRule을 순회하면서 규칙에 해당하는 컴포넌트들을 생성해줍니다.
	for (const FUseRule& Rule : UseRulesArray)
	{
		UUseRuleComponent* RuleComponent = NewObject<UUseRuleComponent>(this, Rule.Rule);
		if(!RuleComponent)
		{
			COS_LOG_SCREEN(TEXT("CardBase내의 RuleComponent가 올바르게 생성되지 않았습니다."));
			return;
		}
		RuleComponent->RegisterComponent();

		// 이 카드의 UseRule을 관리하는 UseRuleInstance에 추가해줍니다.
		UseRuleInstances.Add(Rule.Rule,RuleComponent);
	}

	// 카드의 FStatusData들을 가져옵니다.
	TArray<FStatusData> CardStartingStatuses = GetCardStartingStatuses(ECardDataType::Hand);

	// FStatusData를 순회하면서 해당하는 컴포넌트들을 생성해줍니다.
	for (const FStatusData& CardStarting : CardStartingStatuses)
	{
		UStatusComponent* StatusComponent = NewObject<UStatusComponent>(this,CardStarting.StatusClass);

		if(!StatusComponent)
		{
			COS_LOG_SCREEN(TEXT("CardBase내의 StatusComponent가 올바르게 생성되지 않았습니다."));
			return;
		}

		StatusComponent->RegisterComponent();
	}
	
	SetCardRarityFromTags();

	SetCardTypeFromTags();
}

bool ACardBase::AttemptUseCard(TArray<AActor*> Targets, bool SkipPlayableCheck, bool SkipConsequences, bool AutoPlay)
{
	InputTargets = Targets;

	// CheckIfPlayable을 뛰어넘고 바로 UseCard
	if (SkipPlayableCheck)
	{
		UseCard(SkipConsequences, AutoPlay);
		return true;
	}

	// CheckIfPlayable로 가능 여부 확인 후 UseCard
	FString FailMessage;
	if (CheckIfPlayable(FailMessage))
	{
		UseCard(SkipConsequences, AutoPlay);
		return true;
	}

	// 사용불가능한 상태이므로 에러메시지 출력
	UFunctionLibrary_Utility::SendScreenLogMessage(FText::FromString(FailMessage), FColor::Red);
	return false;
}

void ACardBase::UseCard(bool bSkipConsequences, bool bAutoPlay)
{
	// 카드를 사용하는 이벤트를 전역 디스패처에 등록
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_UseCard, this, nullptr);

	// 자동 재생일 경우 AutoPlay 플래그를 태그에 추가
	FGameplayTagContainer CallTags;
	if (bAutoPlay)
	{
		CallTags.AddTag(CosGameTags::Flag_AutoPlayed);
	}

	// PrePlay 이벤트 호출
	DispatcherHubLocal->CallEventWithCallTags(CosGameTags::Event_Card_PrePlay, this, nullptr, ECallGlobal::CallAfter, CallTags);

	// 카드 사용 후 결과 처리
	if (!bSkipConsequences)
	{
		ResolveUseRuleConsequences();
	}

	// 반복 작업 초기화 및 다음 반복으로 진행
	CurrentRepetitions = -1;
	ContinueToNextRepetition();
}

bool ACardBase::CheckIfPlayable(FString& OutFailMessage)
{
	// 카드의 멤버변수에서 CardDataHand의 Rule을 가져온다.
	TArray<FUseRule> Rules = GetCardUseRules(ECardDataType::Hand);

	for (const FUseRule& Rule : Rules)
	{
		if (!UseRuleInstances.Contains(Rule.Rule))
			continue;

		// 사용해도 되는지 확인한다.
		// 각 UseRuleComponent를 상속받은 객체들의 재정의된 내용이 실행된다.
		// TODO : 아직 상속받은 컴포넌트의 내용 정의 안됨
		// 예를 들어 StatCost에서는 이 카드의 지정된 Cost가 CardPlayer의 CostValue보다 낮아야만 사용가능이 된다.
		if (!UseRuleInstances[Rule.Rule]->CheckIfUseAllowed(Rule, OutFailMessage))
		{
			return false;
		}
	}

	OutFailMessage = TEXT("");
	return true;
}

TArray<FUseRule> ACardBase::GetCardUseRules(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).UseRules;
}

void ACardBase::ResolveUseRuleConsequences()
{
	TArray<FUseRule> UseRules = GetCardUseRules(ECardDataType::Hand);

	for (const FUseRule& UseRule : UseRules)
	{
		// UseRuleComponent 상속받은 객체 내부에 정의된 내용을 호출한다.
		// 예를 들어 StatCost에서는 카드를 사용하므로서 소모한 마나를 빼는 작업을 한다.
		if (UseRuleInstances.Contains(UseRule.Rule))
		{
			UseRuleInstances[UseRule.Rule]->ResolveUseConsequence(UseRule);
		}
	}
}

void ACardBase::ContinueToNextRepetition()
{
	// 반복 횟수 증가
	++CurrentRepetitions;

	// 카드의 최대 반복 횟수를 초과하면 종료
	if (CurrentRepetitions >= GetCardRepetitions(ECardDataType::Hand))
	{
		// 반복 횟수가 1회 이상이면 다음 효과로 진행
		if (CurrentRepetitions > 0)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
				{
					EffectLoopIndex = -1;
					ContinueToNextEffect();
				}
			));
		}
		else
		{
			EffectLoopIndex = -1;
			ContinueToNextEffect();
		}
	}
	else
	{
		EndCardUse();
	}
}

int32 ACardBase::GetCardRepetitions(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).Repetitions;
}

void ACardBase::EndCardUse()
{
	DispatcherHubLocal->CallEvent(CosGameTags::Event_Card_PostPlay, this);
	DispatcherHubLocal->CallEvent(GetPostUseEvent(ECardDataType::Hand), this);
}

void ACardBase::ContinueToNextEffect()
{
	// EffectLoopIndex 증가
	++EffectLoopIndex;

	// 카드 효과가 범위를 벗어나지 않으면 진행
	const TArray<FCardEffect>& CardEffects = GetCardEffects(ECardDataType::Hand);
	if (EffectLoopIndex < CardEffects.Num())
	{
		// TargetLoopIndex 초기화
		TargetLoopIndex = -1;

		// 현재 카드 효과 가져오기
		const FCardEffect& LoopCardEffect = CardEffects[EffectLoopIndex];

		// TargetingComponent 가져오기
		CurrentTargeting = AccessTargetingClassLazy(GetOwner(), LoopCardEffect.Targeting);

		// 타겟 입력이 필요한 경우 처리
		if (CurrentTargeting->bRequiresInput)
		{
			CurrentTargeting->OnInputTargetsReceived.AddUniqueDynamic(this, &ACardBase::ProceedOnInputTargetsReceived);

			// 유효한 타겟이 없으면 다음 타겟으로 진행
			TArray<AActor*> ValidTargets;
			if (!CurrentTargeting->FindValidTargets(InputTargets, LoopCardEffect, this, false, ValidTargets))
			{
				ContinueToNextTarget();
			}
		}
		else
		{
			// 타겟 입력이 필요 없는 경우 바로 유효한 타겟 찾기
			CurrentTargeting->FindValidTargets(InputTargets, LoopCardEffect, this, false, CurrentValidTargets);
			ContinueToNextTarget();
		}
	}
	else
	{
		// 범위를 벗어나면 다음 반복으로 진행
		ContinueToNextRepetition();
	}
}

void ACardBase::ContinueToNextTarget()
{
	++TargetLoopIndex;

	// 중단 상태일 경우 중단 처리
	if (bInterrupt)
	{
		bInterrupt = false;
		EndCardUse();
		return;
	}

	// 모든 타겟을 처리한 경우 다음 효과로 넘어간다
	if (CurrentValidTargets.Num() - 1 < TargetLoopIndex)
	{
		ContinueToNextEffect();
		return;
	}

	// 현재 처리 중인 카드 효과를 가져온다
	TempCardEffect = GetCardEffects(ECardDataType::Hand)[EffectLoopIndex];

	// 카드 효과 컴포넌트를 생성 및 초기화
	InitializeCurrentCardEffect(TempCardEffect);

	// 즉시 실행 카드 효과 처리
	if (CurrentCardEffect->bImmediated)
	{
		HandleImmediateCardEffect();
	}
	else
	{
		// 카드 효과가 즉시 실행되지 않는 경우
		CurrentCardEffect->OnCardResolved.AddUniqueDynamic(this, &ACardBase::ContinueAfterCardResolved);
	}
}

FCard ACardBase::GetCardDataByCardDataType(ECardDataType Type)
{
	// 각 ECardDataType에 대응하는 FCard를 저장하는 맵
	TMap<ECardDataType, FCard> CardDataMap =
	{
		{ECardDataType::Base, CardDataBase},
		{ECardDataType::Deck, CardDataDeck},
		{ECardDataType::Hand, CardDataHand},
		{ECardDataType::Pile, CardDataPile}
	};

	// 유효한 카드 타입이 있는지 확인 후 반환, 없을 경우 기본값 반환
	if (CardDataMap.Contains(Type))
	{
		return CardDataMap[Type];
	}

	// 잘못된 Type에 대해 기본값 반환 (에러 처리가 필요한 경우 추가)
	return FCard(); // 혹은 적절한 기본 FCard 객체
}

FDataTableRowHandle ACardBase::GetCardDataRowHandle(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).DataRow;
}

FText ACardBase::GetCardName(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).CardName;
}

FGameplayTagContainer ACardBase::GetCardTags(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).CardTags;
}

TArray<FStatusData> ACardBase::GetCardStartingStatuses(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).StartingStatuses;
}

FGameplayTag ACardBase::GetPostUseEvent(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).PostUseEvent;
}

TArray<FCardEffect> ACardBase::GetCardEffects(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).CardEffects;
}

UTargetingComponent* ACardBase::AccessTargetingClassLazy(AActor* TargetingHolderActor,
                                                         TSubclassOf<UTargetingComponent> TargetingClass)
{
	// 외부에서는 Owner가 들어오게 된다.
	// TargetingHolderActor가 유효하지 않으면
	if (!TargetingHolderActor)
	{
		// 자기 자신을 Holder로 설정해서 접근
		return AccessTargetingClassLazy(this, TargetingClass);
	}

	// TargetingHolderActor에 있는 Targeting Component가 유효하면 반환
	if (UTargetingComponent* TargetingComponent = Cast<UTargetingComponent>(TargetingHolderActor->GetComponentByClass(TargetingClass)))
	{
		return TargetingComponent;
	}

	// Holder는 유효하지만 TargetingComponent가 유효하지 않으면 
	// 건네받은 TargetingClass로 TargetingComponent를 생성해서 TargetingHolderActor에 부착시킨다.
	if (IsValid(TargetingClass))
	{
		// Holder에 더해주고 만든 것 반환
		UTargetingComponent* TargetingComponent = NewObject<UTargetingComponent>(TargetingHolderActor);
		TargetingComponent->RegisterComponent();

		return TargetingComponent;
	}

	// 건네받은 TargetingClass도 유효하지 않으면 UTargeting_UnTargetedComponent를 클래스로 해서 다시 접근
	return AccessTargetingClassLazy(TargetingHolderActor, UTargeting_UnTargetedComponent::StaticClass());
}

void ACardBase::ProceedOnInputTargetsReceived(TArray<AActor*> Targets)
{
	CurrentValidTargets = Targets;
	CurrentTargeting->OnInputTargetsReceived.RemoveDynamic(this, &ACardBase::ProceedOnInputTargetsReceived);
	ContinueToNextTarget();
}

void ACardBase::ContinueAfterCardResolved()
{
	CurrentCardEffect->DestroyComponent();
	ContinueToNextTarget();
}

void ACardBase::QueueCardEffectAction(AActor* TargetActor, AActor* SourcePuppet, UCardEffectComponent* CardEffect,
                                      bool bAnimateSourcePuppet)
{
	if (!IsValid(CardEffect->EffectAction))
		return;
	AAction_Effect* ActionEffect = GetWorld()->SpawnActorDeferred<AAction_Effect>(CardEffect->EffectAction,
	                                                                              FTransform::Identity, nullptr,
	                                                                              nullptr, ESpawnActorCollisionHandlingMethod::Undefined, ESpawnActorScaleMethod::SelectDefaultAtRuntime);
	if (!CardEffect)
	{
		COS_LOG_SCREEN(TEXT("ACardBase : ActionEffect가 생성되지 않습니다."));
		return;
	}

	ActionEffect->Target = TargetActor;
	ActionEffect->SourcePuppet = SourcePuppet;
	if (bAnimateSourcePuppet)
		ActionEffect->HeroAnim = CardEffect->HeroAnim;
	ActionEffect->Effect = GetCardEffects(ECardDataType::Hand)[EffectLoopIndex];

	ActionEffect->FinishSpawning(FTransform::Identity);
}

FGameplayTag ACardBase::SetCardRarityFromTags()
{
	FGameplayTagContainer CardTagsContainer = GetCardTags(ECardDataType::Base);
	
	Rarity = CosGameTags::Rarity_Invalid;

	TArray<FGameplayTag> LocalRarities = {CosGameTags::Rarity_Common,CosGameTags::Rarity_Curse,CosGameTags::Rarity_Epic,CosGameTags::Rarity_Invalid, CosGameTags::Rarity_Legendary,CosGameTags::Rarity_Rare};
	
	for (FGameplayTag LocalRarity : LocalRarities)
	{
		if(CardTagsContainer.HasTag(LocalRarity))
		{
			Rarity = LocalRarity;

			return Rarity;
		}
	}

	return Rarity;
}

FGameplayTag ACardBase::SetCardTypeFromTags()
{
	TArray<FGameplayTag> LocalTypes = {CosGameTags::Effect_Attack,CosGameTags::Effect_Curse,CosGameTags::Effect_Power,CosGameTags::Effect_Skill};

	FGameplayTagContainer CardTags = GetCardTags(ECardDataType::Base);

	CardType =  CosGameTags::Effect_Invalid;
	
	for (FGameplayTag LocalType : LocalTypes)
	{
		if(CardTags.HasTag(LocalType))
		{
			CardType = LocalType;
			return CardType;
		}
	}

	return CardType;
}

FGameplayTagContainer ACardBase::GetGameplayTags()
{
	return GameplayTagComponent->GameplayTags;
}

FText ACardBase::GetCardDescription(ECardDataType InCardDataType)
{
	return GetCardDataByCardDataType(InCardDataType).Description;
}

void ACardBase::InitializeCurrentCardEffect(const FCardEffect& CardEffect)
{
	// 새로운 카드 효과 컴포넌트를 생성하고 필요한 정보를 설정
	UCardEffectComponent* NewCardEffectComponent = NewObject<UCardEffectComponent>(this, CardEffect.EffectClass);
	NewCardEffectComponent->RegisterComponent();

	CurrentCardEffect = NewCardEffectComponent;
	CurrentCardEffect->EffectValue = CardEffect.EffectValue;
	CurrentCardEffect->TargetComponent = CardEffect.TargetComponent;
	CurrentCardEffect->ParentCard = this;
	CurrentCardEffect->GameplayTags = CardEffect.GameplayTags;
	CurrentCardEffect->TargetingClass = CardEffect.Targeting;
	CurrentCardEffect->HeroAnim = CardEffect.HeroAnim;
	CurrentCardEffect->EffectAction = CardEffect.EffectAction;
	CurrentCardEffect->UsedData = CardEffect.UsedData;
	CurrentCardEffect->Identifier = CardEffect.Identifier;
}

void ACardBase::HandleImmediateCardEffect()
{
	// 카드 효과의 액션이 유효한지 확인
	if (IsValid(CurrentCardEffect->EffectAction))
	{
		ExecuteEffectAction();
	}

	// 카드 효과를 처리하고, 즉시 후속 작업으로 넘어간다
	CurrentCardEffect->ResolveCardEffect();
	if (CurrentCardEffect->bImmediated)
	{
		ContinueAfterCardResolved();
	}
}

void ACardBase::ExecuteEffectAction()
{
	// 소스 퍼펫을 가져오고, 타겟 액터에 카드 효과 적용
	if (!GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("소유자는 UInterface_CardTarget을 구현하지 않습니다."));
		return;
	}

	AActor* SourcePuppet = IInterface_CardTarget::Execute_GetPuppet(GetOwner());
	AActor* TargetActor = GetValidTargetPuppet(CurrentValidTargets[TargetLoopIndex]);

	if (!TargetActor)
	{
		COS_LOG_SCREEN(TEXT("대상이 UInterface_CardTarget을 구현하지 않습니다."));
		return;
	}

	// 카드 효과 액션을 큐에 추가
	QueueCardEffectAction(TargetActor, SourcePuppet, CurrentCardEffect, TargetLoopIndex == 0);
}

AActor* ACardBase::GetValidTargetPuppet(AActor* TargetActor) const
{
	// 타겟 액터가 UInterface_CardTarget을 구현하는지 확인하고, 퍼펫을 반환
	if (TargetActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		return IInterface_CardTarget::Execute_GetPuppet(TargetActor);
	}

	return nullptr;
}

void ACardBase::Interrupt_Implementation()
{
	bInterrupt = true;
}

void ACardBase::CancelInterruption_Implementation()
{
	bInterrupt = false;
}
