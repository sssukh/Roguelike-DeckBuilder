#include "CardSystem/CardBase.h"

#include "Core/DispatcherHubComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Core/GameplayTagComponent.h"

#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Utility.h"

#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

#include "StatusSystem/StatusComponent.h"

#include "BlueprintGameplayTagLibrary.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "ActionSystem/Action_Effect.h"
#include "CardSystem/CardUseRules/UseRuleComponent.h"

#include "CardSystem/CardUseRules/UseRule_StatCost.h"
#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "CombatSystem/TargetSystem/TargetingComponent_Untargeted.h"


ACardBase::ACardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	DispatcherHubLocal = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubLocal"));

	CardType = CosGameTags::Effect_Invalid;
}

void ACardBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeFromData();
}

void ACardBase::InitializeFromData()
{
	// 1. Deck 카드 데이터를 가져옵니다.
	const FDataTableRowHandle CardDataRowHandle = GetCardDataRowHandle(ECardDataType::Deck);

	// 2. 유효한 데이터가 아니면 초기화를 중단합니다.
	if (!InitializeCardDataFromRow(CardDataRowHandle))
	{
		return;
	}

	// 3. 카드의 소유자가 유효하고 IInterface_CardTarget 인터페이스를 구현하는지 확인합니다.
	if (IsValid(GetOwner()) && GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		// 4. 소유자의 고유 ID를 가져와 Deck 카드 데이터에 설정합니다.
		FString OwnerUniqueID = IInterface_CardTarget::Execute_GetMinionUniqueID(GetOwner());
		CardDataDeck.OwnerID = OwnerUniqueID;
	}

	// 5. 카드 데이터를 초기화하고 사용 규칙과 상태 컴포넌트를 설정합니다.
	InitializeCardData();
	SetupUseRuleComponents();
	SetupStatusComponents();

	// 6. 카드의 희귀도와 타입을 태그로부터 설정합니다.
	SetCardRarityFromTags();
	SetCardTypeFromTags();
}

bool ACardBase::InitializeCardDataFromRow(const FDataTableRowHandle& CardDataRowHandle)
{
	// 1. 데이터 핸들이 유효한지 확인합니다.
	if (CardDataRowHandle.IsNull())
	{
		return HandleInvalidCardData(CardDataRowHandle);
	}

	// 2. 데이터 테이블에서 카드 데이터를 찾습니다.
	const FCard* FoundCard = CardDataRowHandle.DataTable->FindRow<FCard>(CardDataRowHandle.RowName, TEXT("FCard in ACardBase"));

	// 3. 유효한 카드 데이터를 찾은 경우, 기본 카드 데이터를 설정합니다.
	if (FoundCard)
	{
		CardDataBase = *FoundCard;

		// 4. Deck 카드 이름이 비어있으면 기본 데이터를 CardDataDeck에 복사합니다.
		if (GetCardName(ECardDataType::Deck).IsEmpty())
		{
			CardDataDeck = CardDataBase;
		}
	}
	else
	{
		// 5. 카드가 없을 경우, 에러 처리를 수행하고 false를 반환합니다.
		return HandleInvalidCardData(CardDataRowHandle);
	}

	return true;
}

bool ACardBase::HandleInvalidCardData(const FDataTableRowHandle& CardDataRowHandle)
{
	// 1. Deck 카드 이름이 비어있으면 에러 로그를 출력합니다.
	if (GetCardName(ECardDataType::Deck).IsEmpty())
	{
		COS_SCREEN(TEXT("오류: 데이터 테이블(%s)에서 이름(%s)의 잘못된 카드를 초기화하려고 시도했습니다."),
		           *CardDataRowHandle.DataTable->GetName(), *CardDataRowHandle.RowName.ToString());
		return false; // 유효하지 않은 데이터로 초기화 실패
	}

	// 2. Deck 카드 이름이 유효하면 기본 카드 데이터를 설정합니다.
	CardDataBase = CardDataDeck;
	return true;
}

void ACardBase::InitializeCardData()
{
	// 1. Deck 데이터를 기준으로 Pile과 Hand 데이터를 초기화합니다.
	CardDataPile = CardDataDeck;
	CardDataHand = CardDataDeck;

	// 2. 카드의 GameplayTags를 Hand 데이터로부터 설정합니다.
	GameplayTagComponent->GameplayTags = GetCardTags(ECardDataType::Hand);
}

void ACardBase::SetupUseRuleComponents()
{
	// 1. 카드의 Hand 데이터 타입에서 사용 규칙을 가져옵니다.
	TArray<FUseRule> UseRulesArray = GetCardUseRules(ECardDataType::Hand);

	// 2. 각 사용 규칙에 대해 컴포넌트를 생성하고 초기화합니다.
	for (const FUseRule& UseRule : UseRulesArray)
	{
		// 3. 고유한 컴포넌트 이름을 생성합니다.
		FName UniqueComponentName = MakeUniqueObjectName(this, UUseRuleComponent::StaticClass());

		// 4. 새로운 UUseRuleComponent를 생성하고, 해당 규칙에 맞게 설정합니다.
		UUseRuleComponent* NewUseRuleComponent = NewObject<UUseRuleComponent>(this, UseRule.Rule, UniqueComponentName);

		// 5. 컴포넌트를 등록하고 초기화합니다.
		NewUseRuleComponent->RegisterComponent();
		NewUseRuleComponent->InitializeUseRule();
		AddInstanceComponent(NewUseRuleComponent);

		// 6. 생성된 컴포넌트를 사용 규칙 인스턴스에 추가합니다.
		UseRuleInstances.Add(UseRule.Rule, NewUseRuleComponent);
	}
}

