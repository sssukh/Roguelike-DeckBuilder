#include "CardSystem/CardActions/ActionBase.h"

#include "Core/ActionManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"


// Sets default values
AActionBase::AActionBase(): ActionManagerRef(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AActionBase::BeginPlay()
{
	Super::BeginPlay();

	SetUpAction();

	AActionManager* ActionManager = Cast<AActionManager>(UGameplayStatics::GetActorOfClass(this, AActionManager::StaticClass()));
	if (!ActionManager)
	{
		COS_LOG_SCREEN(TEXT("World에 ActionManager가 존재하지 않습니다. "));
		return;
	}

	ActionManager->QueueAction(this);
}

void AActionBase::AnimateAction_Implementation(UObject* CallingActionManager)
{
	ActionManagerRef = CallingActionManager;
	PlayAction();
}

void AActionBase::EndAction_Implementation()
{
	if (EndDelay >= 0.0f)
	{
		FTimerHandle EndDelayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EndDelayTimerHandle, [this]()
		{
			IInterface_CardActionManager::Execute_ProceedFromOngoingAction(ActionManagerRef, this);
		}, EndDelay, false); // 반복 없이 한 번만 실행
	}
	else
	{
		IInterface_CardActionManager::Execute_ProceedFromOngoingAction(ActionManagerRef, this);
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
}
