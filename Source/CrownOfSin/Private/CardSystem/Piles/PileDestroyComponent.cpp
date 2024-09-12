#include "CardSystem/Piles/PileDestroyComponent.h"
#include "CardSystem/CardBase.h"
#include "Utilities/CosGameplayTags.h"


UPileDestroyComponent::UPileDestroyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval = 3.0f;

	// ...
	FriendlyName = FText::FromString(FString(TEXT("Pending Destruction")));
	PileTag = CosGameTags::Pile_Destroy;
	Tooltip.RowName = FName(TEXT("DestructionPile"));
}

void UPileDestroyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPileDestroyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Cards.Num() <= 0) return;


	if (bDoOnce)
	{
		bDoOnce = false;

		for (ACardBase* Card : Cards)
		{
			Card->Destroy();
		}

		Cards.Empty();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			bDoOnce = true;
		}, ResetDelay, false);
	}
}
