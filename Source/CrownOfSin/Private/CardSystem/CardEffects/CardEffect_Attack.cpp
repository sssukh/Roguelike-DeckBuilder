#include "CardSystem/CardEffects/CardEffect_Attack.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardActions/Action_ScreenShake.h"
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
	AttackTarget = TargetActor;

	if (IsValid(ParentCard))
	{
		UDispatcherHubLocalComponent* DispatcherHubLocal = Cast<UDispatcherHubLocalComponent>(ParentCard->GetOwner()->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass()));
		if (IsValid(DispatcherHubLocal))
		{
			DispatcherHubLocal->CallEvent(CosGameTags::Event_PreAttack, this);
		}
	}


	FTransform SpawnTransform = FTransform::Identity;

	if (AAction_ScreenShake* NewActionScreenShake = GetWorld()->SpawnActorDeferred<AAction_ScreenShake>(AAction_ScreenShake::StaticClass(), SpawnTransform,
	                                                                                                    nullptr, nullptr,
	                                                                                                    ESpawnActorCollisionHandlingMethod::Undefined))
	{
		NewActionScreenShake->Scale = FMath::Abs(EffectValue) * DamageScaleFactor;
		//ToDo 값을 설정해야합니다.
		// NewActionScreenShake->ShakeClass = 
		NewActionScreenShake->FinishSpawning(SpawnTransform);
	}

	if (UDispatcherHubLocalComponent* DispatcherHubLocalComponent = Cast<UDispatcherHubLocalComponent>(AttackTarget->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass())))
	{
		DispatcherHubLocalComponent->CallEvent(CosGameTags::Event_IncomingAttack, this);
	}

	UStatus_Health* Status_Health = Cast<UStatus_Health>(AttackTarget->GetComponentByClass(UStatus_Health::StaticClass()));
	if (IsValid(Status_Health))
	{
		Status_Health->SubtractStatusValue(EffectValue, true, false, nullptr);
	}

	if (IsValid(ParentCard))
	{
		if (IsValid(ParentCard->GetOwner()))
		{
			if (UDispatcherHubLocalComponent* DispatcherHubLocalComponent = Cast<UDispatcherHubLocalComponent>
				(ParentCard->GetOwner()->GetComponentByClass(UDispatcherHubLocalComponent::StaticClass())))
			{
				DispatcherHubLocalComponent->CallEvent(CosGameTags::Event_PostAttack, this);
			}
		}
	}


	return true;
}
