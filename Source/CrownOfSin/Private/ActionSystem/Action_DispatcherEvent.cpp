#include "ActionSystem/Action_DispatcherEvent.h"

#include "Core/DispatcherHubComponent.h"


AAction_DispatcherEvent::AAction_DispatcherEvent(): DispatcherHubReference(nullptr), CallingObject(nullptr), CallSpecificObject(nullptr), AlsoCallGlobal(), PayLoad(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAction_DispatcherEvent::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_DispatcherEvent::PlayAction_Implementation()
{
	DispatcherHubReference->ResolveCallEvent(Event, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
	Execute_EndAction(this);
}
