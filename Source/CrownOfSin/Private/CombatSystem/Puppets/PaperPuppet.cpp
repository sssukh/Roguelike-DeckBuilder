#include "CombatSystem/Puppets/PaperPuppet.h"

#include "PaperFlipbookComponent.h"
#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_AdvAnim.h"
#include "ActionSystem/Action_Appear.h"
#include "CombatSystem/Puppets/PuppetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/CosEnumStruct.h"
#include "Core/MinionBase.h"
#include "Interfaces/Interface_Utility.h"
#include "Libraries/AssetPath.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Utilities/CosGameplayTags.h"


APaperPuppet::APaperPuppet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//..

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	// Create PaperScene
	{
		PaperScene = CreateDefaultSubobject<USceneComponent>(TEXT("PaperScene"));
		PaperScene->SetRelativeLocation(FVector(0.0f, 0.0f, 145.0f));
		PaperScene->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		PaperScene->SetRelativeScale3D(FVector(3.5f, 3.5f, 3.5f));
		PaperScene->SetupAttachment(DefaultSceneRoot);
	}

	// Create PuppetFlipbook
	{
		PuppetFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PuppetFlipbook"));
		PuppetFlipbook->SetupAttachment(PaperScene);
	}


	// Create MinionUI
	{
		MinionUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("BottomUI"));

		// 위젯을 화면 공간에 표시하도록 설정
		MinionUI->SetWidgetSpace(EWidgetSpace::Screen);

		// 위젯의 크기 설정
		MinionUI->SetDrawSize(FVector2D(300.0f, 600.0f));

		// 피벗 설정 (중앙 아래쪽에 맞춤)
		MinionUI->SetPivot(FVector2D(0.5f, 1.0f));

		// 컴포넌트의 상대 위치와 회전 설정
		MinionUI->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		MinionUI->SetRelativeRotation(FRotator(0.0f, 90.000244f, 0.0f));


		// 위젯 클래스 설정 (WBP_MinionUI의 클래스를 찾아서 설정) 
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(*AssetPath::Blueprint::WBP_MinionUiTop_C);
		if (WidgetClassFinder.Succeeded())
		{
			MinionUI->SetWidgetClass(WidgetClassFinder.Class);
		}

		// 이 컴포넌트를 액터의 루트 컴포넌트 또는 다른 컴포넌트에 부착할 수 있습니다.
		MinionUI->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착
	}

	//Create MouseCollider
	{
		// BoxComponent 생성
		MouseCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("MouseCollider"));

		// 상대 위치 설정
		MouseCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

		// 박스의 크기 설정
		MouseCollider->SetBoxExtent(FVector(1.0f, 80.0f, 130.0f));

		// 네비게이션 영역 클래스 설정 (NavArea_Obstacle 사용)
		MouseCollider->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());

		// 캐릭터가 이 컴포넌트 위를 걸을 수 없도록 설정	
		MouseCollider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

		// 충돌 설정 (BodyInstance에 대한 설정)
		MouseCollider->SetCollisionObjectType(ECC_Pawn);
		MouseCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MouseCollider->SetCollisionProfileName(TEXT("Pawn"));

		// 충돌 응답 설정 (Visibility 채널을 무시)
		MouseCollider->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

		MouseCollider->SetupAttachment(RootComponent);
	}


	PuppetComponent = CreateDefaultSubobject<UPuppetComponent>(TEXT("PuppetComponent"));
}

void APaperPuppet::BeginPlay()
{
	Super::BeginPlay();
}

void APaperPuppet::SetRenderCustomDepthForPrimitives(const TArray<UPrimitiveComponent*>& PrimitiveComponents, bool bVisibility, int32 StencilValue)
{
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		PrimitiveComponent->SetRenderCustomDepth(bVisibility);
		PrimitiveComponent->SetCustomDepthStencilValue(StencilValue);
	}
}

void APaperPuppet::SetRenderCustomDepthForEntirePuppet(bool bVisibility, int32 StencilValue)
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;

	TArray<UActorComponent*> SkeletalMeshComponents;
	GetComponents(USkeletalMeshComponent::StaticClass(), SkeletalMeshComponents);
	for (UActorComponent* FoundSkeletalMeshComponent : SkeletalMeshComponents)
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(FoundSkeletalMeshComponent);
		if (!PrimitiveComponent) continue;
		PrimitiveComponents.Add(PrimitiveComponent);
	}

	TArray<UActorComponent*> StaticMeshComponents;
	GetComponents(UStaticMeshComponent::StaticClass(), StaticMeshComponents);
	for (UActorComponent* FoundStaticMeshComponent : StaticMeshComponents)
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(FoundStaticMeshComponent);
		if (!PrimitiveComponent) continue;
		PrimitiveComponents.Add(PrimitiveComponent);
	}

	SetRenderCustomDepthForPrimitives(PrimitiveComponents, bVisibility, StencilValue);
}

void APaperPuppet::HandleAnimatePuppet()
{
	UPaperFlipbook* IdleFlipBook = AnimFlipbooks[CosGameTags::Anim_Idle];
	PuppetFlipbook->SetFlipbook(IdleFlipBook);
	if (PuppetComponent->OnAnimationEnd.IsBound())
		PuppetComponent->OnAnimationEnd.Broadcast(this, CurrentAnimation);
}