void ACardBase::SetupStatusComponents()
{
	// 1. 카드의 Hand 데이터 타입에서 초기 상태 정보를 가져옵니다.
	TArray<FStatusData> StartingStatusArray = GetCardStartingStatuses(ECardDataType::Hand);

	// 2. 각 상태에 대해 컴포넌트를 생성하고 초기화합니다.
	for (const FStatusData& StatusData : StartingStatusArray)
	{
		// 3. 고유한 상태 컴포넌트 이름을 생성합니다.
		FName UniqueStatusComponentName = MakeUniqueObjectName(this, UStatusComponent::StaticClass());

		// 4. 새로운 UStatusComponent를 생성하고, 상태 값과 태그를 설정합니다.
		UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(this, StatusData.StatusClass, UniqueStatusComponentName);

		NewStatusComponent->RegisterComponent();
		NewStatusComponent->StatusValue = StatusData.Value;
		NewStatusComponent->GameplayTags = StatusData.GameplayTags;
		AddInstanceComponent(NewStatusComponent);
	}
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
	UFunctionLibrary_Utility::SendScreenLogMessage(this, FText::FromString(FailMessage), FColor::Red);
	return false;
}

bool ACardBase::AttemptUseCardUnTargeted(bool bSkipPlayableCheck, bool bSkipConsequences, bool bAutoPlay)
{
	// 타겟이 없는(UnTargeted) 상태를 나타내기 위해 null 타겟을 설정.
	TArray<AActor*> Targets;
	Targets.Add(nullptr); // 타겟이 필요하지 않음을 의미.

	// AttemptUseCard 함수 호출, 타겟 없이 카드를 사용하려 시도.
	return AttemptUseCard(Targets, bSkipPlayableCheck, bSkipConsequences, bAutoPlay);
}

void ACardBase::UseCard(bool bSkipConsequences, bool bAutoPlay)
{
	// 1. 카드를 사용했다는 이벤트를 전역 디스패처에 등록합니다.
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_UseCard, this, nullptr);

	// 2. 자동 재생일 경우, AutoPlay 태그를 추가하여 이를 알립니다.
	FGameplayTagContainer CallTags;
	if (bAutoPlay)
	{
		CallTags.AddTag(CosGameTags::Flag_AutoPlayed);
	}

	// 3. 카드가 사용되기 전의 PrePlay 이벤트를 호출합니다.
	DispatcherHubLocal->CallEventWithCallTags(CosGameTags::Event_Card_PrePlay, this, nullptr, ECallGlobal::CallAfter, CallTags);

	// 4. 결과 처리를 건너뛰지 않는 경우, 카드 사용에 따른 결과를 처리합니다.
	if (!bSkipConsequences)
	{
		ResolveUseRuleConsequences();
	}

	// 5. 반복 작업을 초기화하고, 다음 반복을 처리할 준비를 합니다.
	CurrentRepetitions = -1;
	ContinueToNextRepetition();
}

void ACardBase::EndCardUse()
{
	DispatcherHubLocal->CallEvent(CosGameTags::Event_Card_PostPlay, this);
	DispatcherHubLocal->CallEvent(GetPostUseEvent(ECardDataType::Hand), this);
}

bool ACardBase::CheckIfPlayable(FString& OutFailMessage)
{
	// 1. 카드의 Hand 데이터에 해당하는 사용 규칙을 가져옵니다.
	TArray<FUseRule> UseRules = GetCardUseRules(ECardDataType::Hand);

	// 2. 각 사용 규칙을 순회하며, 해당 규칙을 충족하는지 확인합니다.
	for (const FUseRule& CurrentRule : UseRules)
	{
		// 3. 해당 규칙에 대한 컴포넌트가 존재하는지 확인합니다.
		if (!UseRuleInstances.Contains(CurrentRule.Rule))
			continue;

		// 4. 규칙을 충족하지 못하면 실패 메시지를 반환하고 false를 반환합니다.
		if (!UseRuleInstances[CurrentRule.Rule]->CheckIfUseAllowed(CurrentRule, OutFailMessage))
		{
			return false;
		}
	}

	// 5. 모든 규칙을 통과하면 빈 메시지와 함께 true를 반환합니다.
	OutFailMessage = TEXT("");
	return true;
}

void ACardBase::ResolveUseRuleConsequences()
{
	// 1. Hand 데이터 타입의 카드에 대한 사용 규칙을 가져옵니다.
	TArray<FUseRule> CardUseRules = GetCardUseRules(ECardDataType::Hand);

	// 2. 각 사용 규칙을 순회하며 규칙에 따른 결과를 적용합니다.
	for (const FUseRule& UseRule : CardUseRules)
	{
		// 3. 해당 규칙에 대한 컴포넌트가 있는지 확인합니다.
		if (UseRuleInstances.Contains(UseRule.Rule))
		{
			// 4. 규칙에 정의된 결과를 처리합니다.
			UseRuleInstances[UseRule.Rule]->ResolveUseConsequence(UseRule);
		}
	}
}

