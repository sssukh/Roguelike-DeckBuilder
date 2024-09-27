#include "CombatSystem/Puppets/SkeletalPuppet.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_AdvAnim.h"
#include "ActionSystem/Action_Appear.h"
#include "ChaosCloth/ChaosClothingSimulationFactory.h"
#include "CombatSystem/Puppets/PuppetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/MinionBase.h"
#include "Libraries/AssetPath.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Utilities/CosGameplayTags.h"


ASkeletalPuppet::ASkeletalPuppet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	//..

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	//Create Skeletal
	{
		SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

		// 옷감 시뮬레이션 팩토리 설정 (Chaos 사용)
		SkeletalMeshComponent->ClothingSimulationFactory = UChaosClothingSimulationFactory::StaticClass();

		// 충돌 이벤트 발생 설정
		SkeletalMeshComponent->SetGenerateOverlapEvents(true);

		// 충돌 설정 (BodyInstance에 대한 설정)
		SkeletalMeshComponent->SetCollisionObjectType(ECC_Pawn);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		SkeletalMeshComponent->SetCollisionProfileName(TEXT("Pawn")); // 충돌 프로필을 "Pawn"으로 설정

		SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // 충돌 응답 설정 (Visibility 채널을 무시)

		SkeletalMeshComponent->SetupAttachment(DefaultSceneRoot);
	}

	// Create BottomUIWidgetComponent
	{
		BottomUIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BottomUI"));

		// 위젯을 화면 공간에 표시하도록 설정
		BottomUIWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		// 위젯의 크기 설정
		BottomUIWidgetComponent->SetDrawSize(FVector2D(300.0f, 600.0f));

		// 피벗 설정 (중앙 아래쪽에 맞춤)
		BottomUIWidgetComponent->SetPivot(FVector2D(0.5f, 1.0f));

		// 컴포넌트의 상대 위치와 회전 설정
		BottomUIWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		BottomUIWidgetComponent->SetRelativeRotation(FRotator(0.0f, 90.000244f, 0.0f));


		// 위젯 클래스 설정 (WBP_MinionUI의 클래스를 찾아서 설정) 
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(*AssetPath::Blueprint::WBP_MinionUI_C);
		if (WidgetClassFinder.Succeeded())
		{
			BottomUIWidgetComponent->SetWidgetClass(WidgetClassFinder.Class);
		}

		// 이 컴포넌트를 액터의 루트 컴포넌트 또는 다른 컴포넌트에 부착할 수 있습니다.
		BottomUIWidgetComponent->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착
	}

	//Create MouseCollider
	{
		// BoxComponent 생성
		MouseCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("MouseCollider"));

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

		// 상대 위치 설정
		MouseCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

		MouseCollider->SetupAttachment(RootComponent);
	}

	//Create TopUIWidgetComponent
	{
		// WidgetComponent 생성
		TopUIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TopUI"));

		// 위젯을 화면 공간에 표시하도록 설정
		TopUIWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		// 위젯 크기 설정 (100x100)
		TopUIWidgetComponent->SetDrawSize(FVector2D(100.0f, 100.0f));

		// 피벗 설정 (중앙 아래쪽에 맞춤)
		TopUIWidgetComponent->SetPivot(FVector2D(0.5f, 1.0f));

		// 상대 위치 및 회전 설정
		TopUIWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 256.0f));
		TopUIWidgetComponent->SetRelativeRotation(FRotator(0.0f, 90.000244f, 0.0f));

		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(*AssetPath::Blueprint::WBP_MinionUiTop_C);
		if (WidgetClassFinder.Succeeded())
		{
			TopUIWidgetComponent->SetWidgetClass(WidgetClassFinder.Class);
		}

		// 이 컴포넌트를 액터의 루트 컴포넌트 또는 다른 컴포넌트에 부착할 수 있습니다.
		TopUIWidgetComponent->SetupAttachment(RootComponent);
	}

	PuppetComponent = CreateDefaultSubobject<UPuppetComponent>(TEXT("PuppetComponent"));
}


void ASkeletalPuppet::BeginPlay()
{
	Super::BeginPlay();
}

void ASkeletalPuppet::SetRenderCustomDepthForPrimitives(const TArray<UPrimitiveComponent*>& PrimitiveComponents, bool bVisibility, int32 StencilValue)
{
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		PrimitiveComponent->SetRenderCustomDepth(bVisibility);
		PrimitiveComponent->SetCustomDepthStencilValue(StencilValue);
	}
}

