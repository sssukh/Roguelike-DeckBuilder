#include "UI/Actors/TextSplash.h"


// Sets default values
ATextSplash::ATextSplash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void ATextSplash::BeginPlay()
{
	Super::BeginPlay();
}
