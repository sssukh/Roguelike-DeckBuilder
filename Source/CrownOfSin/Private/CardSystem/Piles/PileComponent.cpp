#include "CardSystem/Piles/PileComponent.h"

#include "Utilities/CosGameplayTags.h"


UPileComponent::UPileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;


	PileTag = CosGameTags::Pile;
	// ...
}

void UPileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
