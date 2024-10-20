﻿#include "CardSystem/CardPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardTransferComponent.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "CardSystem/Piles/PileComponent.h"
#include "CardSystem/Piles/PileDeckComponent.h"
#include "CardSystem/Piles/PileDestroyComponent.h"
#include "CardSystem/Piles/PileDiscardComponent.h"
#include "CardSystem/Piles/PileDrawComponent.h"
#include "CardSystem/Piles/PileExhaustComponent.h"
#include "CardSystem/Piles/PileShopComponent.h"
#include "CardSystem/Piles/PileVoidComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Core/PayloadHolderComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/StatusComponent.h"
#include "StatusSystem/Status_Draw.h"
#include "StatusSystem/Status_Initiative.h"
#include "StatusSystem/Status_Mana.h"
#include "StatusSystem/Status_ManaGain.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

#include "UI/UW_Layout_Cos.h"


ACardPlayer::ACardPlayer(): ChanceManagerComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	DispatcherHubLocalComponent = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubLocalComponent"));

	ChanceManagerComponent = CreateDefaultSubobject<UChanceManagerComponent>(TEXT("ChanceManagerComponent"));

	//Status Component 생성
	{
		Status_Initiative = CreateDefaultSubobject<UStatus_Initiative>(TEXT("Status_Initiative"));
		Status_Initiative->StatusValue = 10000;

		Status_ManaGain = CreateDefaultSubobject<UStatus_ManaGain>(TEXT("Status_ManaGain"));
		Status_ManaGain->StatusValue = 3;

		Status_Draw = CreateDefaultSubobject<UStatus_Draw>(TEXT("Status_Draw"));
		Status_Draw->StatusValue = 5;

		Status_Mana = CreateDefaultSubobject<UStatus_Mana>(TEXT("Status_Mana"));
	}

	//Pile Component 생성
	{
		PileDestroyComponent = CreateDefaultSubobject<UPileDestroyComponent>(TEXT("PileDestroyComponent"));
		PileShopComponent = CreateDefaultSubobject<UPileShopComponent>(TEXT("PileShopComponent"));
		PileDrawComponent = CreateDefaultSubobject<UPileDrawComponent>(TEXT("PileDrawComponent"));
		PileDeckComponent = CreateDefaultSubobject<UPileDeckComponent>(TEXT("PileDeckComponent"));
		PileDiscardComponent = CreateDefaultSubobject<UPileDiscardComponent>(TEXT("PileDiscardComponent"));
		PileExhaustComponent = CreateDefaultSubobject<UPileExhaustComponent>(TEXT("PileExhaustComponent"));

		PileHandComponent = CreateDefaultSubobject<UPileHandComponent>(TEXT("PileHandComponent"));
		PileHandComponent->MaxPileSize = 12;

		PileVoidComponent = CreateDefaultSubobject<UPileVoidComponent>(TEXT("PileVoidComponent"));
	}


	PayloadHolderComponent = CreateDefaultSubobject<UPayloadHolderComponent>(TEXT("PayloadHolderComponent"));

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));

	CardTransferComponent = CreateDefaultSubobject<UCardTransferComponent>(TEXT("CardTransferComponent"));
}

void ACardPlayer::BeginPlay()
{
	Super::BeginPlay();


	COS_IF_CHECK_VOID(DefaultIMC, TEXT("Card Player의 Default IMC를 설정해주세요!!"));
	COS_IF_CHECK_VOID(WBP_LayoutClass, TEXT("Card Player의 WBP Layout CLass를 설정해주세요!!!!!"));
	
	 APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSubsystem->AddMappingContext(DefaultIMC, 0);

	GeneratePileTagLookup();

	DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_TurnStart);

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_Death);

	PlayerUI = CreateWidget<UUW_Layout_Cos>(PlayerController, WBP_LayoutClass);
	PlayerUI->AddToViewport();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	const TArray<FStatusData>& Artifacts = IInterface_CardGameInstance::Execute_GetArtifactsFromInstance(GameInstance);
	for (const FStatusData& Artifact : Artifacts)
	{
		Execute_AddToStatus(this, Artifact.StatusClass, Artifact.Value, false, this);
	}

	PlayerController->bShowMouseCursor = true;
}

void ACardPlayer::GeneratePileTagLookup()
{
	TArray<UActorComponent*> PileComponents;
	GetComponents(UPileComponent::StaticClass(), PileComponents);

	for (UActorComponent* PileActorComponent : PileComponents)
	{
		UPileComponent* PileComponent = Cast<UPileComponent>(PileActorComponent);

		if (!PileComponent)
			continue;

		PileTagLookup.Add(PileComponent->PileTag, PileComponent);
	}
}

