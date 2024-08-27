// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "CrownOfSin/Public/Interfaces/Interface_Node.h"
#include "CrownOfSin/Public/Libraries/FlowControllerLibiary.h"
#include "GameFramework/Actor.h"
#include "Node.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeSelected, ANode*, Node);


class FOnTimelineEvent;
class UTimelineComponent;
class USplineComponent;
class UMapEventComponent;

UCLASS()
class CROWNOFSIN_API ANode : public AActor, public IInterface_Node
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:
	virtual void Tick(float DeltaSeconds) override;



public:
	UFUNCTION(Blueprintable, Category = "Node")
	void AddConnectionSplines();

	UFUNCTION(Blueprintable, Category = "Node")
	void DisplaySplinePath(USplineComponent* Spline);

	UFUNCTION()
	void CrossOut();

	UFUNCTION()
	void Enable();

	UFUNCTION()
	void StopEnable();

public:
	// Interface
	UFUNCTION()
	virtual void HoverOverNode() override;

	UFUNCTION()
	virtual void HoverOffNode() override;

	UFUNCTION()
	virtual void ClickNode() override;
public:
	// variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UStaticMeshComponent> NodeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UStaticMeshComponent> CrossMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UMapEventComponent> MapEventRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UTimelineComponent> TL_Wobble;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ANode*> Connections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstanceDynamic> NodeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor TrueColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle MapEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMapEventComponent> MapEventClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> NewSplineMesh;

	bool bIsOpened = false;

	FGate NodeGate;
	// Delegate
public:
	FOnNodeSelected OnNodeSelected;

	FOnTimelineEvent NodeTimeLineFinish;

	// Timeline
public:
	UFUNCTION(Category = "Timeline")
	void TimelineFinish();

	UFUNCTION(Category = "Timeline")
	void TimeLineUpdate(float InWobble);

	UPROPERTY(EditAnywhere,Category = "Timeline")
	UCurveFloat* Wobble;
};
