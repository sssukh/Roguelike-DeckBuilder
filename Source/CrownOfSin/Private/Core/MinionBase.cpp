#include "Core/MinionBase.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_Appear.h"
#include "CombatSystem/AttackPatternComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Core/MinionTrack.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "Libraries/FunctionLibrary_Utility.h"
#include "StatusSystem/StatusComponent.h"
#include "StatusSystem/Status_Initiative.h"
#include "Utilities/CosGameplayTags.h"


AMinionBase::AMinionBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	DispatcherHubLocalComponent = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubLocalComponent"));
	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

void AMinionBase::BeginPlay()
{
	Super::BeginPlay();

	// 1. 미니언의 게임플레이 태그 설정
	GameplayTagComponent->GameplayTags = MinionData.GameplayTags;

	// 2. 퍼펫 스폰
	FTransform SpawnTransform = GetActorTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::SelectDefaultAtRuntime;

	// 퍼펫을 스폰하고 인터페이스 확인
	Puppet = GetWorld()->SpawnActor<AActor>(MinionData.Puppet, SpawnTransform, SpawnParams);
	if (!Puppet->Implements<UInterface_CardPuppet>())return;

	// 퍼펫 초기화 (인터페이스 메서드 실행)
	IInterface_CardPuppet::Execute_InitializePuppet(Puppet, this, bSpawnedDuringGameplay);

	// 이벤트 바인딩: 턴 시작 이벤트에 바인딩
	DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_TurnStart);

	// 3. 미니언의 상태 컴포넌트 생성
	for (const TTuple<TSubclassOf<UStatusComponent>, int32>& StartingStatus : MinionData.StartingStatuses)
	{
		TSubclassOf<UStatusComponent> StartingStatusClass = StartingStatus.Key;
		int32 StatusValue = StartingStatus.Value;

		// 클래스의 기본 객체(CDO)를 가져오기
		UStatusComponent* StatusCDO = StartingStatusClass.GetDefaultObject();

		// 고유한 이름 생성 및 상태 컴포넌트 생성
		FName UniqueName = UFunctionLibrary_Utility::GenerateUniqueObjectName(this, StartingStatusClass);
		UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(this, StartingStatusClass, UniqueName);
		NewStatusComponent->RegisterComponent();

		// 컴포넌트의 초기 값 설정
		NewStatusComponent->StatusValue = StatusValue;
		NewStatusComponent->OwnerUiRef = AccessPuppetUI(StatusCDO->SlotType);
		NewStatusComponent->GameplayTags = StatusCDO->GameplayTags;
		NewStatusComponent->bShowImmediately = true;

		// 컴포넌트를 액터에 등록
		AddInstanceComponent(NewStatusComponent);
	}

	// 4. 공격 패턴 컴포넌트 생성
	if (MinionData.AttackPatternComponent)
	{
		FName UniqueName = UFunctionLibrary_Utility::GenerateUniqueObjectName(this, MinionData.AttackPatternComponent);
		UAttackPatternComponent* NewAttackPatternComponent = NewObject<UAttackPatternComponent>(this, MinionData.AttackPatternComponent, UniqueName);
		NewAttackPatternComponent->RegisterComponent();
		NewAttackPatternComponent->AttackPatternData = MinionData.AttackPatternData;

		// 공격 패턴 컴포넌트 액터에 등록
		AddInstanceComponent(NewAttackPatternComponent);
	}
}

void AMinionBase::RemoveFromGame()
{
	Execute_UnMarkTarget(this);

	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_Appear>([this](AAction_Appear* AAction_Appear)
	{
		AAction_Appear->FadeSpeed = 1.0f;
		AAction_Appear->Puppet = Puppet;
		AAction_Appear->bDisappear = true;
		AAction_Appear->EndDelay = -1.0f;
	});

	DispatcherHubLocalComponent->CallEvent(CosGameTags::Event_Death, this, nullptr, ECallGlobal::CallAfter);
	Destroy();
}

FGameplayTagContainer AMinionBase::GetGameplayTags()
{
	return GameplayTagComponent->GameplayTags;
}

UObject* AMinionBase::AccessPuppetUI(EStatusSlot InSlot)
{
	if (Puppet && Puppet->Implements<UInterface_CardPuppet>())
	{
		return IInterface_CardPuppet::Execute_GetPuppetUI(Puppet, InSlot);
	}
	return nullptr;
}

void AMinionBase::UnMarkTarget_Implementation()
{
	IInterface_CardPuppet::Execute_UnmarkPuppet(Puppet);
}

bool AMinionBase::CheckIfValidTarget_Implementation(const FGameplayTagContainer& ValidTargets)
{
	if (GameplayTagComponent)
	{
		return GameplayTagComponent->GameplayTags.HasAny(ValidTargets);
	}

	return false;
}

