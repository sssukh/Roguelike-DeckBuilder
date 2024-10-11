// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEffectSystem/EffectActor.h"

#include "ActionSystem/Action_Effect.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "CombatSystem/TargetSystem/TargetingComponent_Untargeted.h"
#include "Core/CosEnumStruct.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "GameEffectSystem/EffectState.h"
#include "GameEffectSystem/GameEffectComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values
AEffectActor::AEffectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EffectState = CreateDefaultSubobject<UEffectState>("EffectState");

	
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEffectActor::ApplyEffect(bool bSkipConsequences, bool bAutoPlay)
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
	EffectState->ResetRepetition();
	ContinueToNextRepetition();
}

void AEffectActor::ContinueToNextEffect()
{
	// EffectIndex를 증가시켜 다음 Effect로 넘어갑니다.
	++EffectState->EffectLoopIndex;

	// EffectActor를 생성할 때 같이 넣어진 OwningEffects를 이용
	if (EffectState->EffectLoopIndex >= OwningEffects.Num())
	{
		ContinueToNextRepetition();
		return;
	}

	// 지금 처리할 Effect를 받아옵니다.
	const FCardEffect& CurrentEffect = OwningEffects[EffectState->EffectLoopIndex];

	// Effect를 처리합니다.
	ProcessCurrentEffect(CurrentEffect);
}

void AEffectActor::ContinueToNextTarget()
{
	++EffectState->TargetLoopIndex;

	if (EffectState->bInterrupt)
	{
		EffectState->bInterrupt = false;
		EndEffect();
		return;
	}

	if (EffectState->TargetLoopIndex >= EffectState->CurrentValidTargets.Num())
	{
		ContinueToNextEffect();
		return;
	}
	
	InitializeEffectComponent(OwningEffects[EffectState->EffectLoopIndex]);

	if (CurrentEffectComponent->bImmediate)
	{
		HandleImmediateEffect();
	}
	else
	{
		CurrentEffectComponent->OnEffectResolved.AddUniqueDynamic(this, &AEffectActor::ContinueAfterTakingEffect);
	}
}

void AEffectActor::ContinueToNextRepetition()
{
	// 1. 현재 반복 횟수를 증가시킵니다.
	++EffectState->Repetition;

	// 2. 현재 반복 횟수가 효과에 설정된 최대 반복 횟수보다 작거나 같으면 계속 진행합니다.
	 if (EffectState->Repetition <= EffectState->Repetition)
	 {
	 	// 3. 반복이 1회 이상일 경우, 다음 틱에서 다음 카드 효과로 진행합니다.
	 	if (EffectState->Repetition > 0)
	 	{
	 		// 다음 틱에서 실행할 함수를 설정합니다.
	 		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
	 		{
	 			EffectState->ResetState(); // 효과 루프 인덱스를 초기화합니다.
	 			ContinueToNextEffect(); // 다음 효과로 넘어갑니다.
	 		}));
	 	}
	 	else
	 	{
	 		// 4. 반복이 0일 경우, 바로 다음 카드 효과로 진행합니다.
	 		EffectState->ResetState(); // 효과 루프 인덱스를 초기화합니다.
	 		ContinueToNextEffect();
	 	}
	 }
	 else
	 {
	 	// 5. 반복이 모두 완료되었으면  효과를 종료합니다.
	 	EndEffect();
	 }
}

void AEffectActor::HandleInterruption()
{
}

void AEffectActor::ProcessCurrentEffect(const FCardEffect& Effect)
{
	CurrentTargetingComponent = AccessTargetingClassLazy(GetOwner(), Effect.Targeting);
        
	if (!CurrentTargetingComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to access targeting component"));
		ContinueToNextEffect();
		return;
	}

	if (CurrentTargetingComponent->bRequiresInput)
	{
		CurrentTargetingComponent->OnInputTargetsReceived.AddUniqueDynamic(this, &AEffectActor::ProceedOnInputTargetsReceived);

		TArray<AActor*> ValidTargets;
		if (!CurrentTargetingComponent->FindValidTargets(InputTargets, Effect, this, false, ValidTargets))
		{
			ContinueToNextTarget();
		}
	}
	else
	{
		TArray<AActor*> ValidTargets;
		CurrentTargetingComponent->FindValidTargets(InputTargets, Effect, this, false, ValidTargets);
		CurrentValidTargets = ValidTargets;
		ContinueToNextTarget();
	}
}

