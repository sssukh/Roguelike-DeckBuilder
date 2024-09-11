#include "Core/CosGameInstance.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDeckComponent.h"
#include "Core/CosSaveGame.h"
#include "Core/MinionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosLog.h"
#include "StatusSystem/Status_Health.h"
#include "StatusSystem/Artifacts/Status_Artifact.h"
#include "Utilities/CosGameplayTags.h"

UCosGameInstance::UCosGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SaveId = FString(TEXT("Test"));
	CurrentNodeMap = FString(TEXT("NodeMap"));
	HeroUniqueId = FName(TEXT("Hero"));

	VisitedNodes.Add(0);
}

void UCosGameInstance::Init()
{
	Super::Init();

	Execute_ResetGame(this);

	SetGraphicsQuality(GlobalGraphicsQuality);

	for (FHeroDeck StartingHero : StartingHeroes)
	{
		FString UniqueID;
		Execute_AddPersistentHeroToInstance(this, StartingHero.Hero, StartingHero.Deck, UniqueID);
	}
}

UCosSaveGame* UCosGameInstance::GetSavedGameOrMakeIfInvalid(const FString& InSaveID)
{
	if (UGameplayStatics::DoesSaveGameExist(InSaveID, 0))
	{
		UCosSaveGame* LoadedSaveGame = Cast<UCosSaveGame>(UGameplayStatics::LoadGameFromSlot(InSaveID, 0));
		return LoadedSaveGame;
	}

	// 저장된 게임이 존재하지 않는 경우, 새로운 저장 게임 객체를 생성합니다.
	return Cast<UCosSaveGame>(UGameplayStatics::CreateSaveGameObject(UCosSaveGame::StaticClass()));
}

void UCosGameInstance::ClearPersistentData()
{
	PersistentHeroes.Empty();
	Deck.Empty();
	CurrentMinions.Empty();
	Artifacts.Empty();
	DoneStoryEncounters.Empty();

	const UCosGameInstance* DefaultGameInstance = GetDefault<UCosGameInstance>();
	VisitedNodes = DefaultGameInstance->VisitedNodes;
	HeroUniqueId = DefaultGameInstance->HeroUniqueId;
	CurrentNodeMap = DefaultGameInstance->CurrentNodeMap;
}

FString UCosGameInstance::MakeUniqueHeroId(FString LeadingString)
{
	FString GeneratedID;
	bool bIsDuplicate;

	do
	{
		// 랜덤 정수를 생성하고 이를 문자열로 변환
		int32 RandomNumber = UKismetMathLibrary::RandomInteger(INT32_MAX);
		FString RandomNumberString = UKismetStringLibrary::Conv_IntToString(RandomNumber);

		// LeadingString과 RandomNumberString을 결합하여 새로운 ID 생성
		GeneratedID = UKismetStringLibrary::Concat_StrStr(LeadingString, RandomNumberString);

		// ID가 고유한지 확인
		FMinion FoundHero;
		int32 FoundIndex;
		bIsDuplicate = Execute_GetHeroWithIdFromInstance(this, GeneratedID, FoundHero, FoundIndex);
	}
	while (bIsDuplicate);

	return GeneratedID;;
}

TMap<TSubclassOf<UStatusComponent>, int32> UCosGameInstance::FilterOutArtifactsFromMinion(const FMinion& InMinion)
{
	TMap<TSubclassOf<UStatusComponent>, int32> FilteredStatuses;
	for (TTuple<TSubclassOf<UStatusComponent>, int32> StatusEntry : InMinion.StartingStatuses)
	{
		TSubclassOf<UStatusComponent> StatusClass = StatusEntry.Key;
		int32 StatusValue = StatusEntry.Value;

		// 올바른 방법으로 자식 클래스를 확인합니다.
		if (!StatusClass->IsChildOf(UStatus_Artifact::StaticClass()))
		{
			FilteredStatuses.Add(StatusClass, StatusValue);
		}
	}

	return FilteredStatuses;
}