bool AMinionBase::GetStatusValue_Implementation(TSubclassOf<UStatusComponent> InStatus, int32& OutStatusValue)
{
	UActorComponent* FoundStatusComponent = GetComponentByClass(InStatus);
	if (IsValid(FoundStatusComponent))
	{
		UStatusComponent* StatusComponent = Cast<UStatusComponent>(FoundStatusComponent);
		OutStatusValue = StatusComponent->StatusValue;
		return true;
	}

	OutStatusValue = 0;
	return false;
}

FString AMinionBase::GetMinionUniqueID_Implementation()
{
	return MinionData.UniqueID;
}

bool AMinionBase::SetStatusMax_Implementation(TSubclassOf<UStatusComponent> InStatus, int32 NewMax)
{
	MinionData.StatusLimits.Emplace(InStatus, NewMax);
	return true;
}

bool AMinionBase::GetStatusMax_Implementation(TSubclassOf<UStatusComponent> InStatus, int32& OutMax)
{
	if (MinionData.StatusLimits.Contains(InStatus))
	{
		OutMax = MinionData.StatusLimits[InStatus];
		return true;
	}

	OutMax = 0;
	return false;
}

int32 AMinionBase::SubtractFromStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	return Execute_AddToStatus(this, InStatusClass, -InAmount, bIsShowSplash, InPayLoad);
}

AActor* AMinionBase::GetPuppet_Implementation()
{
	return Puppet;
}

int32 AMinionBase::AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	// 상태 컴포넌트를 찾은 경우
	if (UStatusComponent* FoundStatusComponent = Cast<UStatusComponent>(FindComponentByClass(InStatusClass)))
	{
		return FoundStatusComponent->AddStatusValue(InAmount, bIsShowSplash, bIsShowSplash, false, InPayLoad);
	}

	UStatusComponent* StatusClassCDO = InStatusClass.GetDefaultObject();
	if (InAmount > 0 || StatusClassCDO->bCanBeZero)
	{
		FName UniqueName = UFunctionLibrary_Utility::GenerateUniqueObjectName(this, InStatusClass);
		UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(this, InStatusClass, UniqueName);
		NewStatusComponent->RegisterComponent();

		NewStatusComponent->StatusValue = 0;
		NewStatusComponent->OwnerUiRef = AccessPuppetUI(StatusClassCDO->SlotType);
		NewStatusComponent->bShowImmediately = false;
		NewStatusComponent->GameplayTags = FGameplayTagContainer();
		AddInstanceComponent(NewStatusComponent);

		return NewStatusComponent->AddStatusValue(InAmount, bIsShowSplash, bIsShowSplash, true, InPayLoad);
	}

	return 0;
}

FVector AMinionBase::GetPuppetRelativeCenter_Implementation()
{
	if (Puppet && Puppet->Implements<UInterface_CardPuppet>())
	{
		return IInterface_CardPuppet::Execute_GetPuppetRelativeCenter(Puppet);
	}
	return FVector();
}

bool AMinionBase::AttemptMarkTarget_Implementation(const FGameplayTagContainer& ValidTargets)
{
	if (Execute_CheckIfValidTarget(this, ValidTargets))
	{
		return Execute_MarkTarget(this);
	}
	return false;
}

bool AMinionBase::MarkTarget_Implementation()
{
	if (Puppet && Puppet->Implements<UInterface_CardPuppet>())
	{
		IInterface_CardPuppet::Execute_MarkPuppet(Puppet);
	}
	return true;
}

void AMinionBase::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (!bIsGlobal)
	{
		if (EventTag == CosGameTags::Event_TurnStart)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
			{
				DispatcherHubLocalComponent->CallEvent(CosGameTags::Event_TurnEnd, this, nullptr, ECallGlobal::CallAfter);
			}));
		}
	}
}

FText AMinionBase::GetFriendlyName_Implementation()
{
	return MinionData.Name;
}

float AMinionBase::GetPriority_Implementation()
{
	UStatus_Initiative* Status_Initiative = FindComponentByClass<UStatus_Initiative>();
	if (IsValid(Status_Initiative))
	{
		return Status_Initiative->StatusValue;
	}

	return 8.0f;
}

float AMinionBase::GetInitiative_Implementation()
{
	UStatus_Initiative* Status_Initiative = FindComponentByClass<UStatus_Initiative>();
	if (IsValid(Status_Initiative))
	{
		float LocalInitiative = Status_Initiative->StatusValue;
		if (IsValid(MinionTrack))
		{
			return LocalInitiative + MinionTrack->GetInitiativeFromTrack(this);
		}

		return LocalInitiative;
	}

	return 0.0f;
}