void ACardBase::ContinueToNextRepetition()
{
	// 1. 현재 반복 횟수를 증가시킵니다.
	++CurrentRepetitions;

	// 2. 현재 반복 횟수가 카드에 설정된 최대 반복 횟수보다 작거나 같으면 계속 진행합니다.
	if (CurrentRepetitions <= GetCardRepetitions(ECardDataType::Hand))
	{
		// 3. 반복이 1회 이상일 경우, 다음 틱에서 다음 카드 효과로 진행합니다.
		if (CurrentRepetitions > 0)
		{
			// 다음 틱에서 실행할 함수를 설정합니다.
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
			{
				EffectLoopIndex = -1; // 효과 루프 인덱스를 초기화합니다.
				ContinueToNextEffect(); // 다음 효과로 넘어갑니다.
			}));
		}
		else
		{
			// 4. 반복이 0일 경우, 바로 다음 카드 효과로 진행합니다.
			EffectLoopIndex = -1;
			ContinueToNextEffect();
		}
	}
	else
	{
		// 5. 반복이 모두 완료되었으면 카드 사용을 종료합니다.
		EndCardUse();
	}
}

void ACardBase::ContinueToNextEffect()
{
	// 1. 현재 카드 효과 인덱스를 증가시킵니다.
	++EffectLoopIndex;

	// 2. Hand 데이터 타입에 정의된 카드 효과들을 가져옵니다.
	const TArray<FCardEffect>& HandCardEffects = GetCardEffects(ECardDataType::Hand);

	// 3. 카드 효과 인덱스가 카드 효과 리스트의 범위 내에 있는지 확인합니다.
	int32 LastEffectIndex = HandCardEffects.Num() - 1;
	if (EffectLoopIndex <= LastEffectIndex)
	{
		// 4. 타겟 인덱스를 초기화합니다.
		TargetLoopIndex = -1;

		// 5. 현재 처리 중인 카드 효과를 가져옵니다.
		const FCardEffect& HandCurrentCardEffect = HandCardEffects[EffectLoopIndex];

		// 6. 현재 카드 효과에 맞는 타겟팅 컴포넌트를 가져옵니다.
		CurrentTargetingComponent = AccessTargetingClassLazy(GetOwner(), HandCurrentCardEffect.Targeting);

		// 7. 타겟 입력이 필요한 경우 처리합니다.
		if (CurrentTargetingComponent->bRequiresInput)
		{			
			// 타겟 입력을 받기 위한 델리게이트를 추가합니다.
			CurrentTargetingComponent->OnInputTargetsReceived.AddUniqueDynamic(this, &ACardBase::ProceedOnInputTargetsReceived);

			// 유효한 타겟이 없으면 다음 타겟으로 진행합니다.
			TArray<AActor*> ValidTargets;
			if (!CurrentTargetingComponent->FindValidTargets(InputTargets, HandCurrentCardEffect, this, false, ValidTargets))
			{
				ContinueToNextTarget();
			}
		}
		else
		{
			// 8. 타겟 입력이 필요하지 않으면 바로 유효한 타겟을 찾습니다.
			CurrentTargetingComponent->FindValidTargets(InputTargets, HandCurrentCardEffect, this, false, CurrentValidTargets);
			ContinueToNextTarget();
		}
	}
	else
	{
		// 9. 모든 카드 효과를 처리한 경우 다음 반복으로 진행합니다.
		ContinueToNextRepetition();
	}
}

void ACardBase::ContinueToNextTarget()
{
	++TargetLoopIndex;

	// 카드 사용이 중단(interrupted)된 경우, 카드 사용을 종료하고 함수 반환.
	if (bInterrupt)
	{
		bInterrupt = false; // 중단 상태를 해제.
		EndCardUse(); // 카드 사용을 종료.
		return; // 함수 종료.
	}

	// 모든 타겟을 처리한 경우 다음 효과로 넘어간다
	if (CurrentValidTargets.Num() - 1 < TargetLoopIndex)
	{
		ContinueToNextEffect();
		return;
	}

	// 현재 처리 중인 카드 효과를 가져온다
	FCardEffect TempCardEffect = GetCardEffects(ECardDataType::Hand)[EffectLoopIndex];

	// 카드 효과 컴포넌트를 생성 및 초기화
	InitializeCurrentCardEffect(TempCardEffect);

	// 즉시 실행 카드 효과 처리
	if (CurrentCardEffectComponent->bImmediate)
	{
		HandleImmediateCardEffect();
	}
	else
	{
		// 카드 효과가 즉시 실행되지 않는 경우
		CurrentCardEffectComponent->OnCardResolved.AddUniqueDynamic(this, &ACardBase::ContinueAfterCardResolved);
	}
}

