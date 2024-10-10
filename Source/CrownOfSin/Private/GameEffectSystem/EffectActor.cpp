// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEffectSystem/EffectActor.h"

#include "Core/CosEnumStruct.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "GameEffectSystem/EffectState.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"


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
	ContinueToNextRepetition();
	
}

void AEffectActor::ContinueToNextEffect()
{
	++EffectState->EffectLoopIndex;

	// TODO:수정 요함. 함수화 시켜서 자식클래스에게 각각 맞는 값을 가져오자.
	// const TArray<FCardEffect>& HandCardEffects = GetCardEffects(ECardDataType::Hand);
        
	// if (EffectState->EffectLoopIndex >= HandCardEffects.Num())
	// {
	// 	ContinueToNextRepetition();
	// 	return;
	// }

	
	// const FCardEffect& CurrentEffect = HandCardEffects[EffectState->EffectLoopIndex];

	// TODO: 수정 요함.
	// ProcessCurrentEffect(CurrentEffect);
}

void AEffectActor::ContinueToNextTarget()
{
	++EffectState->TargetLoopIndex;

	if (EffectState->bInterrupt)
	{
		HandleInterruption();
		return;
	}

	if (EffectState->TargetLoopIndex >= EffectState->CurrentValidTargets.Num())
	{
		ContinueToNextEffect();
		return;
	}

	// TODO:수정 요함. 함수화 시켜서 자식클래스에게 각각 맞는 값을 가져오자.
	// const TArray<FCardEffect>& HandCardEffects = GetCardEffects(ECardDataType::Hand);
	
	// const FCardEffect& CurrentEffect = HandCardEffects[EffectState->EffectLoopIndex];

	// 받아온 이펙트 구조체로 EffectComponent 초기화
	// InitializeCurrentCardEffect(CurrentEffect);

	// 즉시 발동할 것인지 아닌지 체크 후 처리
	// if (CurrentCardEffectComponent->bImmediate)
	// {
	// 	HandleImmediateCardEffect();
	// }
	// else
	// {
	// 	CurrentCardEffectComponent->OnCardResolved.AddUniqueDynamic(this, &ACardBase::ContinueAfterCardResolved);
	// }
}

void AEffectActor::ContinueToNextRepetition()
{
	// 1. 현재 반복 횟수를 증가시킵니다.
	++EffectState->Repetition;

	//TODO: 2. 현재 반복 횟수가 효과에 설정된 최대 반복 횟수보다 작거나 같으면 계속 진행합니다.
	// if (EffectState->Repetition <= GetCardRepetitions(ECardDataType::Hand))
	// {
	// 	// 3. 반복이 1회 이상일 경우, 다음 틱에서 다음 카드 효과로 진행합니다.
	// 	if (EffectState->Repetition > 0)
	// 	{
	// 		// 다음 틱에서 실행할 함수를 설정합니다.
	// 		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
	// 		{
	// 			EffectState->ResetState(); // 효과 루프 인덱스를 초기화합니다.
	// 			ContinueToNextEffect(); // 다음 효과로 넘어갑니다.
	// 		}));
	// 	}
	// 	else
	// 	{
	// 		// 4. 반복이 0일 경우, 바로 다음 카드 효과로 진행합니다.
	// 		EffectState->ResetState(); // 효과 루프 인덱스를 초기화합니다.
	// 		ContinueToNextEffect();
	// 	}
	// }
	// else
	// {
	// 	// TODO: 5. 반복이 모두 완료되었으면  효과를 종료합니다.
	// 	// EndEffect();
	// }
}


