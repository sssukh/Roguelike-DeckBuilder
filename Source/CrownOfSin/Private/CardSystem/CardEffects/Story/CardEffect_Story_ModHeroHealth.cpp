#include "CardSystem/CardEffects/Story/CardEffect_Story_ModHeroHealth.h"

#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Health.h"


UCardEffect_Story_ModHeroHealth::UCardEffect_Story_ModHeroHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UCardEffect_Story_ModHeroHealth::BeginPlay()
{
	Super::BeginPlay();
}

bool UCardEffect_Story_ModHeroHealth::ResolveCardEffect(AActor* TargetActor)
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance)
		return false;

	const TArray<FMinion>& PersistentHeroesFromInstance = IInterface_CardGameInstance::Execute_GetPersistentHeroesFromInstance(CardGameInstance);

	for (const FMinion& Hero : PersistentHeroesFromInstance)
	{
		if (!Hero.StartingStatuses.Contains(UStatus_Health::StaticClass()) || !Hero.StatusLimits.Contains(UStatus_Health::StaticClass()))
		{
			continue;
		}

		int32 Health = Hero.StartingStatuses[UStatus_Health::StaticClass()];
		int32 HealthLimit = Hero.StatusLimits[UStatus_Health::StaticClass()];
		int32 NewHealth = FMath::Max(FMath::Min(Health + EffectValue, HealthLimit), 0);

		IInterface_CardGameInstance::Execute_UpdateHeroPersistentHealth(CardGameInstance, Hero.UniqueID, NewHealth);
	}


	return true;
}
