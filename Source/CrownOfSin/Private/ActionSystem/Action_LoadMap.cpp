

#include "ActionSystem/Action_LoadMap.h"


// Sets default values
AAction_LoadMap::AAction_LoadMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_LoadMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_LoadMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

