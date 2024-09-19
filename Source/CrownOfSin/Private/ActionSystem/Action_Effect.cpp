#include "ActionSystem/Action_Effect.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "CombatSystem/Puppets/PuppetComponent.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CosGameplayTags.h"


AAction_Effect::AAction_Effect(): Target(nullptr), SourcePuppet(nullptr), StartDelayIfNoAnim(0)
{
	PrimaryActorTick.bCanEverTick = true;

	AnimTags.AddTag(CosGameTags::Anim);
	StartDelayIfNoAnim = 0.1f;
}

void AAction_Effect::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_Effect::PlayAction_Implementation()
{
	// 애니메이션이 유효한지 확인
	if (HeroAnim.IsValid() && IsValid(SourcePuppet))
	{
		// PuppetComponent를 찾아 애니메이션 이벤트를 바인딩
		if (UPuppetComponent* PuppetComponent = SourcePuppet->FindComponentByClass<UPuppetComponent>())
		{
			// 애니메이션 이벤트를 체크할 델리게이트를 바인딩
			PuppetComponent->OnAnimationEvent.AddDynamic(this, &AAction_Effect::CheckAnimationEvent);

			// Puppet이 카드 퍼핏 인터페이스를 구현하는지 확인하고 애니메이션 실행
			if (SourcePuppet->Implements<UInterface_CardPuppet>())
			{
				IInterface_CardPuppet::Execute_AnimatePuppet(SourcePuppet, HeroAnim);
			}
		}

		return; // 애니메이션을 실행했으므로 여기서 종료
	}

	// 애니메이션이 없을 경우 지연을 두고 파티클 스폰
	if (StartDelayIfNoAnim >= 0.0f)
	{
		// 지연 후에 효과 파티클을 스폰
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [this]()
		{
			SpawnEffectParticles(); // 지연 후 파티클 스폰
		}, StartDelayIfNoAnim, false);
	}
	else
	{
		// 지연 없이 즉시 파티클 스폰
		SpawnEffectParticles();
	}
}

void AAction_Effect::SpawnEffectParticles()
{
	// 타겟과 파티클이 유효한지 확인
	if (!IsValid(Target) || !IsValid(Effect.Particle))
	{
		Execute_EndAction(this);
		return;
	}

	// 타겟이 UInterface_CardPuppet 인터페이스를 구현하는지 확인
	if (Target->Implements<UInterface_CardPuppet>())
	{
		// 타겟의 중심 좌표 계산
		const FVector PuppetRelativeCenter = IInterface_CardPuppet::Execute_GetPuppetRelativeCenter(Target);
		const FVector TargetLocation = PuppetRelativeCenter + Target->GetActorLocation();

		// 카메라 위치와 타겟 위치 사이의 보간 위치 계산
		const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
		FVector SpawnLocation = UKismetMathLibrary::VLerp(TargetLocation, CameraLocation, 0.2f);
		SpawnLocation.Z = TargetLocation.Z; // 스폰 위치의 Z축을 타겟의 Z값으로 고정

		// 파티클 시스템을 해당 위치에 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Effect.Particle, SpawnLocation);
	}

	// 액션 종료 호출
	Execute_EndAction(this);
}

void AAction_Effect::CheckAnimationEvent(AActor* InPuppet, FGameplayTag InAnimationTag)
{
	// 애니메이션 태그가 일치하는지 확인
	if (InAnimationTag != HeroAnim || !IsValid(SourcePuppet))
	{
		return;
	}

	// 소스 퍼핏에서 PuppetComponent를 찾음
	if (UPuppetComponent* PuppetComponent = SourcePuppet->FindComponentByClass<UPuppetComponent>())
	{
		// 애니메이션 이벤트 핸들러를 해제
		PuppetComponent->OnAnimationEvent.RemoveDynamic(this, &AAction_Effect::CheckAnimationEvent);

		// 파티클 스폰 실행
		SpawnEffectParticles();
	}
}
