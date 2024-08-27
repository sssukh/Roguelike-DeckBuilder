#include "NodeSystem/NodeMap.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/CosLog.h"
#include "NodeSystem/Node.h"


ANodeMap::ANodeMap(): DefaultSceneRoot(nullptr), DebugArrow(nullptr), OriginNode(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AutoReceiveInput = EAutoReceiveInput::Player0;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	DebugArrow = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("DebugArrow"));
	DebugArrow->SetupAttachment(RootComponent);
}

void ANodeMap::BeginPlay()
{
	Super::BeginPlay();

	InitNodeMap();
}

void ANodeMap::InitNodeMap()
{
	// 현재 노드 맵의 상태를 최신 정보로 업데이트합니다.
	UpdateNodeMap();

	// 모든 ANode 타입의 액터를 검색하여 NodeList에 저장합니다.
	TArray<AActor*> AllNodes;
	UGameplayStatics::GetAllActorsOfClass(this, ANode::StaticClass(), AllNodes);

	// NodeList에 있는 각 노드를 순회하며 선택 이벤트를 바인딩합니다.
	for (AActor* NodeActor : AllNodes)
	{
		// AActor 타입의 NodeActor를 ANode 타입으로 캐스팅합니다.
		ANode* Node = Cast<ANode>(NodeActor);

		if (!Node) // 캐스팅에 실패한 경우, 다음 액터로 넘어갑니다.
		{
			continue;
		}

		// 각 노드의 OnNodeSelected 델리게이트에 UpdateOnNodeSelected 함수를 바인딩합니다.
		// 노드가 선택될 때 ANodeMap::UpdateOnNodeSelected가 호출되도록 설정합니다.
		Node->OnNodeSelected.AddDynamic(this, &ANodeMap::UpdateOnNodeSelected);
	}
}

ANode* ANodeMap::FindAndStoreOrigin()
{
	// 모든 ANode 클래스를 상속받은 액터를 검색하여 NodeList에 저장합니다.
	TArray<AActor*> NodeList;
	UGameplayStatics::GetAllActorsOfClass(this, ANode::StaticClass(), NodeList);

	// NodeList에 있는 각 노드를 순회하면서 Origin 노드를 찾습니다.
	for (AActor* NodeActor : NodeList)
	{
		// AActor 타입의 NodeActor를 ANode 타입으로 캐스팅합니다.
		ANode* Node = Cast<ANode>(NodeActor);
		if (!Node) // 캐스팅에 실패한 경우 다음 액터로 넘어갑니다.
		{
			continue;
		}

		// 현재 노드가 Origin 노드인지 확인합니다.
		if (Node->bOrigin)
		{
			// Origin 노드를 찾은 경우, OriginNode 멤버 변수에 저장하고 반복문을 종료합니다.
			OriginNode = Node;
			break;
		}
	}

	// OriginNode가 유효한지 확인합니다.
	// Origin 노드를 찾지 못한 경우 경고 메시지를 출력합니다.
	if (!IsValid(OriginNode))
	{
		COS_LOG_SCREEN(TEXT("경고: Origin 노드를 찾을 수 없습니다."));
	}

	// Origin 노드를 반환합니다. 유효하지 않으면 nullptr을 반환합니다.
	return OriginNode;
}

TMap<ANode*, ANode*> ANodeMap::FindAllPathsFromNode(ANode* InOriginNode)
{
	// 각 노드와 그 부모 노드를 저장할 맵을 초기화합니다.
	// 키: 현재 노드, 값: 부모 노드
	TMap<ANode*, ANode*> NodeParentMap;

	// 시작 노드를 부모가 없는 상태로 맵에 추가합니다.
	NodeParentMap.Add(InOriginNode, nullptr);

	// 탐색할 노드를 저장할 큐(현재 열려 있는 노드들)
	TArray<ANode*> OpenNodesQueue;
	OpenNodesQueue.Add(InOriginNode); // 시작 노드를 큐에 추가합니다.

	// 다음 단계에서 탐색할 자식 노드를 저장할 임시 리스트
	TArray<ANode*> ChildNodes;

	// 탐색이 완료될 때까지 계속합니다.
	while (true)
	{
		// 탐색할 노드가 더 이상 없는 경우, 경로 맵을 반환합니다.
		if (OpenNodesQueue.IsEmpty())
		{
			return NodeParentMap;
		}

		// 현재 열려 있는 각 노드에 대해 연결된 노드를 탐색합니다.
		for (ANode* CurrentNode : OpenNodesQueue)
		{
			// 현재 노드와 연결된 각 노드를 확인합니다.
			for (ANode* ConnectedNode : CurrentNode->Connections)
			{
				// 이미 경로에 추가된 노드라면 건너뜁니다.
				if (NodeParentMap.Contains(ConnectedNode))
				{
					continue;
				}

				// 아직 추가되지 않은 노드인 경우, 자식 리스트에 추가합니다.
				ChildNodes.Add(ConnectedNode);

				// 연결된 노드와 그 부모 노드를 맵에 추가합니다.
				NodeParentMap.Add(ConnectedNode, CurrentNode);
			}
		}

		// 다음 단계에서 탐색할 자식 노드를 열려 있는 노드 리스트로 설정합니다.
		OpenNodesQueue = ChildNodes;

		// 자식 노드 리스트를 비웁니다. 다음 반복에서 다시 사용됩니다.
		ChildNodes.Empty();
	}
}

