#include "CardSystem/CardEffects/CardEffect_ShowRewardScreen.h"

#include "ActionSystem/Action_RewardScreen.h"


// Sets default values for this component's properties
UCardEffect_ShowRewardScreen::UCardEffect_ShowRewardScreen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardEffect_ShowRewardScreen::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_ShowRewardScreen::ResolveCardEffect(AActor* TargetActor)
{
	FTransform SpawnTransform = FTransform::Identity;

	if (AAction_RewardScreen* NewActionRewardScreen = GetWorld()->SpawnActorDeferred<AAction_RewardScreen>(AAction_RewardScreen::StaticClass(), SpawnTransform,
	                                                                                                       nullptr, nullptr,
	                                                                                                       ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		NewActionRewardScreen->FinishSpawning(SpawnTransform);
	}

	return true;
}