void ACardBase::ProceedOnInputTargetsReceived(TArray<AActor*> Targets)
{
	// 입력된 유효한 타겟들을 저장.
	CurrentValidTargets = Targets;

	// 델리게이트에서 타겟 입력 수신자를 제거하여 중복 호출 방지.
	CurrentTargetingComponent->OnInputTargetsReceived.RemoveDynamic(this, &ACardBase::ProceedOnInputTargetsReceived);

	// 다음 타겟을 처리.
	ContinueToNextTarget();
}

void ACardBase::InitializeCurrentCardEffect(const FCardEffect& CardEffect)
{
	// 카드 효과 컴포넌트를 생성하고 등록.
	UCardEffectComponent* NewCardEffectComponent = NewObject<UCardEffectComponent>(this, CardEffect.EffectClass);
	NewCardEffectComponent->RegisterComponent();

	// 현재 카드 효과를 새로 생성한 컴포넌트로 설정.
	CurrentCardEffectComponent = NewCardEffectComponent;

	// 카드 효과 데이터를 컴포넌트에 설정.
	CurrentCardEffectComponent->EffectValue = CardEffect.EffectValue;
	CurrentCardEffectComponent->TargetComponent = CardEffect.TargetComponent;
	CurrentCardEffectComponent->ParentCard = this;
	CurrentCardEffectComponent->GameplayTags = CardEffect.GameplayTags;
	CurrentCardEffectComponent->TargetingClass = CardEffect.Targeting;
	CurrentCardEffectComponent->HeroAnim = CardEffect.HeroAnim;
	CurrentCardEffectComponent->EffectActionClass = CardEffect.EffectAction;
	CurrentCardEffectComponent->UsedData = CardEffect.UsedData;
	CurrentCardEffectComponent->Identifier = CardEffect.Identifier;

	AddInstanceComponent(NewCardEffectComponent);
}

void ACardBase::HandleImmediateCardEffect()
{
	// 카드 효과의 액션이 유효한지 확인하고 실행.
	if (IsValid(CurrentCardEffectComponent->EffectActionClass))
	{
		ExecuteEffectAction();
	}

	// 타겟에 대한 카드 효과를 처리.
	CurrentCardEffectComponent->ResolveCardEffect(CurrentValidTargets[TargetLoopIndex]);

	// 카드 효과가 즉시 실행되었다면, 후속 처리를 바로 진행.
	if (CurrentCardEffectComponent->bImmediate)
	{
		ContinueAfterCardResolved();
	}
}

void ACardBase::ExecuteEffectAction()
{
	// 소스 퍼펫을 가져오고, 타겟 액터에 카드 효과 적용
	if (!GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		COS_SCREEN(TEXT("소유자는 UInterface_CardTarget을 구현하지 않습니다."));
		return;
	}

	// 소스 퍼펫과 타겟을 가져온다.
	AActor* SourcePuppet = IInterface_CardTarget::Execute_GetPuppet(GetOwner());
	AActor* TargetActor = GetValidTargetPuppet(CurrentValidTargets[TargetLoopIndex]);

	// 유효한 타겟이 없으면 로그를 출력하고 함수 종료.
	if (!TargetActor)
	{
		COS_SCREEN(TEXT("대상이 UInterface_CardTarget을 구현하지 않습니다."));
		return;
	}

	// 카드 효과 액션을 큐에 추가
	QueueCardEffectAction(TargetActor, SourcePuppet, CurrentCardEffectComponent, TargetLoopIndex == 0);
}

AActor* ACardBase::GetValidTargetPuppet(AActor* TargetActor) const
{
	// 타겟 액터가 UInterface_CardTarget을 구현하는지 확인.
	if (TargetActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		// 퍼펫을 반환.
		return IInterface_CardTarget::Execute_GetPuppet(TargetActor);
	}

	// 유효하지 않은 경우 nullptr 반환.
	return nullptr;
}

TArray<AActor*> ACardBase::FindAllPotentialTargets()
{
	// 모든 유효한 타겟들을 저장할 배열입니다.
	TArray<AActor*> AllValidTargets;

	// 1. 카드의 Hand 데이터에 있는 모든 카드 효과를 가져옵니다.
	const TArray<FCardEffect>& CardEffects = GetCardEffects(ECardDataType::Hand);

	// 2. 각 카드 효과에 대해 타겟팅 컴포넌트를 사용하여 유효한 타겟을 찾습니다.
	for (const FCardEffect& CardEffect : CardEffects)
	{
		// 3. 카드 효과에 맞는 타겟팅 컴포넌트를 가져옵니다.
		UTargetingComponent* TargetingComponent = AccessTargetingClassLazy(GetOwner(), CardEffect.Targeting);

		// 4. 현재 카드 효과에 맞는 타겟들을 찾고, 이를 임시 배열에 저장합니다.
		TArray<AActor*> ValidTargets;
		TargetingComponent->FindValidTargets(InputTargets, CardEffect, this, true, ValidTargets);

		// 5. 찾은 타겟들을 전체 유효 타겟 배열에 추가합니다.
		AllValidTargets.Append(ValidTargets);
	}

	// 6. 모든 카드 효과에 대한 타겟들을 포함한 배열을 반환합니다.
	return AllValidTargets;
}