void UCosGameInstance::SetGraphicsQuality(int32 InQuality)
{
	// 조정할 그래픽 설정을 저장하는 배열입니다.
	TArray<FString> GraphicsSettings;
	GraphicsSettings.Add(FString(TEXT("sg.ShadowQuality"))); // 그림자 품질 설정
	GraphicsSettings.Add(FString(TEXT("sg.ViewDistanceQuality"))); // 시야 거리 품질 설정
	GraphicsSettings.Add(FString(TEXT("sg.AntiAliasingQuality"))); // 앤티 앨리어싱 품질 설정
	GraphicsSettings.Add(FString(TEXT("sg.PostProcessQuality"))); // 후처리 품질 설정
	GraphicsSettings.Add(FString(TEXT("sg.TextureQuality"))); // 텍스처 품질 설정

	// 입력된 품질 값을 0에서 3 사이로 클램프하여 전역 그래픽 품질에 설정합니다.
	GlobalGraphicsQuality = FMath::Clamp(InQuality, 0, 3);

	// 그래픽 품질을 문자열로 변환합니다
	FString GraphicsQualityString = UKismetStringLibrary::Conv_IntToString(GlobalGraphicsQuality);

	// 각 그래픽 설정에 대해 콘솔 명령을 생성하고 실행합니다.
	for (FString GraphicsSetting : GraphicsSettings)
	{
		// 설정 이름과 품질 수준을 결합하여 콘솔 명령을 생성합니다.
		FString Command = UKismetStringLibrary::Concat_StrStr(GraphicsSetting, GraphicsQualityString);
		// 콘솔 명령을 실행하여 그래픽 설정을 적용합니다.
		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command, nullptr);
	}

	// 해상도 품질 설정을 위한 문자열입니다.
	FString ResolutionQualitySetting = FString(TEXT("sg.ResolutionQuality"));

	// 해상도 품질 값을 설정합니다.
	if (GlobalGraphicsQuality == 0)
	{
		// 그래픽 품질이 최저(0)일 때 해상도를 60으로 설정
		FString Command = UKismetStringLibrary::Concat_StrStr(ResolutionQualitySetting, FString(TEXT("60")));
		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command, nullptr);
	}
	else
	{
		// 그래픽 품질이 1 이상일 때 해상도를 100으로 설정
		FString Command = UKismetStringLibrary::Concat_StrStr(ResolutionQualitySetting, FString(TEXT("100")));
		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command, nullptr);
	}
}

void UCosGameInstance::SetFilteredStartingStatusesForMinion(FMinion& Minion, const FString& UniqueID)
{
	// 아티팩트를 필터링하여 영웅의 시작 상태를 설정
	const TMap<TSubclassOf<UStatusComponent>, int32>& FilteredStatuses = FilterOutArtifactsFromMinion(Minion);
	Minion.UniqueID = UniqueID;
	Minion.StartingStatuses = FilteredStatuses;

	// 건강 상태를 설정
	SetHealthStatusLimits(Minion);

	// 영웅을 영구적으로 게임 인스턴스에 추가
	PersistentHeroes.Add(Minion);
}

void UCosGameInstance::SetHealthStatusLimits(FMinion& Minion)
{
	TSubclassOf<UStatusComponent> HealthStatusClass = UStatus_Health::StaticClass();

	if (Minion.StartingStatuses.Contains(HealthStatusClass))
	{
		int32 StatusValue = Minion.StartingStatuses[HealthStatusClass];

		// 건강 상태만 포함된 상태 제한을 설정
		TMap<TSubclassOf<UStatusComponent>, int32> NewStatusLimits;
		NewStatusLimits.Add(HealthStatusClass, StatusValue);

		// Minion의 상태 제한을 업데이트
		Minion.StatusLimits = NewStatusLimits;
	}
}

bool UCosGameInstance::AssignHeroDeckToMinion(FDataTableRowHandle HeroDeck, const FString& UniqueID)
{
	FDeck* FoundDeck = HeroDeck.DataTable->FindRow<FDeck>(HeroDeck.RowName, TEXT(""));
	if (!FoundDeck)
	{
		return false;
	}

	// 덱의 카드들을 찾아서 설정
	for (FDataTableRowHandle CardRowHandle : FoundDeck->Cards)
	{
		FCard* FoundCard = CardRowHandle.DataTable->FindRow<FCard>(CardRowHandle.RowName, TEXT(""));
		if (!FoundCard)
		{
			continue;
		}

		// 카드에 소유자 ID 설정
		FoundCard->OwnerID = UniqueID;
		Deck.Add(*FoundCard);
	}

	return true;
}

bool UCosGameInstance::AttemptSaveGame_Implementation(const FString& InLevelNameOverride, bool bIsUpdateDeck)
{
	if (bIsUpdateDeck)
	{
		if (ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(this, ACardPlayer::StaticClass())))
		{
			UPileDeckComponent* PileDeckComponent = CardPlayer->FindComponentByClass<UPileDeckComponent>();
			Execute_UpdateDeckInInstance(this, PileDeckComponent->Cards);
		}
	}


	if (!UKismetStringLibrary::NotEqual_StrStr(SaveId, FString(TEXT("Test"))))
	{
		return false;
	}

	UCosSaveGame* SaveGame = GetSavedGameOrMakeIfInvalid(SaveId);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	FString LevelName = InLevelNameOverride.IsEmpty() ? CurrentLevelName : InLevelNameOverride;

	SaveGame->SavePersistentData(PersistentHeroes, Deck, VisitedNodes, Artifacts, LevelName, CurrentEncounter, CurrentNodeMap, RewardTables, AllowedCardRewardTags, DoneStoryEncounters);

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveId, 0);

	return true;
}

