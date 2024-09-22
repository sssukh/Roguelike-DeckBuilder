#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_EventHolder.h"
#include "TurnManager.generated.h"

class UDelayHelper;

/* 
 * ATurnManager 클래스는 미니언과 카드 플레이어의 턴 순서를 관리하고 전투의 승리 및 패배 조건을 처리하는 매니저 클래스입니다.
 * 각 라운드에서 순서에 따라 미니언이 행동하며, 전투가 끝나면 승리 또는 패배 상태를 결정합니다.
 * 또한, 스폰된 액터를 이니셔티브 목록에 추가하고, 전투 중에 이벤트를 통해 액터들의 턴을 관리합니다.
 */
UCLASS()
class CROWNOFSIN_API ATurnManager : public AActor, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	ATurnManager();

protected:
	virtual void BeginPlay() override;

public:
	/*새로운 라운드를 시작하는 함수로, 현재 라운드를 증가시키고 액터들의 턴 순서를 설정한 후 첫 번째 액터의 턴을 시작합니다.*/
	UFUNCTION()
	void StartNewRound();

	/**
 	 * 다음 액터의 턴을 시작하는 함수로, 이니셔티브가 가장 높은 액터를 찾아 해당 액터의 턴을 시작합니다.
 	 * 만약 모든 액터의 턴이 끝나면 새로운 라운드를 시작합니다.
 	 */
	UFUNCTION()
	void StartNextObjectTurn();

	/**
	 * 현재 액터의 턴을 종료하고, 다음 틱에서 새로운 액터의 턴을 시작합니다.
	 * 현재 턴을 진행 중인 액터가 있는 경우, 그 액터를 RoundOrderActors 리스트에서 제거합니다.
	 */
	UFUNCTION()
	void EndTurn();

	/**
	 * 전투 승리 시 호출되는 함수로, 전투 종료를 처리하고 승리 이벤트를 발생시킵니다.
	 * 영웅의 지속적인 체력을 저장하고, 전투 후 보상을 처리합니다.
	 */
	UFUNCTION()
	void ExecuteCombatVictory();

	/*전투 패배 또는 게임 오버 시 호출되는 함수로, 전투 종료를 처리하고 게임 오버 이벤트를 발생시킵니다.*/
	UFUNCTION()
	void ExecuteGameOver();

	/*전투 종료 후, 현재 노드를 저장하고 새로운 맵을 로드하는 역할을 합니다.*/
	UFUNCTION()
	void ExitCombat();

public:
	/**
	
	 * 주어진 액터를 이니셔티브 배열(InitiativeArray)에 추가하며, 이니셔티브 값을 기반으로 정렬합니다.
	 * @param InActor 이니셔티브 배열에 추가할 액터입니다.
	 * @param InitiativeArray 이니셔티브가 정렬된 배열입니다.
	 * @return 추가된 액터의 위치 인덱스를 반환합니다.
	 */
	UFUNCTION()
	int32 AddActorToInitiative(AActor* InActor, TArray<AActor*>& InitiativeArray);

	/**
	 * 새로 생성된 액터를 이니셔티브 순서에 추가하고, 해당 액터가 파괴되었을 때 콜백을 등록합니다.
	 * 현재 활성 액터가 카드 플레이어일 경우, 해당 액터를 라운드 순서에도 추가합니다.
	 * @param SpawnedActor 새로 생성된 액터입니다.
	 */
	UFUNCTION()
	void AddSpawnedActorToInitiative(AActor* SpawnedActor);

	/**
	 * 주어진 고유 ID에 해당하는 미니언을 찾습니다.
	 * @param MinionUniqueId 찾고자 하는 미니언의 고유 ID
	 * @param OutMinion 찾은 미니언을 반환할 출력 매개변수
	 * @return 미니언을 찾았을 경우 true, 그렇지 않으면 false
	 */
	UFUNCTION()
	bool FindMinionAmongRemaining(const FString& MinionUniqueId, AMinionBase*& OutMinion);

	/**
	 * 남아있는 액터들 중에서 가장 높은 이니셔티브 값을 가진 액터를 찾습니다.
	 * @param FastestActor 가장 높은 이니셔티브를 가진 액터를 반환하는 출력 매개변수
	 * @return 유효한 액터를 찾으면 true, 그렇지 않으면 false
	 */
	UFUNCTION()
	bool GetRemainingActorWithHighestInitiative(AActor*& FastestActor);

	/**
	 * 미니언의 사망 후 전투가 종료될지 여부를 확인합니다.
	 * 영웅이 남아 있지만 적이 없으면 전투 승리를, 영웅이 없다면 게임 오버를 처리합니다.
	 */
	UFUNCTION()
	void CheckIfCombatShouldEndAfterDeath();

	/**
	 * 지정된 태그를 가진 살아있는 미니언이 있는지 확인합니다.
	 * 태그와 일치하는 살아있는 미니언이 있으면 true를 반환하고, 그렇지 않으면 false를 반환합니다.
	 */
	UFUNCTION()
	bool CheckForRemainingMinionsWithTag(FGameplayTag CheckTag);

protected:
	/**
 	 * 액터가 파괴될 때, 해당 액터를 라운드 순서 및 이니셔티브 목록에서 제거합니다.
 	 */
	UFUNCTION()
	void RemoveActorIfDestroyed(AActor* DestroyedActor);

private:
	// 델리게이트로 사용할 멤버 함수들
	bool IsInitiativeListReady();
	void LogLoopProgress(int32 LoopIndex);
	void OnInitiativeReady();

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	/**
	 * RunEvent_Implementation
	 * 호출된 이벤트에 따라 적절한 액션을 수행하는 함수.
	 * 타겟 스폰, 턴 종료, 사망, 게임 승리, 보상 화면 종료 등의 이벤트를 처리합니다.
	 */
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	/** 기본 씬 루트 컴포넌트 (씬 트랜스폼을 위한 루트) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turn Manager|Component")
	USceneComponent* DefaultSceneRoot;

public:
	/** 게임 세팅 완료 상태를 나타내는 플래그 */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	bool bSetUpDone = false;

	/** 전투가 종료되었는지 여부를 나타내는 플래그 */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	bool bCombatOver = false;

	/** 현재 라운드 번호 */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	int32 CurrentRound;

	/** 현재 턴을 진행 중인 액터 */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	AActor* ActiveActor;

	/** 이니셔티브 정렬된 액터 목록 */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	TArray<AActor*> InitiativeSortedActors;

	/** 현재 라운드에서 액터들이 턴을 진행할 순서 */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	TArray<AActor*> RoundOrderActors;

private:
	/** 게임 시작 딜레이 처리를 위한 헬퍼 클래스 */
	UPROPERTY()
	UDelayHelper* BeginDelayHelper;
};
