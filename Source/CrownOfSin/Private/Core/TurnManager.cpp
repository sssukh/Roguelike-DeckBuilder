#include "Core/TurnManager.h"

#include "ActionSystem/Action_LoadMap.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Interfaces/Interface_Initiative.h"
#include "Libraries/DelayHelper.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"

#include "CardSystem/CardPlayer.h"
#include "Core/MinionBase.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Card.h"
#include "StatusSystem/Status_Health.h"

ATurnManager::ATurnManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void ATurnManager::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 호출되는 초기화 함수로, 전투 시스템의 기본 설정과 이벤트 바인딩을 처리합니다.
	// 이니셔티브 배열이 준비되면 전투 라운드를 시작하는 딜레이 로직을 추가로 처리합니다.

	// 전투 관련 이벤트들을 글로벌 디스패처 허브에 바인딩
	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Death);
	EventTags.AddTag(CosGameTags::Event_TargetSpawned);
	EventTags.AddTag(CosGameTags::Event_TurnEnd);
	EventTags.AddTag(CosGameTags::Event_WinGame);
	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);

	// 게임 세팅 완료 상태 플래그 설정
	bSetUpDone = true;

	// 딜레이 처리를 위한 DelayHelper 생성
	BeginDelayHelper = NewObject<UDelayHelper>(this);

	// 이니셔티브 배열에 액터가 추가되었는지 확인하는 조건 함수
	auto IsInitiativeListReady = [this]() -> bool
	{
		return InitiativeSortedActors.Num() > 0;
	};

	// 딜레이 중간에 반복 호출될 함수 (현재 로직에선 사용하지 않음)
	auto OnLoopProgress = [](int32 LoopIndex)
	{
		// 추가 로직 필요 시 구현
	};

	// 이니셔티브 배열이 준비되었을 때 호출되는 완료 함수
	auto OnInitiativeReady = [this]()
	{
		// 다음 틱에 게임 시작 이벤트 호출 및 첫 번째 라운드 시작
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_GameStart, this);
			StartNewRound();
		});
	};

	// 이니셔티브가 준비될 때까지 0초 딜레이로 반복 확인
	BeginDelayHelper->DelayWhile(IsInitiativeListReady, OnLoopProgress, OnInitiativeReady, 0.0f);
}

void ATurnManager::StartNewRound()
{
	// 라운드 카운트를 증가시킵니다.
	CurrentRound++;

	// 이니셔티브에 따라 액터 순서를 설정합니다.
	RoundOrderActors = InitiativeSortedActors;

	// 첫 번째 액터의 턴을 시작합니다.
	StartNextObjectTurn();
}

void ATurnManager::StartNextObjectTurn()
{
	// 전투가 끝났다면 더 이상 턴을 진행하지 않음
	if (bCombatOver) return;

	// 가장 높은 이니셔티브를 가진 액터를 찾아서 ActiveActor에 할당
	if (GetRemainingActorWithHighestInitiative(ActiveActor))
	{
		// 해당 액터의 DispatcherHubLocalComponent를 찾음
		UDispatcherHubLocalComponent* DispatcherHubLocal = ActiveActor->FindComponentByClass<UDispatcherHubLocalComponent>();

		// 컴포넌트가 없으면 함수 종료
		if (!DispatcherHubLocal) return;

		// 해당 액터의 턴을 시작하는 이벤트 호출
		DispatcherHubLocal->CallEvent(CosGameTags::Event_TurnStart, ActiveActor);
	}
	else
	{
		// 모든 액터의 턴이 끝났다면 다음 틱에서 새로운 라운드를 시작
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			StartNewRound();
		});
	}
}

void ATurnManager::EndTurn()
{
	// 현재 활성화된 액터가 유효한지 확인
	if (IsValid(ActiveActor))
	{
		// 현재 액터가 RoundOrderActors 리스트에 포함되어 있으면 제거
		if (RoundOrderActors.Contains(ActiveActor))
		{
			RoundOrderActors.Remove(ActiveActor);
		}
	}

	// 다음 틱에서 다음 액터의 턴을 시작하도록 설정
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		StartNextObjectTurn();
	});
}

