#include "CardSystem/CardBase.h"

#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardSystem/CardActions/Action_Effect.h"
#include "CombatSystem/CardUseRules/UseRuleComponent.h"
#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "CombatSystem/TargetSystem/Targeting_UnTargetedComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Utility.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values
ACardBase::ACardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	DispatcherHubLocal = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubLocal"));
}

// Called when the game starts or when spawned
void ACardBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	FString FailMessage;
	// CheckIfPlayable로 가능 여부 확인 후 UseCard
	if (CheckIfPlayable(FailMessage))
	{
		UseCard(SkipConsequences, AutoPlay);
		return true;
	}

	// 사용불가능한 상태이므로 에러메시지 출력
	UFunctionLibrary_Utility::SendScreenLogMessage(FText::FromString(FailMessage), FColor::Red);
	return false;
}

void ACardBase::UseCard(bool SkipConsequences, bool AutoPlay)
{
	// DispatcherHub에 이벤트 등록
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_UseCard,
	                                                        this, nullptr, 1.0f, nullptr, FGameplayTagContainer());

	FGameplayTagContainer TagContainer;
	if (AutoPlay)
		TagContainer.AddTag(CosGameTags::Flag_AutoPlayed);

	DispatcherHubLocal->CallEventWithCallTags(CosGameTags::Event_Card_PrePlay, this, nullptr, ECallGlobal::CallAfter, TagContainer);

	// 사용한 카드의 사용 후처리를 진행한다.
	// 규칙들을 순회하면서 결과를 적용한다.
	if (!SkipConsequences)
	{
		ResolveUseRuleConsequences();
	}

	// repetitions 초기화
	CurrentRepetitions = -1;

	ContinueToNextRepetition();
}

bool ACardBase::CheckIfPlayable(FString& FailMessage)
{
	// 카드의 멤버변수에서 CardDataHand의 Rule을 가져온다.
	TArray<FUseRule> Rules = GetCardUseRules(ECardDataType::Hand);

	for (FUseRule Rule : Rules)
	{
		// 카드에 정의된 UseRuleInstances에서 Rule을 찾는다.
		UUseRuleComponent* RuleComponent = *UseRuleInstances.Find(Rule.Rule);

		// 사용해도 되는지 확인한다.
		// 각 UseRuleComponent를 상속받은 객체들의 재정의된 내용이 실행된다.
		// TODO : 아직 상속받은 컴포넌트의 내용 정의 안됨
		// 예를 들어 StatCost에서는 이 카드의 지정된 Cost가 CardPlayer의 CostValue보다 낮아야만 사용가능이 된다.
		if (!RuleComponent->CheckIfUseAllowed(Rule, FailMessage))
		{
			return false;
		}
	}

	FailMessage = TEXT("");
	return true;
}

TArray<FUseRule> ACardBase::GetCardUseRules(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).UseRules;
}

void ACardBase::ResolveUseRuleConsequences()
{
	TArray<FUseRule> UseRules = GetCardUseRules(ECardDataType::Hand);

	for (FUseRule UseRule : UseRules)
	{
		// UseRuleComponent 상속받은 객체 내부에 정의된 내용을 호출한다.
		// 예를 들어 StatCost에서는 카드를 사용하므로서 소모한 마나를 빼는 작업을 한다.
		UUseRuleComponent* RuleComponent = *UseRuleInstances.Find(UseRule.Rule);

		RuleComponent->ResolveUseConsequence(UseRule);
	}
}

void ACardBase::ContinueToNextRepetition()
{
	// 반복 횟수 1 증가
	++CurrentRepetitions;

	// 
	if (GetCardRepetitions(ECardDataType::Hand) <= CurrentRepetitions)
	{
		if (CurrentRepetitions > 0)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda(
				[&]()
				{
					EffectLoopIndex = -1;
					ContinueToNextEffect();
				}
			));
		}

		return;
	}

	EndCardUse();
}

int32 ACardBase::GetCardRepetitions(ECardDataType Type)
{
	return GetCardDataByCardDataType(Type).Repetitions;
}

void ACardBase::EndCardUse()
{
	// GameplayTag 필요
	// DispatcherHubComponent->CallEvent()

	// GameplayTag 필요
	// DispatcherHubComponent->CallEvent()
}

