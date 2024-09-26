#include "CardSystem/CardEffects/CardEffect_Attack.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "CardSystem/CardBase.h"
#include "ActionSystem/Action_ScreenShake.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "StatusSystem/Status_Health.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UCardEffect_Attack::UCardEffect_Attack(): AttackTarget(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCardEffect_Attack::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Attack::ResolveCardEffect(AActor* TargetActor)
{
	// 공격 대상을 설정
	AttackTarget = TargetActor;

	// ParentCard가 유효한 경우, 공격 전 이벤트를 호출
	if (IsValid(ParentCard))
	{
		// ParentCard 소유자의 DispatcherHubLocalComponent를 가져옴
		UDispatcherHubLocalComponent* DispatcherHubLocal = Cast<UDispatcherHubLocalComponent>
			(ParentCard->GetOwner()->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass()));

		// DispatcherHubLocal이 유효하면 사전 공격 이벤트 호출
		if (IsValid(DispatcherHubLocal))
		{
			DispatcherHubLocal->CallEvent(CosGameTags::Event_PreAttack, this);
		}
	}

	// 액션 매니저 서브시스템에서 화면 흔들림 액션을 생성하고 큐에 추가
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_ScreenShake>([this](AAction_ScreenShake* ScreenShakeAction)
	{
		ScreenShakeAction->Scale = FMath::Abs(EffectValue) * DamageScaleFactor; // 화면 흔들림의 스케일을 설정
	}, ESpawnActorCollisionHandlingMethod::Undefined);


	// 공격 대상의 DispatcherHubLocalComponent에서 IncomingAttack 이벤트를 호출
	if (UDispatcherHubLocalComponent* TargetDispatcherHub = Cast<UDispatcherHubLocalComponent>(
		AttackTarget->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass())))
	{
		TargetDispatcherHub->CallEvent(CosGameTags::Event_IncomingAttack, this);
	}

	// 대상의 상태 컴포넌트에서 체력(Status_Health)을 찾아 공격 데미지 처리
	UStatus_Health* TargetHealthStatus = Cast<UStatus_Health>(AttackTarget->GetComponentByClass(UStatus_Health::StaticClass()));
	if (IsValid(TargetHealthStatus))
	{
		// 체력 감소 처리
		TargetHealthStatus->SubtractStatusValue(EffectValue, true, false, nullptr);
	}

	// 공격 후 이벤트를 호출
	if (IsValid(ParentCard) && IsValid(ParentCard->GetOwner()))
	{
		// ParentCard 소유자의 DispatcherHubLocalComponent에서 PostAttack 이벤트를 호출
		if (UDispatcherHubLocalComponent* PostAttackDispatcherHub = Cast<UDispatcherHubLocalComponent>(
			ParentCard->GetOwner()->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass())))
		{
			PostAttackDispatcherHub->CallEvent(CosGameTags::Event_PostAttack, this);
		}
	}

	return true;
}