void ATurnManager::ExecuteCombatVictory()
{
	// 전투 종료를 표시
	bCombatOver = true;

	// 승리 이벤트 호출
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Victory, this);

	// 게임 인스턴스 가져오기
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance) return;

	// 지속적인 영웅 상태 가져오기
	const TArray<FMinion>& PersistentHeroes = IInterface_CardGameInstance::Execute_GetPersistentHeroesFromInstance(CardGameInstance);

	// 남아있는 영웅의 체력을 업데이트
	for (const FMinion& PersistentHero : PersistentHeroes)
	{
		AMinionBase* Minion;
		if (FindMinionAmongRemaining(PersistentHero.UniqueID, Minion))
		{
			// 영웅 태그와 체력 상태 확인
			UStatus_Health* MinionStatusHealth = Minion->FindComponentByClass<UStatus_Health>();
			if (Minion->GetGameplayTags().HasTag(CosGameTags::Target_Hero) && MinionStatusHealth)
			{
				// 영웅의 지속적인 체력 업데이트
				IInterface_CardGameInstance::Execute_UpdateHeroPersistentHealth(CardGameInstance, PersistentHero.UniqueID, MinionStatusHealth->StatusValue);
			}
		}
		else
		{
			// 영웅이 남아있지 않으면 기본 체력 1로 설정
			IInterface_CardGameInstance::Execute_UpdateHeroPersistentHealth(CardGameInstance, PersistentHero.UniqueID, 1);
		}
	}

	// 다음 틱에서 보상 화면 또는 전투 종료 처리
	GetWorld()->GetTimerManager().SetTimerForNextTick([this,CardGameInstance]()
	{
		// 1.5초 후 처리 진행
		UFunctionLibrary_Singletons::QueueDelay(this, 1.5f);

		// 메인 영웅 확인
		AActor* MainHero;
		if (IInterface_CardGameInstance::Execute_GetMainHero(CardGameInstance, MainHero))
		{
			// 승리 이벤트 호출 및 보상 화면 처리
			UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_Victory, this);
			UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_CloseRewardScreen);

			// 현재 전투의 보상 카드 스폰 및 실행
			const FEncounterData& CurrentEncounter = IInterface_CardGameInstance::Execute_GetCurrentEncounterFromInstance(CardGameInstance);
			UFunctionLibrary_Card::SpawnAndPlayHeroCardUnTargeted(this, CurrentEncounter.Reward, nullptr);
		}
		else
		{
			// 메인 영웅이 없으면 전투 종료
			ExitCombat();
		}
	});
}

void ATurnManager::ExecuteGameOver()
{
	// 전투 종료를 표시
	bCombatOver = true;

	// 게임 오버 이벤트 호출
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_GameOver, this);

	// 다음 틱에 게임 오버 액션 이벤트 전파
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_GameOver, this);
	});
}

void ATurnManager::ExitCombat()
{
	// 게임 인스턴스 가져오기. 없을 경우 함수 종료.
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance) return;

	// 현재 노드 맵 가져오기 및 저장
	FString CurrentNodeMap = IInterface_CardGameInstance::Execute_GetCurrentNodeMapFromInstance(CardGameInstance);
	IInterface_CardGameInstance::Execute_AttemptSaveGame(CardGameInstance, CurrentNodeMap, true);

	// 약간의 딜레이를 추가하여 다음 작업을 지연시킴
	UFunctionLibrary_Singletons::QueueDelay(this, 0.5f);

	// 기본 변환 값 설정
	FTransform SpawnTransform = FTransform::Identity;

	// 맵 로드 액션을 지연 생성
	if (AAction_LoadMap* NewActionLoadMap = GetWorld()->SpawnActorDeferred<AAction_LoadMap>(AAction_LoadMap::StaticClass(),
	                                                                                        SpawnTransform,
	                                                                                        nullptr, nullptr,
	                                                                                        ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		// 로드할 맵과 딜레이 값 설정
		NewActionLoadMap->Level = CurrentNodeMap;
		NewActionLoadMap->EndDelay = -1.0f;
		NewActionLoadMap->FinishSpawning(SpawnTransform);
	}
}