void ASkeletalPuppet::SetRenderCustomDepthForEntirePuppet(bool bVisibility, int32 StencilValue)
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

void ASkeletalPuppet::InitializePuppet_Implementation(AMinionBase* InMinion, bool bSpawnedDuringGameplay)
{
	// 1. 부모 미니언 설정 및 데이터 가져오기
	ParentMinion = InMinion;
	const FMinion& MinionData = ParentMinion->MinionData;

	// 2. 스켈레탈 메시 및 애니메이션, 트랜스폼 설정
	SkeletalMeshComponent->SetSkinnedAssetAndUpdate(MinionData.SkeletalMesh);
	SkeletalMeshComponent->SetAnimInstanceClass(MinionData.AnimationBlueprint);
	SkeletalMeshComponent->SetRelativeTransform(MinionData.TransformOffset);

	// 3. UI 설정
	BottomUIWidgetComponent->SetDrawSize(MinionData.UIScale);
	TopUIWidgetComponent->SetRelativeLocation(FVector(0, 0, MinionData.UIScale.Y * 0.5f));

	// 4. 마우스 콜라이더의 스케일 설정
	MouseCollider->SetWorldScale3D(FVector(1, MinionData.UIScale.X / 180.0f, 1));

	// 5. 스켈레탈 메시의 재질 설정
	int32 Index = 0;
	for (UMaterial* Material : MinionData.Materials)
	{
		if (IsValid(Material))
		{
			SkeletalMeshComponent->SetMaterial(Index, Material);
		}
		Index++;
	}

	// 6. 모든 메쉬 컴포넌트에 대해 동적 재질 인스턴스를 생성
	TArray<UActorComponent*> MeshComponents;
	GetComponents(UMeshComponent::StaticClass(), MeshComponents);
	for (UActorComponent* MeshActorComponent : MeshComponents)
	{
		UMeshComponent* MeshComponent = Cast<UMeshComponent>(MeshActorComponent);
		if (!MeshComponent) continue;;

		TArray<UMaterialInterface*> Materials = MeshComponent->GetMaterials();
		for (int i = 0; i < Materials.Num(); ++i)
		{
			DynamicMeshMaterials.Emplace(MeshComponent->CreateDynamicMaterialInstance(i, Materials[i]));
		}
	}

	// 7. 게임 중 스폰된 경우 페이드 인 애니메이션 처리
	if (bSpawnedDuringGameplay)
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

void ASkeletalPuppet::AnimatePuppet_Implementation(FGameplayTag InAnimationTag)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance->Implements<UInterface_CardPuppet>())
	{
		IInterface_CardPuppet::Execute_AnimatePuppet(AnimInstance, InAnimationTag);
	}
}

void ASkeletalPuppet::SignalAnimEvent_Implementation(FGameplayTag InAnimationTag, FName InNotify)
{
	if (PuppetComponent->OnAnimationEvent.IsBound())
		PuppetComponent->OnAnimationEvent.Broadcast(this, InAnimationTag);
}

void ASkeletalPuppet::SignalAnimEnd_Implementation(FGameplayTag InAnimationTag)
{
	if (PuppetComponent->OnAnimationEnd.IsBound())
		PuppetComponent->OnAnimationEnd.Broadcast(this, InAnimationTag);
}

void ASkeletalPuppet::MarkPuppet_Implementation()
{
	SetRenderCustomDepthForEntirePuppet(true, 0);
}

void ASkeletalPuppet::UnmarkPuppet_Implementation()
{
	SetRenderCustomDepthForEntirePuppet(false, 0);
}

bool ASkeletalPuppet::GetMinionFromPuppet_Implementation(AMinionBase*& OutMinion)
{
	if (IsValid(ParentMinion))
	{
		OutMinion = ParentMinion;
		return true;
	}
	OutMinion = nullptr;
	return false;
}

UObject* ASkeletalPuppet::GetPuppetUI_Implementation(EStatusSlot InSlot)
{
	UWidgetComponent* UIWidgetComponent = InSlot == EStatusSlot::Active ? TopUIWidgetComponent : BottomUIWidgetComponent;
	return UIWidgetComponent->GetWidget();
}

FVector ASkeletalPuppet::GetPuppetRelativeCenter_Implementation()
{
	return FVector(0, 0, SkeletalMeshComponent->Bounds.BoxExtent.Z / 1.5f);
}
