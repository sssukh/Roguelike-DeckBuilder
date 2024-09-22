#include "ActionSystem/Action_Appear.h"

#include "Interfaces/Interface_CardPuppet.h"


AAction_Appear::AAction_Appear(): Puppet(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAction_Appear::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_Appear::PlayAction_Implementation()
{
	if (!Puppet->Implements<UInterface_CardPuppet>()) return;

	if (bDisappear)
		IInterface_CardPuppet::Execute_DisappearPuppet(Puppet, FadeSpeed);
	else
		IInterface_CardPuppet::Execute_AppearPuppet(Puppet, FadeSpeed);
	
	Execute_EndAction(this);
}
