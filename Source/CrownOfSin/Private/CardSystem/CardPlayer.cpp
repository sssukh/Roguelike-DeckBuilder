#include "CardSystem/CardPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
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

	PileDestroyComponent = CreateDefaultSubobject<UPileDestroyComponent>(TEXT("PileDestroyComponent"));
	PileShopComponent = CreateDefaultSubobject<UPileShopComponent>(TEXT("PileShopComponent"));
	PileDrawComponent = CreateDefaultSubobject<UPileDrawComponent>(TEXT("PileDrawComponent"));
	PileDeckComponent = CreateDefaultSubobject<UPileDeckComponent>(TEXT("PileDeckComponent"));
	PileDiscardComponent = CreateDefaultSubobject<UPileDiscardComponent>(TEXT("PileDiscardComponent"));
	PileExhaustComponent = CreateDefaultSubobject<UPileExhaustComponent>(TEXT("PileExhaustComponent"));
	PileHandComponent = CreateDefaultSubobject<UPileHandComponent>(TEXT("PileHandComponent"));
	PileVoidComponent = CreateDefaultSubobject<UPileVoidComponent>(TEXT("PileVoidComponent"));

	PayloadHolderComponent = CreateDefaultSubobject<UPayloadHolderComponent>(TEXT("PayloadHolderComponent"));

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

void ACardPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (!DefaultIMC)
	{
		COS_SCREEN(TEXT("Card Player의 Default IMC를 설정해주세요!!"));
		return;
	}

	if (!WBP_LayoutClass)
	{
		COS_SCREEN(TEXT("WBP Layout CLass를 설정해주세요!!!!!"));
		return;
	}


	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSubsystem->AddMappingContext(DefaultIMC, 0);

	// PileComponent와 관련된 태그를 맵핑하는 함수 호출 (GeneratePileTagLookup)
	GeneratePileTagLookup();

	// DispatcherHubLocalComponent에 이벤트 바인딩: 턴 시작 이벤트와 바인딩
	DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_TurnStart);

	// 글로벌 이벤트 허브에 사망 이벤트 바인딩
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

void ACardPlayer::DisplayScreenLogMessage(FText Message, FColor Color)
{
	PlayerUI->DisplayScreenLogMessage(Message, Color);
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

bool ACardPlayer::IsValidStatusClass(TSubclassOf<UStatusComponent> InStatusClass)
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
	FString ClassName = InStatusClass->GetName();
	FString BaseName = FString::Printf(TEXT("%s_Component"), *ClassName);

	// 고유한 이름을 생성하여 새로운 상태 컴포넌트 생성
	FName UniqueName = MakeUniqueObjectName(this, InStatusClass, FName(*BaseName));
	UStatusComponent* NewStatusComponent = NewObject<UStatusComponent>(this, InStatusClass, UniqueName);
	NewStatusComponent->RegisterComponent();

	NewStatusComponent->StatusValue = 0;
	NewStatusComponent->OwnerUiRef = PlayerUI;
	NewStatusComponent->bShowImmediately = false;
	NewStatusComponent->GameplayTags = FGameplayTagContainer();

	return NewStatusComponent;
}

int32 ACardPlayer::AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad)
{
	// 상태 클래스의 유효성 검사
	if (!IsValidStatusClass(InStatusClass))
	{
		return 0;
	}

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

void ACardPlayer::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	IInterface_StoryEncounter::Execute_InitializeStoryEncounter(PlayerUI, EncounterData, bIsFirstScreen);
}
