#include "Core/GlobalDispatcherHub.h"

#include "Core/DispatcherHubComponent.h"


AGlobalDispatcherHub::AGlobalDispatcherHub()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	DispatcherHubComponent = CreateDefaultSubobject<UDispatcherHubComponent>(TEXT("DispatcherHubComponent"));
	DispatcherHubComponent->bGlobal = true;
}

void AGlobalDispatcherHub::BeginPlay()
{
	Super::BeginPlay();
}

void AGlobalDispatcherHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