int32 ATurnManager::AddActorToInitiative(AActor* InActor, TArray<AActor*>& InitiativeArray)
{
	// 액터가 이니셔티브 인터페이스를 구현하지 않으면 함수 종료
	if (!InActor->Implements<UInterface_Initiative>()) return 0;

	// 액터의 이니셔티브 값 가져오기
	float ActorInitiative = IInterface_Initiative::Execute_GetInitiative(InActor);

	// 이니셔티브 배열을 순회하며 액터를 적절한 위치에 삽입
	int32 InsertIndex = 0;
	for (AActor* CurrentEntry : InitiativeArray)
	{
		// 현재 항목이 이니셔티브 인터페이스를 구현하지 않으면 건너뜀
		if (!CurrentEntry->Implements<UInterface_Initiative>()) continue;

		// 현재 항목의 이니셔티브 값 가져오기
		float CurrentInitiative = IInterface_Initiative::Execute_GetInitiative(CurrentEntry);

		// 새로운 액터의 이니셔티브 값이 더 크거나 같다면 해당 위치에 삽입
		if (ActorInitiative >= CurrentInitiative)
		{
			InitiativeArray.Insert(InActor, InsertIndex);
			return InsertIndex;
		}

		// 인덱스 증가
		++InsertIndex;
	}

	// 배열에 적절한 위치를 찾지 못하면 끝에 추가
	InitiativeArray.Add(InActor);
	return InitiativeArray.Num() - 1;
}

void ATurnManager::AddSpawnedActorToInitiative(AActor* SpawnedActor)
{
	// 새로 생성된 액터를 이니셔티브 순서 배열에 추가
	AddActorToInitiative(SpawnedActor, InitiativeSortedActors);

	// 해당 액터가 파괴되었을 때 RemoveActorIfDestroyed 호출을 위한 콜백 등록
	SpawnedActor->OnDestroyed.AddUniqueDynamic(this, &ATurnManager::RemoveActorIfDestroyed);

	// 현재 게임의 카드 플레이어를 가져옴
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer) return;

	// 현재 활성 액터가 카드 플레이어일 경우, 새로 생성된 액터를 라운드 순서에도 추가
	if (ActiveActor == CardPlayer)
	{
		AddActorToInitiative(SpawnedActor, RoundOrderActors);
	}
}

bool ATurnManager::FindMinionAmongRemaining(const FString& MinionUniqueId, AMinionBase*& OutMinion)
{
	// 맵 내 모든 AMinionBase 클래스의 액터를 배열에 저장
	TArray<AActor*> MinionActors;
	UGameplayStatics::GetAllActorsOfClass(this, AMinionBase::StaticClass(), MinionActors);

	// 배열을 순회하며 미니언을 검색
	for (AActor* MinionActor : MinionActors)
	{
		// AMinionBase로 캐스팅 가능한지 확인
		AMinionBase* Minion = Cast<AMinionBase>(MinionActor);
		if (!Minion) continue; // 캐스팅 실패 시 다음 액터로 넘어감

		// 미니언의 고유 ID가 찾고자 하는 ID와 일치하는지 확인
		if (Minion->MinionData.UniqueID == MinionUniqueId)
		{
			// 일치하는 미니언을 찾았을 경우, OutMinion에 할당하고 true 반환
			OutMinion = Minion;
			return true;
		}
	}

	// 해당 미니언을 찾지 못한 경우, OutMinion을 nullptr로 설정하고 false 반환
	OutMinion = nullptr;
	return false;
}

bool ATurnManager::GetRemainingActorWithHighestInitiative(AActor*& FastestActor)
{
	float MaxInitiative = -FLT_MAX; // 가장 높은 이니셔티브 값을 추적

	// RoundOrderActors 배열을 순회하여 이니셔티브가 가장 높은 액터를 찾음
	for (AActor* Actor : RoundOrderActors)
	{
		// 액터가 UInterface_Initiative 인터페이스를 구현하는지 확인
		if (!Actor->Implements<UInterface_Initiative>()) continue;

		// 이니셔티브 값을 가져옴
		float CurrentInitiative = IInterface_Initiative::Execute_GetInitiative(Actor);

		// 현재 액터의 이니셔티브가 가장 높은 이니셔티브보다 클 경우 갱신
		if (CurrentInitiative > MaxInitiative)
		{
			MaxInitiative = CurrentInitiative; // 가장 높은 이니셔티브 값 저장
			FastestActor = Actor; // 해당 액터를 FastestActor에 저장
		}
	}

	// 가장 높은 이니셔티브를 가진 액터가 유효한지 여부를 반환
	return IsValid(FastestActor);
}

