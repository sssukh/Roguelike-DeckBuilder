#include "Core/RewardHolder.h"


#include "CardSystem/CardPlayer.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_Layout_Cos.h"
#include "Utilities/CosGameplayTags.h"
#include "UI/UW_RewardScreen.h"


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

int32 ARewardHolder::AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	UActorComponent* StatusComponentFound = GetComponentByClass(InStatusClass);

	if (IsValid(StatusComponentFound))
	{
		UStatusComponent* StatusComponent = Cast<UStatusComponent>(StatusComponentFound);
		return StatusComponent->AddStatusValue(InAmount, false, false, false, InPayLoad);
	}

	// InAmount가 0 이상이거나 그렇지 않은경우 bCanBeZero의 여부를 확인하기위해 CDO를 가져와서 확인
	if (InAmount > 0 || Cast<UStatusComponent>(InStatusClass->GetDefaultObject())->bCanBeZero)
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		UStatusComponent* NewStatus = NewObject<UStatusComponent>(this, InStatusClass);
		NewStatus->OwnerUiRef = CardPlayer->PlayerUI->WBP_RewardScreen;
		NewStatus->RegisterComponent();
		NewStatus->AddStatusValue(InAmount, false, false, true, InPayLoad);

		return InAmount;
	}

	return 0;
}
