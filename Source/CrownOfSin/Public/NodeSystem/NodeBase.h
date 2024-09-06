#pragma once

#include "CoreMinimal.h"
#include "CrownOfSin/Public/Interfaces/Interface_Node.h"
#include "GameFramework/Actor.h"
#include "NodeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeSelected, ANodeBase*, Node);


class FOnTimelineEvent;
class UTimelineComponent;
class USplineComponent;
class UMapEventComponent;

/* 노드 맵의 노드.
 * 다른 노드와의 연결 및 만남 유형을 유지합니다. 이 두 가지 모두 노드에 의해 시각화됩니다.
 */
UCLASS()
class CROWNOFSIN_API ANodeBase : public AActor, public IInterface_Node
{
	GENERATED_BODY()

public:
	ANodeBase();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	// AddConnectionSplines 함수는 현재 노드와 연결된 모든 노드를 나타내는 스플라인 컴포넌트를 생성하고 기존에 생성된 스플라인 컴포넌트를 제거하여 중복을 방지합니다.
	// 각 스플라인 컴포넌트는 고유한 이름을 가지며, 연결된 노드의 위치에 따라 시작 및 끝 지점이 설정됩니다.
	UFUNCTION(BlueprintCallable, Category = "Node Event")
	void AddConnectionSplines();

	/**
	 * 이 함수는 주어진 스플라인 컴포넌트를 따라 스플라인 메시 컴포넌트를 생성하여 경로를 시각적으로 나타냅니다.
	 * 기존의 스플라인 메시 컴포넌트는 제거하여 메모리 누수를 방지하고, 새로운 스플라인 메시 컴포넌트를 생성하여 스플라인 경로를 지정된 간격으로 분할하고 각 세그먼트를 시각화합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Node Event")
	void DisplaySplinePath(USplineComponent* Spline, int32 ConnectionIndex);

	UFUNCTION(BlueprintCallable, Category="Node Event")
	void CrossOut();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Node Event")
	void Enable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Node Event")
	void StopEnable();

	/*========================================================================================
	*	Interface_Node
	=========================================================================================*/
public:
	/**
     * 노드 위에 마우스를 올렸을 때 호출되는 함수로, 해당 노드가 활성화 상태일 경우
     * 노드의 색상을 흰색으로 변경하여 강조 표시합니다. 플레이어의 UI 상태에 따라 강조 표시를 비활성화할 수도 있습니다.
     */
	virtual void HoverOverNode_Implementation() override;

	/**
     * 노드에서 마우스를 떼었을 때 호출되는 함수로, 이전 색상으로 복원하여 노드의
     * 강조 표시를 해제합니다.
     */
	virtual void HoverOffNode_Implementation() override;

	virtual void ClickNode_Implementation() override;


	

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	/*이 노드를 연결해야 하는 다른 노드를 선택하세요.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base")
	TArray<ANodeBase*> Connections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base|Config|SetUp")
	UMaterialInterface* NodeMeshMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base|Config|SetUp")
	TObjectPtr<UStaticMesh> SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base|Config|SetUp")
	TSubclassOf<UMapEventComponent> MapEventClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Node Base|Components")
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base|Components")
	TObjectPtr<UStaticMeshComponent> NodeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Node Base|Components")
	TObjectPtr<UStaticMeshComponent> CrossMesh;

	UPROPERTY(EditAnywhere, Category="Node Base|Components")
	TObjectPtr<UMapEventComponent> MapEventRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base", meta=(ExposeOnSpawn="true"))
	bool bOrigin = false;

	UPROPERTY(BlueprintReadWrite, Category="Node Base")
	bool bEnabled = false;

	UPROPERTY(BlueprintReadWrite, Category="Node Base")
	int32 Id = 0;

	UPROPERTY(BlueprintReadWrite, Category="Node Base")
	FLinearColor TrueColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base", meta = (ExposeOnSpawn="true",RowType="/Script/CrownOfSin.MapEvent"))
	FDataTableRowHandle MapEvent;

	UPROPERTY(BlueprintReadWrite, Category="Node Base")
	TObjectPtr<UMaterialInstanceDynamic> NodeMaterial;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Node Base|Delegate")
	FOnNodeSelected OnNodeSelected;

};