bool UCosGameInstance::AttemptLoadGame_Implementation(const FString& InSaveId, bool bOpenLevelIfValid)
{
	SaveId = InSaveId;

	if (!UGameplayStatics::DoesSaveGameExist(SaveId, 0))
	{
		return false;
	}

	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(InSaveId, 0);
	UCosSaveGame* CosSaveGame = Cast<UCosSaveGame>(SaveGame);
	if (!CosSaveGame)
	{
		COS_LOG_SCREEN(TEXT("SaveGame 타입이 CosSaveGame 으로 캐스팅에 실패했습니다. "));
		return false;
	}

	PersistentHeroes = CosSaveGame->PersistentHeroes;
	Deck = CosSaveGame->Deck;
	VisitedNodes = CosSaveGame->VisitedNodes;
	Artifacts = CosSaveGame->Artifacts;
	CurrentEncounter = CosSaveGame->CurrentEncounter;
	CurrentNodeMap = CosSaveGame->CurrentNodeMap;
	RewardTables = CosSaveGame->RewardTables;
	AllowedCardRewardTags = CosSaveGame->AllowedCardRewardTags;
	DoneStoryEncounters = CosSaveGame->DoneStoryEncounters;

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

	if (UKismetStringLibrary::EqualEqual_StrStr(CurrentLevelName, CosSaveGame->LevelName))
	{
		return true;
	}

	if (UKismetStringLibrary::EqualEqual_StrStr(CosSaveGame->LevelName, FString(TEXT("None"))))
	{
		return true;
	}

	if (bOpenLevelIfValid)
	{
		UGameplayStatics::OpenLevel(this, FName(*CosSaveGame->LevelName));
	}

	return true;
}

void UCosGameInstance::ResetGame_Implementation()
{
	ClearPersistentData();

	for (FDataTableRowHandle StartingArtifact : StartingArtifacts)
	{
		if (FStatusData* FindStatus = StartingArtifact.DataTable->FindRow<FStatusData>(StartingArtifact.RowName,TEXT("")))
		{
			Artifacts.Add(*FindStatus);
		}
	}
}

bool UCosGameInstance::AddPersistentHeroToInstance_Implementation(FDataTableRowHandle HeroData, FDataTableRowHandle HeroDeck, FString& OutUniqueID)
{
	// 영웅 데이터를 가져오고 유효성 검사
	FMinion* FoundMinion = HeroData.DataTable->FindRow<FMinion>(HeroData.RowName,TEXT(""));
	if (!FoundMinion)
	{
		return false;
	}

	// 고유한 영웅 ID 생성
	FString UniqueID = MakeUniqueHeroId(HeroData.RowName.ToString());

	// 영웅의 시작 상태에서 아티팩트를 필터링하고 상태를 설정
	SetFilteredStartingStatusesForMinion(*FoundMinion, UniqueID);

	// 영웅의 덱을 설정
	if (!AssignHeroDeckToMinion(HeroDeck, UniqueID))
	{
		return false;
	}

	// 생성된 고유한 영웅 ID를 반환 매개 변수에 설정
	OutUniqueID = UniqueID;

	return true;
}

bool UCosGameInstance::GetHeroWithIdFromInstance_Implementation(const FString& InUniqueID, FMinion& OutHero, int32& OutArrayIndex)
{
	int32 CurrentIndex = 0; // 영웅 목록을 순회하면서 인덱스를 추적하기 위한 변수

	// 인스턴스의 모든 영웅을 순회합니다.
	for (const FMinion& PersistentHero : PersistentHeroes)
	{
		// 현재 영웅의 고유 ID가 입력된 ID와 동일한지 검사합니다.
		if (UKismetStringLibrary::EqualEqual_StrStr(PersistentHero.UniqueID, InUniqueID))
		{
			OutHero = PersistentHero;
			OutArrayIndex = CurrentIndex;
			return true; // 영웅을 성공적으로 찾았으므로 true를 반환합니다.
		}
		CurrentIndex++;
	}

	return false; // 입력된 ID와 일치하는 영웅을 찾지 못한 경우 false를 반환하여 실패를 알립니다. 
}