void ANodeMap::SetUniqueNodeIdsFromOrigin()
{
	// OriginNode에서 시작하여 모든 노드에 대한 경로를 찾습니다.
	// 각 노드의 부모 노드를 저장하는 TMap을 생성합니다.
	TMap<ANode*, ANode*> NodePaths = FindAllPathsFromNode(OriginNode);

	int32 NodeIndex = 0; // 노드의 고유 ID를 저장할 변수입니다.

	// 경로에 포함된 각 노드를 순회하여 고유한 ID를 할당합니다.
	for (TTuple<ANode*, ANode*> NodePath : NodePaths)
	{
		// 경로의 키(Key)는 현재 노드를 의미합니다.
		ANode* CurrentNode = NodePath.Key;

		// NodeIds 맵에 고유 ID와 노드 포인터를 추가합니다.
		NodeIds.Add(NodeIndex, CurrentNode);

		// 노드 자체의 ID 멤버 변수에도 고유 ID를 할당합니다.
		CurrentNode->ID = NodeIndex;

		// 다음 노드에 대해 ID 값을 증가시킵니다.
		NodeIndex++;
	}
}

void ANodeMap::UpdateNodeMap()
{
	// 현재 게임 인스턴스를 가져옵니다.
	UGameInstance* CurrentGameInstance = UGameplayStatics::GetGameInstance(this);

	// GameInstance가 UInterface_CardGameInstance 인터페이스를 구현하는지 확인합니다.
	if (!CurrentGameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		// 구현하지 않은 경우 경고 메시지를 출력하고 함수를 종료합니다.
		COS_LOG_SCREEN(TEXT("GameInstance가 UInterface_CardGameInstance를 상속받지 않았습니다."));
		return;
	}

	// 방문한 노드 경로를 GameInstance에서 가져옵니다.
	// 이 경로는 이미 방문한 노드의 ID를 나타냅니다.
	StoredPath = IInterface_CardGameInstance::Execute_GetVisitedMapNodesFromInstance(CurrentGameInstance);

	// Origin 노드를 찾아 저장합니다.
	// 이 Origin 노드는 경로의 시작점 역할을 합니다.
	FindAndStoreOrigin();

	// Origin 노드에서 시작하여 모든 노드에 고유 ID를 할당합니다.
	// 고유 ID는 각 노드를 식별하고 경로 탐색을 용이하게 합니다.
	SetUniqueNodeIdsFromOrigin();

	// 방문한 노드 경로(StoredPath)에 있는 각 노드를 '교차' 처리합니다.
	// 이 처리는 노드를 사용 불가능하게 표시합니다.
	for (int32 Path : StoredPath)
	{
		if (NodeIds.Contains(Path))
		{
			NodeIds[Path]->CrossOut();
		}
	}

	// 방문한 노드 경로의 마지막 노드를 가져옵니다.
	int32 LastVisitedNodeID = StoredPath.Last();

	// 마지막으로 방문한 노드가 NodeIds 맵에 있는지 확인합니다.
	if (NodeIds.Contains(LastVisitedNodeID))
	{
		// 마지막으로 방문한 노드에 연결된 각 노드를 활성화합니다.
		// 활성화된 노드는 플레이어가 이동할 수 있는 노드입니다.
		for (ANode* ConnectedNode : NodeIds[LastVisitedNodeID]->Connections)
		{
			ConnectedNode->Enable(); // 노드를 활성화합니다.
		}
	}
}

void ANodeMap::UpdateOnNodeSelected(ANode* InNode)
{
	// 맵 내의 모든 ANode 타입의 액터를 검색하여 NodeList에 저장합니다.
	TArray<AActor*> AllNodes;
	UGameplayStatics::GetAllActorsOfClass(this, ANode::StaticClass(), AllNodes);

	// NodeList에 있는 각 노드를 순회하면서 비활성화합니다.
	for (AActor* NodeActor : AllNodes)
	{
		// AActor 타입의 NodeActor를 ANode 타입으로 캐스팅합니다.
		ANode* Node = Cast<ANode>(NodeActor);

		if (!Node) // 캐스팅에 실패한 경우, 다음 액터로 넘어갑니다.
		{
			continue;
		}

		// 노드를 비활성화하여, 플레이어가 더 이상 선택할 수 없도록 합니다.
		Node->StopEnable();
	}

	// 현재 노드 상태에 따라 노드 맵을 업데이트합니다.
	UpdateNodeMap();
}