void AEffectActor::InitializeEffectComponent(const FCardEffect& Effect)
{
	// 카드 효과 컴포넌트를 생성하고 등록.
	UGameEffectComponent* NewGameEffectComponent = NewObject<UGameEffectComponent>(this, Effect.EffectClass);
	// 카드 효과 데이터를 컴포넌트에 설정.
	NewGameEffectComponent->EffectValue = Effect.EffectValue;
	NewGameEffectComponent->TargetComponent = Effect.TargetComponent;
	NewGameEffectComponent->ParentCard = this;
	NewGameEffectComponent->GameplayTags = Effect.GameplayTags;
	NewGameEffectComponent->TargetingClass = Effect.Targeting;
	NewGameEffectComponent->HeroAnim = Effect.HeroAnim;
	NewGameEffectComponent->EffectActionClass = Effect.EffectAction;
	NewGameEffectComponent->UsedData = Effect.UsedData;
	NewGameEffectComponent->Identifier = Effect.Identifier;

	NewGameEffectComponent->RegisterComponent();
	AddInstanceComponent(NewGameEffectComponent);

	// 현재 카드 효과를 새로 생성한 컴포넌트로 설정.
	CurrentEffectComponent = NewGameEffectComponent;
}

void AEffectActor::ContinueAfterTakingEffect()
{
	CurrentEffectComponent->DestroyComponent();
	ContinueToNextTarget();
}

void AEffectActor::HandleImmediateEffect()
{
	// 카드 효과의 액션이 유효한지 확인하고 실행.
	if (IsValid(CurrentEffectComponent->EffectActionClass))
	{
		ExecuteEffectAction();
	}

	// 타겟에 대한 카드 효과를 처리.
	CurrentEffectComponent->TakeEffect(CurrentValidTargets[EffectState->TargetLoopIndex]);
	
	// 카드 효과가 즉시 실행되었다면, 후속 처리를 바로 진행.
	if (CurrentEffectComponent->bImmediate)
	{
		ContinueAfterCardResolved();
	}
}

void AEffectActor::EndEffect()
{
	DispatcherHubLocal->CallEvent(CosGameTags::Event_Card_PostPlay, this);
	DispatcherHubLocal->CallEvent(GetPostUseEvent(ECardDataType::Hand), this);
}

UTargetingComponent* AEffectActor::AccessTargetingClassLazy(AActor* TargetingHolderActor,
	TSubclassOf<UTargetingComponent> TargetingClass)
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

void AEffectActor::ProceedOnInputTargetsReceived(TArray<AActor*> Targets)
{
	// 입력된 유효한 타겟들을 저장.
	CurrentValidTargets = Targets;

	// 델리게이트에서 타겟 입력 수신자를 제거하여 중복 호출 방지.
	CurrentTargetingComponent->OnInputTargetsReceived.RemoveDynamic(this, &AEffectActor::ProceedOnInputTargetsReceived);

	// 다음 타겟을 처리.
	ContinueToNextTarget();
}

void AEffectActor::ExecuteEffectAction()
{
	AActor* SourcePuppet = nullptr;
	if (IsValid(GetOwner())&&GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		// 소스 퍼펫과 타겟을 가져온다.
		SourcePuppet = IInterface_CardTarget::Execute_GetPuppet(GetOwner());
	}

	// 소스 퍼펫과 타겟을 가져온다.
	// AActor* SourcePuppet = IInterface_CardTarget::Execute_GetPuppet(GetOwner());
	AActor* TargetActor = GetValidTargetPuppet(CurrentValidTargets[EffectState->TargetLoopIndex]);

	// 유효한 타겟이 없으면 로그를 출력하고 함수 종료.
	if (!TargetActor)
	{
		COS_SCREEN(TEXT("대상이 UInterface_CardTarget을 구현하지 않습니다."));
		// return;
	}

	// 카드 효과 액션을 큐에 추가
	QueueGameEffectAction(TargetActor, SourcePuppet, CurrentEffectComponent, EffectState->TargetLoopIndex == 0);
}

AActor* AEffectActor::GetValidTargetPuppet(AActor* TargetActor) const
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


