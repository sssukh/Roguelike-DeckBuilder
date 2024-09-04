#include "CardSystem/CardPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

#include "UI/UW_Layout_Cos.h"



// Sets default values

ACardPlayer::ACardPlayer(): ChanceManagerComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	DispatcherHubLocalComponent = CreateDefaultSubobject<UDispatcherHubLocalComponent>(TEXT("DispatcherHubLocalComponent"));
	ChanceManagerComponent = CreateDefaultSubobject<UChanceManagerComponent>(TEXT("ChanceManagerComponent"));
}

void ACardPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (DefaultIMC)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		InputSubsystem->AddMappingContext(DefaultIMC, 0);
	}
	else
	{
		COS_LOG_SCREEN(TEXT("Card Player의 Default IMC를 설정해주세요!!"));
		return;
	}


	GeneratePileTagLookup();

	DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_TurnStart);

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_Death);
	
	if (!WBP_LayoutClass)
	{
		COS_LOG_SCREEN(TEXT("WBP Layout CLass를 설정해주세요!!!!!"));
		return;
	}
	PlayerUI = CreateWidget<UUW_Layout_Cos>(PlayerController, WBP_LayoutClass);
	PlayerUI->AddToViewport();


	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	TArray<FStatusData> Artifacts = IInterface_CardGameInstance::Execute_GetArtifactsFromInstance(GameInstance);

	for (const FStatusData& Artifact : Artifacts)
	{
		Execute_AddToStatus(this, Artifact.StatusClass, Artifact.Value, false, this);
	}

	PlayerController->bShowMouseCursor = true;
}

void ACardPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACardPlayer::DisplayScreenLogMessage(FText Message, FColor Color)
{
	PlayerUI->DisplayScreenLogMessage(Message,Color);
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

int32 ACardPlayer::AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	if (!InStatusClass->IsChildOf(UStatusComponent::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("InStatusClass는 UStatusComponent의 하위 클래스가 아닙니다."));
		return 0;
	}

	UStatusComponent* StatusComponent = Cast<UStatusComponent>(FindComponentByClass(InStatusClass));
	if (IsValid(StatusComponent))
	{
		return StatusComponent->AddStatusValue(InAmount, false, false, false, InPayLoad);
	}

	if (InAmount > 0 || GetDefault<UStatusComponent>(InStatusClass)->bCanBeZero)
	{
		FString ClassName = InStatusClass->GetName();
		FString BaseName = FString::Printf(TEXT("%s_Component"), *ClassName);

		FName UniqueName = MakeUniqueObjectName(this, InStatusClass, FName(*BaseName));
		UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(this, InStatusClass, UniqueName);
		NewStatusComponent->RegisterComponent();

		NewStatusComponent->StatusValue = 0;
		NewStatusComponent->OwnerUiRef = PlayerUI;
		NewStatusComponent->bShowImmediately = false;
		NewStatusComponent->GameplayTags = FGameplayTag();

		NewStatusComponent->AddStatusValue(InAmount, false, false, true, InPayLoad);
		return InAmount;
	}

	return 0;
}

void ACardPlayer::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	PlayerUI->InitializeStoryEncounter(EncounterData,bIsFirstScreen);
}
