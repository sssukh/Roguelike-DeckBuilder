#include "ActionSystem/Action_DispatcherEvent.h"

#include "Core/DispatcherHubComponent.h"


AAction_DispatcherEvent::AAction_DispatcherEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_DispatcherEvent::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_DispatcherEvent::PlayAction_Implementation()
{
	DispatcherHub->ResolveCallEvent(Event, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
	IInterface_CardAction::Execute_EndAction(this);
}
