#include "ActionSystem/Action_AdvAnim.h"

#include "CombatSystem/Puppets/PuppetComponent.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "Libraries/FunctionLibrary_Utility.h"


AAction_AdvAnim::AAction_AdvAnim(): Puppet(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_AdvAnim::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_AdvAnim::PlayAction_Implementation()
{
	if (!UFunctionLibrary_Utility::CheckObjectImplementsInterface(this, Puppet, UInterface_CardPuppet::StaticClass())) return;

	IInterface_CardPuppet::Execute_AnimatePuppet(Puppet, Animation);

	UPuppetComponent* PuppetComponent = Puppet->FindComponentByClass<UPuppetComponent>();
	if (!PuppetComponent) return;

	PuppetComponent->OnAnimationEvent.AddDynamic(this, &AAction_AdvAnim::CheckAnimationEvent);

	IInterface_CardPuppet::Execute_AnimatePuppet(Puppet, Animation);
}

void AAction_AdvAnim::CheckAnimationEvent(AActor* InPuppet, FGameplayTag InAnimationTag)
{
	if (InAnimationTag == Animation)
	{
		UPuppetComponent* PuppetComponent = Puppet->FindComponentByClass<UPuppetComponent>();
		PuppetComponent->OnAnimationEvent.RemoveDynamic(this, &AAction_AdvAnim::CheckAnimationEvent);
		Execute_EndAction(this);
	}
}
