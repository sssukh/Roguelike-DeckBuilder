#include "ActionSystem/Action_UpdateTrack.h"

#include "Core/MinionTrack.h"


AAction_UpdateTrack::AAction_UpdateTrack(): StartDelay(0), Track(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_UpdateTrack::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_UpdateTrack::PlayAction_Implementation()
{
	Track->SlideMinionsToPositions(StartDelay);
	Execute_EndAction(this);
}