void ACardBase::ContinueToNextEffect()
{
	// EffectLoopIndex 증가
	++EffectLoopIndex;

	// 인덱스가 범위를 초과하지 않으면
	if (GetCardEffects(ECardDataType::Hand).Num() - 1 >= EffectLoopIndex)
	{
		// TargetLoopIndex 리셋
		TargetLoopIndex = -1;

		// HandType의 CardEffect들 가져오기
		TArray<FCardEffect> CardEffects = GetCardEffects(ECardDataType::Hand);

		// TargetingComponent 가져오기.
		CurrentTargeting = AccessTargetingClassLazy(GetOwner(), CardEffects[EffectLoopIndex].Targeting);

		if (CurrentTargeting->bRequiresInput)
		{
			CurrentTargeting->OnInputTargetsReceived.AddDynamic(this, &ACardBase::ProceedOnInputTargetsReceived);


			FCardEffect CardEffect = GetCardEffects(ECardDataType::Hand)[EffectLoopIndex];
			if (!CurrentTargeting->FindValidTargets(InputTargets, CardEffect, this, false, CurrentValidTargets))
			{
				ContinueToNextTarget();
			}
		}
		else
		{
			FCardEffect CardEffect = GetCardEffects(ECardDataType::Hand)[EffectLoopIndex];
			CurrentTargeting->FindValidTargets(InputTargets, CardEffect, this, false, CurrentValidTargets);

			ContinueToNextTarget();
		}
	}
	else
	{
		ContinueToNextRepetition();
	}
}

void ACardBase::ContinueToNextTarget()
{
	++TargetLoopIndex;

	if (bInterrupt)
	{
		bInterrupt = false;

		EndCardUse();
		return;
	}

	// TargetLoopIndex가 범위를 벗어나면
	if (CurrentValidTargets.Num() - 1 < TargetLoopIndex)
	{
		ContinueToNextEffect();
		return;
	}

	TempCardEffect = GetCardEffects(ECardDataType::Hand)[EffectLoopIndex];

	CurrentCardEffect = NewObject<UCardEffectComponent>(this, TempCardEffect.EffectClass);

	CurrentCardEffect->EffectValue = TempCardEffect.EffectValue;
	CurrentCardEffect->TargetComponent = TempCardEffect.TargetComponent;
	CurrentCardEffect->ParentCard = this;
	CurrentCardEffect->GameplayTags = TempCardEffect.GameplayTags;
	CurrentCardEffect->TargetingClass = TempCardEffect.Targeting;
	CurrentCardEffect->HeroAnim = TempCardEffect.HeroAnim;
	CurrentCardEffect->EffectAction = TempCardEffect.EffectAction;
	CurrentCardEffect->UsedData = TempCardEffect.UsedData;
	CurrentCardEffect->Identifier = TempCardEffect.Identifier;

	if (CurrentCardEffect->bImmediated)
	{
		if (IsValid(CurrentCardEffect->EffectAction))
		{
			if (!GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
			{
				COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardInstance를 상속받지 않았습니다"));
				return;
			}

			AActor* SourcePuppet = IInterface_CardTarget::Execute_GetPuppet(GetOwner());

			AActor* InterfaceActor = CurrentValidTargets[TargetLoopIndex];

			if (!InterfaceActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
			{
				COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardInstance를 상속받지 않았습니다"));
				return;
			}

			AActor* TargetActor = IInterface_CardTarget::Execute_GetPuppet(InterfaceActor);

			QueueCardEffectAction(TargetActor, SourcePuppet, CurrentCardEffect, TargetLoopIndex == 0);
		}

		CurrentCardEffect->ResolveCardEffect();

		if (CurrentCardEffect->bImmediated)
		{
			ContinueAfterCardResolved();
		}
	}
	else
	{
		CurrentCardEffect->OnCardResolved.AddDynamic(this, &ACardBase::ContinueAfterCardResolved);
	}
}

FCard ACardBase::GetCardDataByCardDataType(ECardDataType Type)
{
	FCard result;
	switch (Type)
	{
	case ECardDataType::Base:
		result = CardDataBase;
		break;
	case ECardDataType::Deck:
		result = CardDataDeck;
		break;
	case ECardDataType::Hand:
		result = CardDataHand;
		break;
	case ECardDataType::Pile:
		result = CardDataPile;
		break;
	default:
		break;
	}

	return result;
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

	// 불리고 나서 델리게이트 해제
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

FGameplayTagContainer ACardBase::GetGameplayTags()
{
	return GameplayTagComponent->GameplayTags;
}

FText ACardBase::GetCardDescription(ECardDataType InCardDataType)
{
	if (InCardDataType == ECardDataType::Hand)
	{
		return CardDataHand.Description;
	}
	if (InCardDataType == ECardDataType::Deck)
	{
		return CardDataDeck.Description;
	}
	if (InCardDataType == ECardDataType::Base)
	{
		return CardDataBase.Description;
	}
	if (InCardDataType == ECardDataType::Pile)
	{
		return CardDataPile.Description;
	}
	return {};
}