void APaperPuppet::InitializePuppet_Implementation(AMinionBase* InMinion, bool bSpawnedDuringGameplay)
{
	// 1. 부모 미니언 설정 및 데이터 가져오기
	ParentMinion = InMinion;
	const FMinion& MinionData = ParentMinion->MinionData;

	MinionUI->SetDrawSize(MinionData.UIScale);

	PuppetFlipbook->SetRelativeTransform(MinionData.TransformOffset);

	MouseCollider->SetWorldScale3D(FVector(1, MinionData.UIScale.X / 180.0f, 1));

	// 7. 게임 중 스폰된 경우 페이드 인 애니메이션 처리
	if (bSpawnedDuringGameplay && AnimFlipbooks.Contains(CosGameTags::Anim_Spawn))
	{
		SetActorHiddenInGame(true); // 처음에는 보이지 않도록 설정

		// 페이드 인 액션 생성
		UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
		ActionManagerSubsystem->CreateAndQueueAction<AAction_Appear>([this](AAction_Appear* AAction_Appear)
		{
			AAction_Appear->bDisappear = false;
			AAction_Appear->FadeSpeed = 1.0f;
			AAction_Appear->Puppet = this;
			AAction_Appear->EndDelay = -1.0f;
		});

		// 스폰 애니메이션 액션 생성
		ActionManagerSubsystem->CreateAndQueueAction<AAction_AdvAnim>([this](AAction_AdvAnim* Action_AdvAnim)
		{
			Action_AdvAnim->Animation = CosGameTags::Anim_Spawn;
			Action_AdvAnim->Puppet = this;
			Action_AdvAnim->EndDelay = -1.0f;
		});
	}
}

void APaperPuppet::AnimatePuppet_Implementation(FGameplayTag InAnimationTag)
{
	CurrentAnimation = InAnimationTag;

	// 주어진 애니메이션 태그에 맞는 Flipbook이 있는지 확인
	if (AnimFlipbooks.Contains(CurrentAnimation))
	{
		UPaperFlipbook* NewFlipBook = AnimFlipbooks[CurrentAnimation];
		PuppetFlipbook->SetFlipbook(NewFlipBook);

		// Hit Frame이 있는 Flipbook인 경우
		if (FlipbookHitFrames.Contains(NewFlipBook))
		{
			float FlipbookTotalLength = PuppetFlipbook->GetFlipbookLength();
			float HitFramePosition = FlipbookHitFrames[NewFlipBook] / static_cast<float>(PuppetFlipbook->GetFlipbookLengthInFrames());

			// Hit Frame까지의 지연 시간 계산
			float HitFrameDelay = FlipbookTotalLength * HitFramePosition;

			FTimerHandle HitFrameTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(HitFrameTimerHandle, [&]()
			{
				// 애니메이션 이벤트를 전달
				if (PuppetComponent->OnAnimationEvent.IsBound())
					PuppetComponent->OnAnimationEvent.Broadcast(this, CurrentAnimation);

				// Hit Frame 이후 애니메이션 종료까지의 시간 계산
				float RemainingTime = FlipbookTotalLength - HitFrameDelay;
				FTimerHandle RemainingTimeHandle;
				GetWorld()->GetTimerManager().SetTimer(RemainingTimeHandle, [&]()
				{
					HandleAnimatePuppet(); // 애니메이션 종료 처리
				}, RemainingTime, false);
			}, HitFrameDelay, false); // Hit Frame까지 지연
		}
		else
		{
			// Hit Frame이 없는 경우 전체 애니메이션 길이만큼 대기 후 종료 처리
			float TotalFlipbookLength = PuppetFlipbook->GetFlipbookLength();
			FTimerHandle FlipbookTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(FlipbookTimerHandle, [this]()
			{
				HandleAnimatePuppet();
			}, TotalFlipbookLength, false);
		}
	}
	else
	{
		// 해당하는 Flipbook이 없으면 이벤트 바로 호출
		if (PuppetComponent->OnAnimationEvent.IsBound())
			PuppetComponent->OnAnimationEvent.Broadcast(this, CurrentAnimation);

		// 애니메이션 종료 시점 처리
		if (PuppetComponent->OnAnimationEnd.IsBound())
			PuppetComponent->OnAnimationEnd.Broadcast(this, CurrentAnimation);
	}
}

void APaperPuppet::MarkPuppet_Implementation()
{
	if (MinionUI->GetWidget()->Implements<UInterface_Utility>())
	{
		IInterface_Utility::Execute_Interact(MinionUI->GetWidget(), FGameplayTagContainer(CosGameTags::Interact_Mark));
	}
}

void APaperPuppet::UnmarkPuppet_Implementation()
{
	if (MinionUI->GetWidget()->Implements<UInterface_Utility>())
	{
		IInterface_Utility::Execute_Interact(MinionUI->GetWidget(), FGameplayTagContainer(CosGameTags::Interact_UnMark));
	}
}

void APaperPuppet::AppearPuppet_Implementation(float InSpeedProp)
{
	SetActorHiddenInGame(false);
}

void APaperPuppet::DisappearPuppet_Implementation(float InSpeedProp)
{
	// Hit Frame이 없는 경우 전체 애니메이션 길이만큼 대기 후 종료 처리
	float TotalFlipbookLength = PuppetFlipbook->GetFlipbookLength();
	FTimerHandle DisappearTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, [this]()
	{
		PuppetFlipbook->Stop();
		Destroy();
	}, TotalFlipbookLength, false);
}

bool APaperPuppet::GetMinionFromPuppet_Implementation(AMinionBase*& OutMinion)
{
	if (IsValid(ParentMinion))
	{
		OutMinion = ParentMinion;
		return true;
	}

	OutMinion = nullptr;
	return false;
}

UObject* APaperPuppet::GetPuppetUI_Implementation(EStatusSlot InSlot)
{
	return MinionUI->GetWidget();
}

FVector APaperPuppet::GetPuppetRelativeCenter_Implementation()
{
	return FVector(0, 0, PaperScene->GetComponentLocation().Z);
}