UTargetingComponent* ACardBase::AccessTargetingClassLazy(AActor* TargetingHolderActor, TSubclassOf<UTargetingComponent> TargetingClass)
{
	// 1. TargetingHolderActor가 유효하지 않으면, 자기 자신(this)을 TargetingHolderActor로 설정하여 재귀 호출합니다.
	if (!IsValid(TargetingHolderActor))
	{
		return AccessTargetingClassLazy(this, TargetingClass);
	}

	// 2. TargetingHolderActor에 해당 클래스의 타겟팅 컴포넌트가 있는지 확인하고, 있으면 반환합니다.
	if (UActorComponent* FoundTargetingComponent = TargetingHolderActor->GetComponentByClass(TargetingClass))
	{
		if (UTargetingComponent* TargetingComponent = Cast<UTargetingComponent>(FoundTargetingComponent))
		{
			return TargetingComponent;
		}
	}


	// 3. 타겟팅 컴포넌트가 없고, 주어진 TargetingClass가 유효하면 새로운 타겟팅 컴포넌트를 생성하여 추가합니다.
	if (IsValid(TargetingClass))
	{
		// 새로운 타겟팅 컴포넌트를 생성하고 등록합니다.
		UTargetingComponent* NewTargetingComponent = NewObject<UTargetingComponent>(TargetingHolderActor, TargetingClass);
		NewTargetingComponent->RegisterComponent();
		TargetingHolderActor->AddInstanceComponent(NewTargetingComponent);

		return NewTargetingComponent;
	}

	// 4. TargetingClass가 유효하지 않으면, 기본 타겟팅 클래스(UTargetingComponent_Untargeted)를 사용하여 다시 접근합니다.
	return AccessTargetingClassLazy(TargetingHolderActor, UTargetingComponent_Untargeted::StaticClass());
}

FCard ACardBase::GetCardByCardDataType(ECardDataType InCardType)
{
	if (InCardType == ECardDataType::Base) return CardDataBase;
	if (InCardType == ECardDataType::Deck) return CardDataDeck;
	if (InCardType == ECardDataType::Hand) return CardDataHand;
	if (InCardType == ECardDataType::Pile) return CardDataPile;

	return FCard(); // 기본 FCard 객체를 반환 (에러 처리가 필요한 경우 추가 가능)
}

FCard& ACardBase::GetCardByCardDataTypeRef(ECardDataType InCardType)
{
	// 1. ECardDataType에 해당하는 FCard의 참조를 매핑하는 맵을 생성합니다.
	TMap<ECardDataType, FCard*> CardDataMap =
	{
		{ECardDataType::Base, &CardDataBase},
		{ECardDataType::Deck, &CardDataDeck},
		{ECardDataType::Hand, &CardDataHand},
		{ECardDataType::Pile, &CardDataPile}
	};

	// 2. 주어진 카드 타입에 해당하는 카드 데이터가 맵에 있는지 확인합니다.
	if (CardDataMap.Contains(InCardType))
	{
		// 3. 해당하는 카드 데이터의 참조를 반환합니다.
		return *CardDataMap[InCardType];
	}

	// 4. 유효하지 않은 카드 타입일 경우 기본 FCard 객체의 참조를 반환합니다.
	//    DefaultCard는 static으로 선언하여 참조 반환 시 항상 유효한 메모리 주소를 갖게 합니다.
	static FCard DefaultCard;
	return DefaultCard;
}

FDataTableRowHandle ACardBase::GetCardDataRowHandle(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).DataRow;
}

FText ACardBase::GetCardName(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).CardName;
}

FSlateColor ACardBase::GetCardFrameTint(ECardDataType InCardType)

{
	return GetCardByCardDataType(InCardType).FrameTint;
}

UTexture2D* ACardBase::
GetCardFrame(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).Frame;
}

UTexture2D* ACardBase::GetCardPortrait(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).Portrait;
}

FGameplayTagContainer ACardBase::GetCardTags(ECardDataType Type)
{
	return GetCardByCardDataType(Type).CardTags;
}

TArray<FStatusData> ACardBase::GetCardStartingStatuses(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).StartingStatuses;
}

FGameplayTag ACardBase::GetPostUseEvent(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).PostUseEvent;
}

TArray<FCardEffect> ACardBase::GetCardEffects(ECardDataType InCardType)
{
	return GetCardByCardDataType(InCardType).CardEffects;
}

int32 ACardBase::GetCardRepetitions(ECardDataType Type)
{
	return GetCardByCardDataType(Type).Repetitions;
}

TArray<FUseRule> ACardBase::GetCardUseRules(ECardDataType Type)
{
	return GetCardByCardDataType(Type).UseRules;
}

FText ACardBase::GetCardDescription(ECardDataType InCardDataType)
{
	return GetCardByCardDataType(InCardDataType).Description;
}

FString ACardBase::GetCardOwnerID(ECardDataType InCardDataType)
{
	return GetCardByCardDataType(InCardDataType).OwnerID;
}

FGameplayTag ACardBase::GetCardEndTurnEvent(ECardDataType InCardDataType)
{
	return GetCardByCardDataType(InCardDataType).EndTurnEvent;
}

FGameplayTagContainer ACardBase::GetGameplayTags()
{
	return GameplayTagComponent->GameplayTags;
}

