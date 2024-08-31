#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeMap.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class ANodeBase;

/*노드를 추적하고 노드 간 경로 찾기를 처리합니다.*/
UCLASS(Category="CrownOfSin")
class CROWNOFSIN_API ANodeMap : public AActor
{
	GENERATED_BODY()

public:
	ANodeMap();

protected:
	virtual void BeginPlay() override;

	/**
     * @brief 노드 맵을 초기화하고, 모든 노드의 선택 이벤트를 바인딩합니다.
     *        현재 맵의 상태를 업데이트하고, 각 노드가 선택되었을 때 호출될 이벤트 핸들러를 설정합니다.
     */
	void InitNodeMap();

public:
	/**
     * @brief 현재 맵에 있는 노드 중에서 Origin 노드를 찾아 저장합니다.
     *        Origin 노드는 맵 탐색의 시작점이 되며, 설정된 경우 해당 노드를 반환합니다.
     *        Origin 노드가 없을 경우 경고 메시지를 출력합니다.
     * @return Origin 노드를 반환. 없을 경우 nullptr 반환.
     */
	ANodeBase* FindAndStoreOrigin();

	/**
     * @brief 주어진 시작 노드에서 모든 연결된 노드에 대한 경로를 찾습니다.
     *        너비 우선 탐색(BFS)을 사용하여 각 노드와 그 부모 노드를 맵에 저장합니다.
     * @param InOriginNode 탐색을 시작할 시작 노드입니다.
     * @return 시작 노드로부터의 모든 경로를 포함하는 TMap을 반환합니다. 키는 현재 노드이고, 값은 부모 노드입니다.
     */
	TMap<ANodeBase*, ANodeBase*> FindAllPathsFromNode(ANodeBase* InOriginNode);

	/**
     * @brief Origin 노드에서 시작하여 모든 연결된 노드에 고유한 ID를 설정합니다.
     *        노드 간 경로를 탐색하고, 각 노드에 연속적인 정수 ID를 할당하여 맵의 노드 ID를 관리합니다.
     */
	void SetUniqueNodeIdsFromOrigin();

	/**
     * @brief 노드 맵을 현재 상태로 업데이트합니다.
     *        게임 인스턴스에서 방문한 노드 경로를 가져와서 노드 맵을 초기화하고, 각 노드에 고유 ID를 설정한 후, 방문된 노드를 처리하고 마지막으로 방문된 노드의 연결된 노드를 활성화합니다.
     */
	void UpdateNodeMap();

protected:
	/**
     * @brief 사용자가 노드를 선택했을 때 호출되며, 모든 노드를 비활성화하고 노드 맵을 업데이트합니다.
     *        선택된 노드와 관련된 노드 맵의 상태를 갱신하여 플레이어의 진행을 반영합니다.
     * @param InNode 사용자가 선택한 노드입니다.
     */
	UFUNCTION()
	void UpdateOnNodeSelected(ANodeBase* InNode);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Node Map|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Node Map|Component")
	UHierarchicalInstancedStaticMeshComponent* DebugArrow;

public:
	// 현재 맵에서 방문된 노드의 ID를 저장하는 배열입니다.
	// 게임 진행 중 플레이어가 방문한 노드 경로를 추적하는 데 사용됩니다.
	UPROPERTY(BlueprintReadOnly, Category="Node Map")
	TArray<int32> StoredPath;

	// 각 노드의 고유 ID와 해당 노드에 대한 포인터를 매핑하는 맵입니다.
	// 노드를 고유하게 식별하고 노드 간의 관계를 관리하는 데 사용됩니다.
	UPROPERTY(BlueprintReadOnly, Category="Node Map")
	TMap<int32, ANodeBase*> NodeIds;

	// 노드 맵의 시작점 역할을 하는 Origin 노드를 가리키는 포인터입니다.
	// 경로 탐색 및 ID 할당의 기준이 되는 노드입니다.
	UPROPERTY(BlueprintReadOnly, Category="Node Map")
	ANodeBase* OriginNode;
};