bool ACardPlayer::GetPileWithPileTag(FGameplayTag PileTag, UPileComponent*& OutPileComponent)
{
	if (PileTagLookup.Contains(PileTag))
	{
		OutPileComponent = PileTagLookup[PileTag];
		return true;
	}

	OutPileComponent = nullptr;
	return false;
}

bool ACardPlayer::IsValidStatusClass(const TSubclassOf<UStatusComponent>& InStatusClass)
{
	if (!InStatusClass->IsChildOf(UStatusComponent::StaticClass()))
	{
		COS_LOG_ERROR(TEXT("InStatusClass는 UStatusComponent의 하위 클래스가 아닙니다."));
		return false;
	}
	return true;
}

UStatusComponent* ACardPlayer::CreateNewStatusComponent(TSubclassOf<UStatusComponent> InStatusClass)
{
	// 고유한 이름을 생성하여 새로운 상태 컴포넌트 생성
	FString ClassName = InStatusClass->GetName();
	FString BaseName = FString::Printf(TEXT("%s_Component"), *ClassName);
	FName UniqueName = MakeUniqueObjectName(this, InStatusClass, FName(*BaseName));

	UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(this, InStatusClass, UniqueName);
	if (NewStatusComponent)
	{
		// NewStatusComponent->RegisterComponent();

		NewStatusComponent->StatusValue = 0;
		NewStatusComponent->OwnerUiRef = PlayerUI;
		NewStatusComponent->bShowImmediately = false;
		NewStatusComponent->GameplayTags = FGameplayTagContainer();

		// 바뀐 위치
		NewStatusComponent->RegisterComponent();

		AddInstanceComponent(NewStatusComponent);
	}
	return NewStatusComponent;
}

void ACardPlayer::DisplayScreenLogMessage(FText Message, FColor Color)
{
	PlayerUI->DisplayScreenLogMessage(Message, Color);
}

int32 ACardPlayer::AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	// 상태 클래스의 유효성 검사
	if (!IsValidStatusClass(InStatusClass)) return 0;

	// 상태 컴포넌트를 찾거나 새로 생성한 후 상태 값을 추가
	UStatusComponent* StatusComponent = Cast<UStatusComponent>(FindComponentByClass(InStatusClass));

	// 상태 컴포넌트를 찾은 경우
	if (StatusComponent)
	{
		return StatusComponent->AddStatusValue(InAmount, false, false, false, InPayLoad);
	}

	// 상태 컴포넌트를 찾지 못한 경우 새로운 컴포넌트를 생성
	StatusComponent = CreateNewStatusComponent(InStatusClass);
	if (StatusComponent)
	{
		StatusComponent->AddStatusValue(InAmount, false, false, true, InPayLoad);
		return InAmount;
	}

	return 0;
}

bool ACardPlayer::CheckIfValidTarget_Implementation(const FGameplayTagContainer& ValidTargets)
{
	return ValidTargets.HasTagExact(CosGameTags::Target_CardPlayer);
}

bool ACardPlayer::GetStatusValue_Implementation(TSubclassOf<UStatusComponent> InStatus, int32& OutStatusValue)
{
	if (UActorComponent* FoundStatus = GetComponentByClass(InStatus))
	{
		UStatusComponent* StatusComponent = Cast<UStatusComponent>(FoundStatus);
		OutStatusValue = StatusComponent->StatusValue;
		return true;
	}
	OutStatusValue = 0;
	return false;
}

int32 ACardPlayer::SubtractFromStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	return Execute_AddToStatus(this, InStatusClass, -InAmount, bIsShowSplash, InPayLoad);
}

void ACardPlayer::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (!bIsGlobal)
	{
		if (EventTag == CosGameTags::Event_TurnStart)
		{
			UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_NewPlayerTurn, this);
		}
	}
	else
	{
		if (EventTag == CosGameTags::Event_Death)
		{
			AActor* CallingActor = Cast<AActor>(CallingObject);
			if (!CallingActor) return;

			UGameplayTagComponent* CollingGameplayTagComponent = CallingActor->FindComponentByClass<UGameplayTagComponent>();
			if (!CollingGameplayTagComponent) return;

			if (CollingGameplayTagComponent->GameplayTags.HasTagExact(CosGameTags::Target_Hero))
			{
				PileDrawComponent->RemoveAllCardsOwnedByActor(CallingActor);
				PileDiscardComponent->RemoveAllCardsOwnedByActor(CallingActor);
				PileHandComponent->RemoveAllCardsOwnedByActor(CallingActor);
			}
		}
	}
}

FText ACardPlayer::GetFriendlyName_Implementation()
{
	return FText::FromString("Card Player");
}

float ACardPlayer::GetPriority_Implementation()
{
	return Status_Initiative->StatusValue;
}

void ACardPlayer::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	Execute_InitializeStoryEncounter(PlayerUI, EncounterData, bIsFirstScreen);
}

float ACardPlayer::GetInitiative_Implementation()
{
	return Status_Initiative->StatusValue;
}
