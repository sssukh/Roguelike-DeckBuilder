#include "Core/MinionTrack.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_UpdateTrack.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Core/MinionBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Utilities/CosLog.h"


// Sets default values
AMinionTrack::AMinionTrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(DefaultSceneRoot);
	BoxComponent->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	BoxComponent->ShapeColor = FColor(138, 223, 116, 255); // R=138, G=223, B=116, A=255
	BoxComponent->SetLineThickness(2.0f);
	BoxComponent->SetBoxExtent(FVector(50.0f, 350.0f, 100.0f));
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

void AMinionTrack::BeginPlay()
{
	Super::BeginPlay();

	GameplayTagComponent->GameplayTags = InitialGameplayTags;
}

AMinionBase* AMinionTrack::AddMinionToTrack(int32 Index, const FMinion& InMinionData, bool bSpawnedDuringGameplay)
{
	// 액션 매니저 서브시스템 가져오기
	UActionManagerSubsystem* ActionManager = GetWorld()->GetSubsystem<UActionManagerSubsystem>();

	// 게임플레이 중에 스폰된 경우 액션을 큐에 추가
	if (bSpawnedDuringGameplay)
	{
		ActionManager->CreateAndQueueAction<AAction_UpdateTrack>([this](AAction_UpdateTrack* UpdateTrackAction)
		{
			UpdateTrackAction->StartDelay = 0.0f;
			UpdateTrackAction->Track = this;
			UpdateTrackAction->EndDelay = -1.0f;
		}, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}

	// 미니언의 스폰 위치 및 회전 설정
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(GetTrackLocationAtIndex(Index, true));
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());

	// 미니언 생성
	AMinionBase* NewMinion = GetWorld()->SpawnActorDeferred<AMinionBase>(AMinionBase::StaticClass(),
	                                                                     SpawnTransform,
	                                                                     nullptr, nullptr,
	                                                                     ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
	                                                                     ESpawnActorScaleMethod::OverrideRootScale);

	// 미니언 정보 설정
	NewMinion->MinionTrack = this;
	NewMinion->bSpawnedDuringGameplay = bSpawnedDuringGameplay;
	NewMinion->MinionData = InMinionData;

	// 미니언 스폰 완료
	NewMinion->FinishSpawning(SpawnTransform);

	// 트랙에 미니언 추가 (중간 삽입 또는 끝에 추가)
	if (Minions.Num() > 0)
	{
		if (Index < Minions.Num() - 1)
		{
			if (IsValid(Minions[Index]))
			{
				Minions.Insert(NewMinion, Index);
			}
			else
			{
				// 선택사항: 필요한 경우 유효하지 않은 Minions[Index]를 처리합니다.
				Minions[Index] = NewMinion;
			}
		}
		else
		{
			// Index가 현재 배열 크기보다 크거나 같은 경우, 배열 크기를 조정 후 추가
			Minions.SetNum(Index + 1, false); // false는 기존 요소들을 유지함을 의미
			Minions[Index] = NewMinion;
		}
	}
	else
	{
		Minions.Add(NewMinion); // 미니언 배열이 비어있는 경우 새로운 미니언 추가
	}

	// 미니언 제거 시 호출될 함수 바인딩
	BindTargetRemoval(NewMinion);
	return NewMinion;
}

void AMinionTrack::BindTargetRemoval(AActor* InTarget)
{
	InTarget->OnDestroyed.AddDynamic(this, &AMinionTrack::RemoveActorIfDestroyed);
}


bool AMinionTrack::GetFrontMinion(AActor*& OutFoundTarget, int32& OutTargetIndex)
{
	if (Minions.IsValidIndex(0))
	{
		OutFoundTarget = Minions[0];
		OutTargetIndex = 0;
		return IsValid(OutFoundTarget);
	}

	return false;
}

bool AMinionTrack::GetBackMinion(AActor*& OutBackTarget, int32& OutBackIndex)
{
	int32 LastIndex = Minions.Num() - 1;
	if (Minions.IsValidIndex(LastIndex))
	{
		OutBackTarget = Minions[LastIndex];
		OutBackIndex = LastIndex;
		return IsValid(OutBackTarget);
	}

	OutBackIndex = 0;
	return false;
}

FVector AMinionTrack::GetTrackLocationAtIndex(int32 TrackIndex, bool bNewSpawn)
{
	// 트랙 상에서 마지막 미니언의 인덱스
	int32 LastMinionIndex = Minions.Num() - 1;

	// 스폰 여부를 반영한 총 미니언 수 계산
	int32 TotalMinionsWithSpawn = (LastMinionIndex + bNewSpawn) + ForwardOffset;

	// 미니언 사이의 간격을 계산 (BoxComponent의 Y축 크기를 기반으로 계산)
	float SpacingBetweenMinions = (1.0f / (TotalMinionsWithSpawn + 2.0f)) * BoxComponent->GetUnscaledBoxExtent().Y;

	// 간격에 스케일을 반영하여 Y축에서의 위치를 계산
	float YPositionOffset = (SpacingBetweenMinions * -2.0f) * GetActorScale3D().Y;

	// 트랙에서 주어진 인덱스의 위치를 계산
	float RelativeIndexPosition = (TrackIndex + ForwardOffset) - (TotalMinionsWithSpawn * 0.5f);

	// 최종 위치 계산 (Y축에 대한 오프셋만 반영)
	FVector LocalLocation(0.0f, RelativeIndexPosition * YPositionOffset, 0.0f);

	// 로컬 위치를 월드 위치로 변환하여 반환
	return GetActorTransform().TransformPosition(LocalLocation);
}

float AMinionTrack::GetInitiativeFromTrack(AMinionBase* InMinion)
{
	// 트랙 상에서 마지막 미니언의 인덱스
	int32 LastMinionIndex = Minions.Num() - 1;

	// 해당 미니언(InMinion)의 인덱스를 찾음
	int32 MinionIndex;
	Minions.Find(InMinion, MinionIndex);

	// 마지막 미니언과의 거리 차이를 계산하여 이니셔티브 값 반환
	return InitiativeModifier + FMath::Max(LastMinionIndex - MinionIndex, 0);
}

void AMinionTrack::RemoveActorIfDestroyed(AActor* DestroyedActor)
{
	// 파괴된 액터가 AMinionBase 타입일 경우에만 처리
	if (AMinionBase* DestroyedMinion = Cast<AMinionBase>(DestroyedActor))
	{
		// 트랙에서 해당 미니언 제거
		Minions.Remove(DestroyedMinion);

		// 액션 매니저 서브시스템 가져오기
		UActionManagerSubsystem* ActionManager = GetWorld()->GetSubsystem<UActionManagerSubsystem>();

		// 트랙 업데이트 작업을 큐에 추가
		ActionManager->CreateAndQueueAction<AAction_UpdateTrack>([this](AAction_UpdateTrack* UpdateTrackAction)
		{
			UpdateTrackAction->StartDelay = 0.0f;
			UpdateTrackAction->Track = this;
			UpdateTrackAction->EndDelay = -1.0f;
		}, ESpawnActorCollisionHandlingMethod::Undefined);
	}
}
