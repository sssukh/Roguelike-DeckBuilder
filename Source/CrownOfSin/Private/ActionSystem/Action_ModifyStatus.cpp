

#include "ActionSystem/Action_ModifyStatus.h"


// Sets default values
AAction_ModifyStatus::AAction_ModifyStatus()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_ModifyStatus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_ModifyStatus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