bool UCosGameInstance::AddArtifactToInstance_Implementation(const FStatusData& InArtifact)
{
	// 아티팩트 목록을 순회하면서 주어진 아티팩트와 동일한 클래스가 있는지 확인합니다.
	for (FStatusData& ExistingArtifact : Artifacts)
	{
		// 만약 동일한 클래스 유형의 아티팩트를 발견하면
		if (ExistingArtifact.StatusClass == InArtifact.StatusClass)
		{
			// 기존 아티팩트의 값을 주어진 아티팩트의 값만큼 증가시킵니다.
			ExistingArtifact.Value = ExistingArtifact.Value + InArtifact.Value;
			return true;
		}
	}

	// 동일한 클래스 유형의 아티팩트를 찾지 못한 경우, 새로운 아티팩트를 목록에 추가합니다.
	Artifacts.Add(InArtifact);
	return true;
}

FString UCosGameInstance::GetCurrentNodeMapFromInstance_Implementation()
{
	return CurrentNodeMap;
}

void UCosGameInstance::SetGlobalGraphicsQuality_Implementation(int32 InQuality)
{
	SetGraphicsQuality(InQuality);
}

int32 UCosGameInstance::GetGlobalGraphicsQuality_Implementation()
{
	return GlobalGraphicsQuality;
}

void UCosGameInstance::UpdateDeckInInstance_Implementation(const TArray<ACardBase*>& InCards)
{
	// 기존 덱을 비웁니다.
	Deck.Empty();

	for (ACardBase* InCard : InCards)
	{
		Deck.Add(InCard->CardDataDeck);
	}
}

void UCosGameInstance::AddVisitedNodeToInstance_Implementation(int32 InNodeID)
{
	VisitedNodes.Add(InNodeID);
}

TArray<FMinion> UCosGameInstance::GetPersistentHeroesFromInstance_Implementation()
{
	return PersistentHeroes;
}

TArray<int32> UCosGameInstance::GetVisitedMapNodesFromInstance_Implementation()
{
	return VisitedNodes;
}

bool UCosGameInstance::IsDebugMode_Implementation()
{
	return bDebugMode;
}

TArray<FDataTableRowHandle> UCosGameInstance::GetDoneStoryEncountersFromInstance_Implementation()
{
	return DoneStoryEncounters;
}

void UCosGameInstance::SetCurrentEncounterInInstance_Implementation(FEncounterData InCurrentEncounter)
{
	CurrentEncounter = InCurrentEncounter;
}

void UCosGameInstance::AddDoneStoryEncounterToInstance_Implementation(FDataTableRowHandle StoryEncounter)
{
	DoneStoryEncounters.Add(StoryEncounter);
}

bool UCosGameInstance::UpdateHeroPersistentHealth_Implementation(const FString& HeroUniqueID, int32 NewHealth)
{
	int32 ArrayIndex;
	FMinion Hero;
	if (Execute_GetHeroWithIdFromInstance(this, HeroUniqueID, Hero, ArrayIndex))
	{
		Hero.StartingStatuses[UStatus_Health::StaticClass()] = NewHealth;

		if (Execute_CheckIfAllHeroesAreDead(this))
		{
			UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_GameOver, this);
			UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_GameOver, this);
		}

		return true;
	}

	return false;
}

bool UCosGameInstance::CheckIfAllHeroesAreDead_Implementation()
{
	int32 DeadHeroCount = 0;

	for (const FMinion& PersistentHero : PersistentHeroes)
	{
		if (PersistentHero.StartingStatuses.Contains(UStatus_Health::StaticClass()))
		{
			if (PersistentHero.StartingStatuses[UStatus_Health::StaticClass()] <= 0)
			{
				DeadHeroCount++;
			}
		}
		else
		{
			DeadHeroCount++;
		}
	}

	return PersistentHeroes.Num() == DeadHeroCount;
}

bool UCosGameInstance::GetMainHero_Implementation(AActor*& OutMainHero)
{
	TArray<AActor*> MinionActors;
	UGameplayStatics::GetAllActorsOfClass(this, AMinionBase::StaticClass(), MinionActors);

	for (AActor* MinionActor : MinionActors)
	{
		AMinionBase* Minion = Cast<AMinionBase>(MinionActor);
		if (!Minion)
			continue;

		if (UKismetStringLibrary::EqualEqual_StrStr(Minion->MinionData.UniqueID, PersistentHeroes[0].UniqueID))
		{
			OutMainHero = Minion;
			return true;
		}
	}

	OutMainHero = nullptr;
	return false;
}

void UCosGameInstance::ChangeNodeMapInInstance_Implementation(const FString& NewNodeMap)
{
	CurrentNodeMap = NewNodeMap;
	VisitedNodes = {0};
}

FGameplayTagContainer UCosGameInstance::GetAllowedCardRewardsFromInstance_Implementation()
{
	// TODO
	return FGameplayTagContainer();
}

TArray<FStatusData> UCosGameInstance::GetArtifactsFromInstance_Implementation()
{
	// TODO
	return Artifacts;
}