bool ACardBase::GetCardEffectValue(FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, ECardDataType InCardType, int32& OutEffectValue)
{
	// 주어진 카드 타입의 모든 카드 효과를 가져옴.
	TArray<FCardEffect> CardEffects = GetCardEffects(InCardType);

	// 카드 효과 리스트를 순회하며 조건에 맞는 효과를 찾음.
	for (const FCardEffect& CardEffect : CardEffects)
	{
		// PossibleTags가 비어 있거나, 카드 효과가 PossibleTags 중 하나라도 포함하는지 확인.
		bool bMatchesPossibleTags = PossibleTags.IsEmpty() || CardEffect.GameplayTags.HasAnyExact(PossibleTags);

		// RequiredTags가 비어 있거나, 카드 효과가 RequiredTags를 모두 포함하는지 확인.
		bool bMatchesRequiredTags = RequiredTags.IsEmpty() || CardEffect.GameplayTags.HasAllExact(RequiredTags);

		// 조건을 만족하는 경우, 해당 카드 효과 값을 반환.
		if (bMatchesPossibleTags && bMatchesRequiredTags)
		{
			OutEffectValue = CardEffect.EffectValue;
			return true; // 효과 값을 찾았으므로 true를 반환.
		}
	}

	// 조건에 맞는 카드 효과가 없는 경우 0으로 설정하고 false 반환.
	OutEffectValue = 0;
	return false;
}

TArray<FDataTableRowHandle> ACardBase::GetCardTooltips(ECardDataType Type)
{
	return GetCardByCardDataType(Type).Tooltips;
}

TSubclassOf<UUserWidget> ACardBase::GetCardVisualWidget(ECardDataType Type)
{
	return GetCardByCardDataType(Type).CardVisualWidget;
}

bool ACardBase::GetStatusCostValueFromUseRules(ECardDataType InPile, const TSubclassOf<UStatusComponent>& StatusClass, int32& OutStatusCost, int32& OutUseRuleIndex)
{
	// 주어진 카드 타입에 대한 사용 규칙을 가져옴.
	const TArray<FUseRule>& UseRules = GetCardUseRules(InPile);

	// 사용 규칙이 없는 경우, 기본 값 설정 후 false 반환.
	if (UseRules.Num() == 0)
	{
		OutStatusCost = 0;
		OutUseRuleIndex = -1;
		return false;
	}

	// 사용 규칙이 하나만 있는 경우 해당 상태 규칙이 있는지 확인.
	if (UseRules.Num() == 1)
	{
		if (UseRules[0].Status == StatusClass)
		{
			// 상태 규칙이 맞으면 비용과 인덱스 반환.
			OutStatusCost = UseRules[0].Cost;
			OutUseRuleIndex = 0;
			return true;
		}

		// 상태 규칙이 없는 경우 기본 값 반환.
		OutStatusCost = 0;
		OutUseRuleIndex = -1;
		return false;
	}

	// 여러 개의 사용 규칙이 있는 경우 상태 규칙을 순회하며 찾음.
	int32 RuleIndex = 0; // 규칙 인덱스 추적.
	for (const FUseRule& UseRule : UseRules)
	{
		// 주어진 상태 클래스에 해당하는 사용 규칙이 있으면 값 반환.
		if (UseRule.Status == StatusClass)
		{
			OutStatusCost = UseRule.Cost;
			OutUseRuleIndex = RuleIndex;
			return true;
		}
		RuleIndex++;
	}

	// 상태 규칙을 찾지 못한 경우 기본 값 반환.
	OutStatusCost = 0;
	OutUseRuleIndex = -1;
	return false;
}

bool ACardBase::IsInHand()
{
	return CurrentPile == CosGameTags::Pile_Hand;
}

void ACardBase::ContinueAfterCardResolved()
{
	CurrentCardEffectComponent->DestroyComponent();
	ContinueToNextTarget();
}

void ACardBase::QueueCardEffectAction(AActor* TargetActor, AActor* SourcePuppet, UCardEffectComponent* CardEffect, bool bAnimateSourcePuppet)
{
	// 1. 카드 효과에 대한 액션이 유효한지 확인합니다.
	if (!IsValid(CardEffect->EffectActionClass)) return;

	// 2. 카드 효과가 유효한지 다시 확인 (생성 실패시 로그 출력).
	if (!CardEffect) return;

	// 3. AAction_Effect 액터를 생성합니다.
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueActionWithClass<AAction_Effect>(this,CardEffect->EffectActionClass, [this,TargetActor,SourcePuppet,CardEffect,bAnimateSourcePuppet](AAction_Effect* Action_Effect)
	{
		Action_Effect->Target = TargetActor;
		Action_Effect->SourcePuppet = SourcePuppet;

		if (bAnimateSourcePuppet)
			Action_Effect->HeroAnim = CardEffect->HeroAnim;

		const TArray<FCardEffect>& HandCardEffect = GetCardEffects(ECardDataType::Hand);

		if (HandCardEffect.IsValidIndex(EffectLoopIndex))
			Action_Effect->Effect = HandCardEffect[EffectLoopIndex];
	}, ESpawnActorCollisionHandlingMethod::Undefined, ESpawnActorScaleMethod::SelectDefaultAtRuntime);
}

bool ACardBase::BindLocalCardEvents(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	DispatcherHubLocal->BindMultipleEventsToHub(EventHolder, EventTags);
	return true;
}

