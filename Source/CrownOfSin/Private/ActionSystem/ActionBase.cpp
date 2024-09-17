#include "ActionSystem/ActionBase.h"
#include "ActionSystem/ActionManagerSubsystem.h"


AActionBase::AActionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AActionBase::BeginPlay()
{
	Super::BeginPlay();

	SetUpAction();

	if (UWorld* World = GetWorld())
	{
		if (UActionManagerSubsystem* ActionManagerSubsystem = World->GetSubsystem<UActionManagerSubsystem>())
		{
			ActionManagerSubsystem->QueueAction(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ActionManagerSubsystem을 가져올 수 없습니다."));
		}
	}
}

void AActionBase::AnimateAction_Implementation(UObject* CallingActionManager)
{
	// 수정 전 코드
	// ActionManagerReference = CallingActionManager;
	PlayAction();
}

void AActionBase::EndAction_Implementation()
{
	if (EndDelay >= 0.0f)
	{
		FTimerHandle EndDelayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EndDelayTimerHandle, [this]()
		{
			if (UWorld* World = GetWorld())
			{
				if (UActionManagerSubsystem* ActionManagerSubsystem = World->GetSubsystem<UActionManagerSubsystem>())
				{
					ActionManagerSubsystem->ProceedFromOngoingAction(this);
				}
			}
		}, EndDelay, false);
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			if (UActionManagerSubsystem* ActionManagerSubsystem = World->GetSubsystem<UActionManagerSubsystem>())
			{
				ActionManagerSubsystem->ProceedFromOngoingAction(this);
			}
		}
	}
}

bool AActionBase::AttemptDestroyAction_Implementation()
{
	Destroy();
	return true;
}

void AActionBase::SetUpAction_Implementation()
{
}

void AActionBase::PlayAction_Implementation()
{
	EndAction();
}
