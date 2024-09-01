#include "Core/RewardHolder.h"


#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/CosGameplayTags.h"


ARewardHolder::ARewardHolder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	DispatcherHubComponentLocal = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubComponentLocal"));
}

void ARewardHolder::BeginPlay()
{
	Super::BeginPlay();

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_CloseRewardScreen);
}

void ARewardHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARewardHolder::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_CloseRewardScreen)
	{
		TArray<UActorComponent*> StatusComponents;
		GetComponents(UStatusComponent::StaticClass(), StatusComponents);

		for (UActorComponent* StatusComponent : StatusComponents)
		{
			StatusComponent->DestroyComponent();
		}
	}
}