FGameplayTag ACardBase::SetCardRarityFromTags()
{
	// 1. 카드의 기본 데이터 타입(Base)에 대한 태그들을 가져옵니다.
	FGameplayTagContainer CardTags = GetCardTags(ECardDataType::Base);

	// 2. 희귀도 태그(Rarity)가 카드 태그에 포함되어 있는지 확인합니다.
	if (CardTags.HasTag(CosGameTags::Rarity))
	{
		// 3. 게임에서 사용되는 희귀도 태그 목록을 정의합니다.
		TArray<FGameplayTag> RarityTags = {
			CosGameTags::Rarity_Common,
			CosGameTags::Rarity_Curse,
			CosGameTags::Rarity_Epic,
			CosGameTags::Rarity_Invalid,
			CosGameTags::Rarity_Legendary,
			CosGameTags::Rarity_Rare
		};

		// 4. 카드의 태그 중에서 희귀도 태그를 찾습니다.
		for (FGameplayTag RarityTag : RarityTags)
		{
			// 5. 카드 태그에 정확히 일치하는 희귀도 태그가 있으면 해당 태그를 반환합니다.
			if (CardTags.HasTagExact(RarityTag))
			{
				Rarity = RarityTag;
				return Rarity;
			}
		}
	}

	// 6. 희귀도 태그가 없으면 기본값으로 Rarity_Invalid를 반환합니다.
	return CosGameTags::Rarity_Invalid;
}

FGameplayTag ACardBase::SetCardTypeFromTags()
{
	// 1. 카드 유형을 나타내는 태그 목록을 정의합니다.
	TArray<FGameplayTag> CardEffectTypes =
	{
		CosGameTags::Effect_Attack,
		CosGameTags::Effect_Curse,
		CosGameTags::Effect_Power,
		CosGameTags::Effect_Skill
	};

	// 2. 카드의 기본 데이터 타입(Base)에 대한 태그를 가져옵니다.
	FGameplayTagContainer CardTags = GetCardTags(ECardDataType::Base);

	// 3. 카드에 정확히 일치하는 유형 태그가 있는지 확인합니다.
	for (FGameplayTag EffectTypeTag : CardEffectTypes)
	{
		// 4. 카드에 일치하는 태그가 있으면 해당 태그를 카드의 유형으로 설정하고 반환합니다.
		if (CardTags.HasTagExact(EffectTypeTag))
		{
			CardType = EffectTypeTag;
			return CardType;
		}
	}

	// 5. 해당하는 카드 유형이 없으면 기본값인 Effect_Invalid를 반환합니다.
	return CosGameTags::Effect_Invalid;
}

void ACardBase::SetCardEffects(ECardDataType InCardType, const TArray<FCardEffect>& NewCardEffects)
{
	GetCardByCardDataTypeRef(InCardType).CardEffects = NewCardEffects;
}

void ACardBase::SetCardUseRules(ECardDataType Pile, const TArray<FUseRule>& NewUseRules)
{
	GetCardByCardDataTypeRef(Pile).UseRules = NewUseRules;
}

bool ACardBase::SetUseRuleCost(TSubclassOf<UUseRule_StatCost> UseRule, TSubclassOf<UStatusComponent> Status, int32 NewValue, ECardDataType Pile, bool bAddIfNotFound)
{
	// 주어진 카드 더미에 대한 사용 규칙을 가져옴.
	TArray<FUseRule> CardUseRules = GetCardUseRules(Pile);

	// 사용 규칙이 없고 새 규칙을 추가하지 않는 경우 false 반환.
	if (CardUseRules.Num() == 0)
	{
		if (!bAddIfNotFound)
		{
			return false;
		}

		// 새로운 사용 규칙을 추가.
		TArray<FUseRule> NewUseRules{FUseRule(UseRule, Status, NewValue)};
		SetCardUseRules(Pile, NewUseRules);
		return true;
	}

	// 사용 규칙이 하나만 있을 경우 처리.
	if (CardUseRules.Num() == 1)
	{
		// 해당 규칙이 맞으면 비용을 업데이트.
		if (CardUseRules[0].Rule == UseRule && CardUseRules[0].Status == Status)
		{
			TArray<FUseRule> NewUseRules{FUseRule(UseRule, Status, NewValue)};
			SetCardUseRules(Pile, NewUseRules);
			return true;
		}

		// 규칙이 없고 새로 추가하지 않는 경우 false 반환.
		if (!bAddIfNotFound)
		{
			return false;
		}

		// 규칙을 찾아서 비용을 업데이트.
		for (FUseRule& CardUseRule : CardUseRules)
		{
			if (CardUseRule.Rule == UseRule && CardUseRule.Status == Status)
			{
				CardUseRule.Cost = NewValue;
				SetCardUseRules(Pile, CardUseRules);
				return true;
			}
		}

		// 규칙이 없으면 새 규칙을 추가.
		if (bAddIfNotFound)
		{
			CardUseRules.Add(FUseRule(UseRule, Status, NewValue));
			SetCardUseRules(Pile, CardUseRules);
			return true;
		}

		return false;
	}

	// 여러 사용 규칙이 있을 때 처리.
	for (FUseRule& CardUseRule : CardUseRules)
	{
		if (CardUseRule.Rule == UseRule && CardUseRule.Status == Status)
		{
			CardUseRule.Cost = NewValue;
			SetCardUseRules(Pile, CardUseRules);
			return true;
		}
	}

	// 규칙이 없을 경우 새 규칙을 추가.
	if (bAddIfNotFound)
	{
		CardUseRules.Add(FUseRule(UseRule, Status, NewValue));
		SetCardUseRules(Pile, CardUseRules);
		return true;
	}

	return false;
}

