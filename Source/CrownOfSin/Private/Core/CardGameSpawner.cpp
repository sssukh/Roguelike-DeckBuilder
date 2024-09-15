#include "Core/CardGameSpawner.h"

#include "Core/MinionTrack.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetTableRef.h"
#include "Libraries/FunctionLibrary_Singletons.h"

ACardGameSpawner::ACardGameSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);


	if (TSubclassOf<AActor> BP_GlobalDispatcherHub = FAssetReferenceUtility::FindClassFromDataTable<AActor>(AssetRefPath::BluePrintPath, FName("BP_GlobalDispatcherHub"), true))
	{
		SpawnOrderActors.Add(BP_GlobalDispatcherHub);
	}
	if (TSubclassOf<AActor> BP_ActionManager = FAssetReferenceUtility::FindClassFromDataTable<AActor>(AssetRefPath::BluePrintPath, FName("BP_ActionManager"), true))
	{
		SpawnOrderActors.Add(BP_ActionManager);
	}
	if (TSubclassOf<AActor> BP_CardPlayer = FAssetReferenceUtility::FindClassFromDataTable<AActor>(AssetRefPath::BluePrintPath, FName("BP_CardPlayer"), true))
	{
		SpawnOrderActors.Add(BP_CardPlayer);
	}
	if (TSubclassOf<AActor> BP_RewardHolder = FAssetReferenceUtility::FindClassFromDataTable<AActor>(AssetRefPath::BluePrintPath, FName("BP_RewardHolder"), true))
	{
		SpawnOrderActors.Add(BP_RewardHolder);
	}
	if (TSubclassOf<AActor> BP_TurnManager = FAssetReferenceUtility::FindClassFromDataTable<AActor>(AssetRefPath::BluePrintPath, FName("BP_TurnManager"), true))
	{
		SpawnOrderActors.Add(BP_TurnManager);
	}
}

void ACardGameSpawner::BeginPlay()
{
	Super::BeginPlay();

	/**
 	 * 카드 전투에 필요한 액터를 생성하고, 저장된 영웅 및 적 정보를 MinionTrack에 추가하는 함수입니다.
 	 * 게임 인스턴스에서 영웅 데이터를 불러와 MinionTrack에 추가하고, Encounter 데이터를 기반으로 적을 생성합니다.
 	 */

	for (TSubclassOf<AActor> SpawnActor : SpawnOrderActors)
	{
		FTransform SpawnTransform = GetActorTransform();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::SelectDefaultAtRuntime;
		AActor* NewSpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActor, SpawnTransform, SpawnParams);
	}

	// 카드 게임 인스턴스를 가져옵니다. (없으면 함수 종료)
	UGameInstance* CardGameInstance = COS_FSingletons::GetCardGameInstance(this);
	if (!CardGameInstance) return;

	// 현재 게임 상태를 저장하려고 시도합니다.
	IInterface_CardGameInstance::Execute_AttemptSaveGame(CardGameInstance, FString(TEXT("")), false);

	// MinionTrack 클래스를 찾아 해당 트랙들을 가져옵니다.
	TArray<AActor*> MinionTrackActors;
	UGameplayStatics::GetAllActorsOfClass(this, AMinionTrack::StaticClass(), MinionTrackActors);
	if (MinionTrackActors.IsEmpty()) return;


	// 첫 번째 MinionTrack에 영웅 데이터를 추가합니다.
	AMinionTrack* HeroMinionTrack = Cast<AMinionTrack>(MinionTrackActors[0]);
	if (!HeroMinionTrack) return;

	// 게임 인스턴스에서 영웅 데이터를 가져와서 첫 번째 트랙에 추가합니다.
	const TArray<FMinion>& PersistentHeroes = IInterface_CardGameInstance::Execute_GetPersistentHeroesFromInstance(CardGameInstance);
	for (const FMinion& PersistentHero : PersistentHeroes)
	{
		HeroMinionTrack->AddMinionToTrack(HeroMinionTrack->Minions.Num(), PersistentHero, true);
	}

	// 두 번째 MinionTrack에 Encounter 데이터를 기반으로 적을 추가합니다.
	AMinionTrack* EncounterMinionTrack = Cast<AMinionTrack>(MinionTrackActors[1]);
	if (!EncounterMinionTrack) return;

	// Encounter 데이터를 가져와서 적을 추가합니다.
	const FEncounterData& CurrentEncounter = IInterface_CardGameInstance::Execute_GetCurrentEncounterFromInstance(CardGameInstance);
	for (const FDataTableRowHandle& EncounterMinion : CurrentEncounter.Minions)
	{
		// 데이터 테이블에서 해당 적 정보를 가져옵니다.
		FMinion* FoundMinion = EncounterMinion.DataTable->FindRow<FMinion>(EncounterMinion.RowName,TEXT(""));
		if (!FoundMinion) continue;

		EncounterMinionTrack->AddMinionToTrack(EncounterMinionTrack->Minions.Num(), *FoundMinion, true);
	}
}
