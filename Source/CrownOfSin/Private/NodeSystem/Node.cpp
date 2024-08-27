// Fill out your copyright notice in the Description page of Project Settings.


#include "CrownOfSin/Public/NodeSystem/Node.h"

#include "VectorTypes.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "CrownOfSin//Public/NodeSystem/NodeSystemEnumStruct.h"
#include "CrownOfSin/Public/CardSystem/CardPlayer.h"
#include "CrownOfSin/Public/Libraries/FlowControllerLibiary.h"
#include "CrownOfSin/Public/NodeSystem/MapEventComponent.h"
#include "CrownOfSin/Public/UI/UWBP_Layout_RLDB.h"
#include "DSP/DynamicsProcessor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANode::ANode()
{
	TL_Wobble = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeLineComponent"));

	NodeGate = FGate(true);
}

// Called when the game starts or when spawned
void ANode::BeginPlay()
{
	Super::BeginPlay();

	TrueColor = NodeMaterial->K2_GetVectorParameterValue(TEXT("Color"));

	MapEventRef = NewObject<UMapEventComponent>(this, MapEventClass);
	MapEventRef->RegisterComponent();

	TL_Wobble->SetTimelineFinishedFunc(NodeTimeLineFinish);

	NodeTimeLineFinish.BindUFunction(this,FName("TimelineFinish"));

	FOnTimelineFloat WobbleUpdate;

	WobbleUpdate.BindUFunction(this,FName("TimelineUpdate"));
	TL_Wobble->AddInterpFloat(Wobble,WobbleUpdate);
}



void ANode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AddConnectionSplines();

	NodeMaterial = NodeMesh->CreateDynamicMaterialInstance(0, NodeMesh->GetMaterial(0));

	FMapEvent* MapEventData = MapEvent.GetRow<FMapEvent>(MapEvent.RowName.ToString());

	if (!MapEventData)
		return;

	NodeMaterial->SetTextureParameterValue(TEXT("Texture"), MapEventData->Icon);
}

void ANode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ANode::AddConnectionSplines()
{
	for (ANode* CurrentConnection : Connections)
	{
		if (!IsValid(CurrentConnection))
			return;

		USplineComponent* SplineComponent = NewObject<USplineComponent>(this, USplineComponent::StaticClass(),
		                                                                TEXT("SplineComponent"));
		SplineComponent->SetupAttachment(RootComponent);
		SplineComponent->RegisterComponent();

		FVector ActorLocation = GetActorLocation();

		FVector ConnectionLocation = CurrentConnection->GetActorLocation();

		FVector NormalizedValue = 50 * (ConnectionLocation - ActorLocation).GetSafeNormal(0.0001);
		SplineComponent->SetLocationAtSplinePoint(0, NormalizedValue + ActorLocation, ESplineCoordinateSpace::World);

		SplineComponent->SetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1,
		                                          CurrentConnection->GetActorLocation() - NormalizedValue,
		                                          ESplineCoordinateSpace::World);

		DisplaySplinePath(SplineComponent);
	}
	return;
}

void ANode::DisplaySplinePath(USplineComponent* Spline)
{
	int32 Divisor = 64;
	int32 LastIndex = Spline->GetSplineLength() / Divisor;

	for (int idx = 0; idx < LastIndex; ++idx)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this,TEXT("SplineMesh"));

		if (!IsValid(NewSplineMesh))
		{
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,
			                                 FString::Printf(TEXT("NewSplineMesh Should be set(SM_Edge)")));
			return;
		}

		SplineMeshComponent->SetStaticMesh(NewSplineMesh);

		FVector StartPos = Spline->GetLocationAtDistanceAlongSpline(idx * Divisor, ESplineCoordinateSpace::World);

		FVector StartTangent = Spline->GetDirectionAtDistanceAlongSpline(idx * Divisor, ESplineCoordinateSpace::World);

		FVector EndPos = Spline->GetLocationAtDistanceAlongSpline(idx * Divisor + Divisor,
		                                                          ESplineCoordinateSpace::World);

		FVector EndTangent = Spline->GetDirectionAtDistanceAlongSpline(idx * Divisor + Divisor,
		                                                               ESplineCoordinateSpace::World);

		SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	}
}

void ANode::CrossOut()
{
	CrossMesh->SetVisibility(true);

	NodeMaterial->SetVectorParameterValue(TEXT("Color"), TrueColor);
}

void ANode::Enable()
{
	bEnabled = true;

	NodeGate.Open();

	TL_Wobble->PlayFromStart();
}

void ANode::StopEnable()
{
	bEnabled = false;

	NodeGate.Close();
}

void ANode::HoverOverNode()
{
	if (!bEnabled)
		return;

	ACardPlayer* Player = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(nullptr, ACardPlayer::StaticClass()));


	if (!Player->PlayerUI->ShouldNodeMapBeBlocked())
	{
		
		TrueColor = NodeMaterial->K2_GetVectorParameterValue(TEXT("Color"));

		NodeMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::White);
	}
}

void ANode::HoverOffNode()
{
	if (bEnabled)
	{
		NodeMaterial->SetVectorParameterValue(TEXT("Color"), TrueColor);
	}
}

void ANode::ClickNode()
{
	if (!bEnabled)
		return;

	ACardPlayer* Player = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(nullptr, ACardPlayer::StaticClass()));

	if (!Player->PlayerUI->ShouldNodeMapBeBlocked())
	{
		// if(GetGameInstance()->GetClass()->ImplementsInterface(IInterface_CardGameInstance))
		// {
		// 	GetGameInstance->
		// }

		OnNodeSelected.Broadcast(this);

		MapEventRef->RunMapEvent(MapEvent);
	}
}

void ANode::TimelineFinish()
{
	if (NodeGate.IsOpen())
		TL_Wobble->PlayFromStart();
}

void ANode::TimeLineUpdate(float InWobble)
{
	FVector InVector = FVector(InWobble*0.2f+ 1.0f);
	NodeMesh->SetRelativeScale3D(InVector);
}