void ACardBase::SetCardDataFromOtherData(ECardDataType From, ECardDataType To)
{
	FCard& CardData = GetCardByCardDataTypeRef(From);
	if (To == ECardDataType::Hand)
	{
		CardDataHand = CardData;
	}
	if (To == ECardDataType::Deck)
	{
		CardDataHand = CardData;
	}
	if (To == ECardDataType::Base)
	{
		CardDataBase = CardData;
	}
	if (To == ECardDataType::Pile)
	{
		CardDataPile = CardData;
	}
}

bool ACardBase::CallLocalEventOnCard(const FGameplayTag& EventTag, ECallGlobal AlsoCallGlobal)
{
	// DispatcherHubLocal을 사용하여 주어진 이벤트 태그에 해당하는 로컬 이벤트 호출.
	// 이때, 카드(this)를 대상으로 하고 추가적인 전달 파라미터는 없음 (nullptr).
	// AlsoCallGlobal은 글로벌 호출 여부를 결정함.
	return DispatcherHubLocal->CallEvent(EventTag, this, nullptr, AlsoCallGlobal);
}

void ACardBase::Discard(FGameplayTagContainer CallTags)
{
	DispatcherHubLocal->CallEventWithCallTags(CosGameTags::Event_Card_Discard, this, nullptr, ECallGlobal::CallAfter, CallTags);
}

void ACardBase::ModifyCardEffectValues(int32 Modification, ECardDataType InCardType, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags)
{
	// 태그 조건이 비어 있는지 확인.
	bool bNoPossibleTags = PossibleTags.Num() == 0;
	bool bNoRequiredTags = RequiredTags.Num() == 0;

	// 카드 효과가 수정되었는지 추적하는 플래그.
	bool bCardModified = false;

	// 카드 타입에 해당하는 효과 목록을 가져옴.
	TArray<FCardEffect> CardEffects = GetCardEffects(InCardType);

	// 카드 효과 리스트를 순회하며 수정할 값을 적용.
	for (int32 i = 0; i < CardEffects.Num(); i++)
	{
		const FCardEffect& CardEffect = CardEffects[i];

		// 태그 조건을 만족하는지 확인.
		bool bHasPossibleTags = bNoPossibleTags || UBlueprintGameplayTagLibrary::HasAnyTags(CardEffect.GameplayTags, PossibleTags, true);
		bool bHasRequiredTags = bNoRequiredTags || UBlueprintGameplayTagLibrary::HasAllTags(CardEffect.GameplayTags, RequiredTags, true);

		if (bHasPossibleTags && bHasRequiredTags)
		{
			// 카드 효과가 수정되었음을 표시.
			bCardModified = true;

			// 효과 값을 수정하고, 최소 0 이상으로 설정.
			CardEffects[i].EffectValue = FMath::Max(CardEffect.EffectValue + Modification, 0);
		}
	}

	// 카드 효과가 수정되었을 경우, 이를 적용하고 이벤트를 호출.
	if (bCardModified)
	{
		// 수정된 카드 효과를 설정.
		SetCardEffects(InCardType, CardEffects);

		// 카드가 수정되었음을 알리는 이벤트를 호출.
		if (OnThisCardModified.IsBound())
		{
			OnThisCardModified.Broadcast();
		}
	}
}

void ACardBase::Exhaust()
{
	// 1. DispatcherHubLocal을 통해 카드 소모 이벤트를 호출합니다.
	//    이 이벤트는 카드가 소모되었음을 다른 시스템에 알립니다.
	DispatcherHubLocal->CallEvent(CosGameTags::Event_Card_Exhaust, this);
}

void ACardBase::ResetInHandModifications()
{
	// 1. Hand 상태의 카드 데이터를 Pile 상태로 재설정합니다.
	CardDataHand = CardDataPile;
}

void ACardBase::RunEndTurnEvent(FGameplayTagContainer CallTags)
{
	// 카드가 '손'에 있을 때의 종료 턴 이벤트를 가져옴.
	FGameplayTag EndTurnEvent = GetCardEndTurnEvent(ECardDataType::Hand);

	// 유효한 종료 턴 이벤트가 없는 경우, 함수 종료.
	if (!EndTurnEvent.IsValid()) return;

	// 종료 턴 이벤트를 호출하고, 전달된 태그(CallTags)를 함께 전달.
	DispatcherHubLocal->CallEventWithCallTags(EndTurnEvent, this, nullptr, ECallGlobal::CallAfter, CallTags);
}

void ACardBase::Interrupt_Implementation()
{
	bInterrupt = true;
}

void ACardBase::CancelInterruption_Implementation()
{
	bInterrupt = false;
}