void ATurnManager::CheckIfCombatShouldEndAfterDeath()
{
	// 영웅이 남아 있는지 확인
	bool bHeroesRemain = CheckForRemainingMinionsWithTag(CosGameTags::Target_Hero);

	// 적이 남아 있는지 확인
	bool bEnemiesRemain = CheckForRemainingMinionsWithTag(CosGameTags::Target_Enemy);

	// 영웅이 남아 있지만 적이 없는 경우 전투 승리 처리
	if (bHeroesRemain && !bEnemiesRemain)
	{
		ExecuteCombatVictory();
	}
	// 영웅이 없는 경우 게임 오버 처리
	else if (!bHeroesRemain)
	{
		ExecuteGameOver();
	}
}

bool ATurnManager::CheckForRemainingMinionsWithTag(FGameplayTag CheckTag)
{
	// 미니언 목록을 가져옴
	TArray<AActor*> MinionList;
	UGameplayStatics::GetAllActorsOfClass(this, AMinionBase::StaticClass(), MinionList);

	// 각 미니언을 순회하여 조건을 확인
	for (AActor* MinionActor : MinionList)
	{
		// 미니언으로 캐스팅하고 유효성 체크
		AMinionBase* Minion = Cast<AMinionBase>(MinionActor);
		if (!Minion) continue; // 캐스팅 실패 시 다음 미니언으로

		// 미니언이 지정된 태그를 가지고 있고 살아있으면 true 반환
		if (Minion->GetGameplayTags().HasTagExact(CheckTag) && !Minion->bDead)
		{
			return true;
		}
	}

	// 조건을 만족하는 미니언이 없으면 false 반환
	return false;
}

void ATurnManager::RemoveActorIfDestroyed(AActor* DestroyedActor)
{
	// 라운드 순서에서 파괴된 액터를 제거
	RoundOrderActors.Remove(DestroyedActor);

	// 이니셔티브 정렬 목록에서도 파괴된 액터를 제거
	InitiativeSortedActors.Remove(DestroyedActor);
}

void ATurnManager::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 타겟이 스폰된 경우, 이니셔티브 목록에 추가
	if (EventTag == CosGameTags::Event_TargetSpawned)
	{
		AActor* SpawnedActor = Cast<AActor>(CallingObject);
		if (!SpawnedActor) return;

		// 새로 스폰된 액터를 이니셔티브에 추가
		AddSpawnedActorToInitiative(SpawnedActor);
	}
	// 턴 종료 이벤트 발생 시
	else if (EventTag == CosGameTags::Event_TurnEnd)
	{
		// 현재 턴 종료 처리
		EndTurn();
	}
	// 사망 이벤트 발생 시 전투 종료 여부 확인
	else if (EventTag == CosGameTags::Event_Death)
	{
		// 전투가 끝났는지 확인
		CheckIfCombatShouldEndAfterDeath();
		if (bCombatOver) return;

		// 사망한 액터가 현재 액티브 액터일 경우, 턴 종료 처리
		if (CallingObject == ActiveActor)
		{
			// 다음 틱에서 턴 종료 실행
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				EndTurn();
			});
		}
	}
	// 게임 승리 이벤트 발생 시
	else if (EventTag == CosGameTags::Event_WinGame)
	{
		// 전역 디스패처 허브에 승리 이벤트 전파
		UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_WinGame, this);
	}
	// 보상 화면이 닫힐 때 전투 종료 처리
	else if (EventTag == CosGameTags::Event_CloseRewardScreen)
	{
		// 전투 종료 처리
		ExitCombat();
	}
}
