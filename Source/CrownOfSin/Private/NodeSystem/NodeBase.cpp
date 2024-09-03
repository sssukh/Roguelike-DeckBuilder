#include "CrownOfSin/Public/NodeSystem/NodeBase.h"

#include "CardSystem/CardPlayer.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CosLog.h"
#include "NodeSystem/MapEventComponent.h"
#include "NodeSystem/NodeEnumStruct.h"
#include "UI/UW_Layout_Cos.h"



ANodeBase::ANodeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
	NodeMesh->SetupAttachment(RootComponent);

	CrossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrossMesh"));
	CrossMesh->SetupAttachment(NodeMesh);
}

void ANodeBase::BeginPlay()
{
	Super::BeginPlay();

	if (NodeMaterial)
		TrueColor = NodeMaterial->K2_GetVectorParameterValue(TEXT("Color"));

	if (!MapEventClass)
	{
		COS_LOG_SCREEN(TEXT("MapEventClass를 설정해주세요!"));
		return;
	}
	MapEventRef = NewObject<UMapEventComponent>(this, MapEventClass);
	MapEventRef->RegisterComponent();
}

void ANodeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AddConnectionSplines();

	// 기존 NodeMaterial이 존재하면 삭제를 위한 절차
	if (NodeMaterial)
	{
		// 필요시 참조를 해제하고 가비지 컬렉션을 기다림
		NodeMaterial->MarkAsGarbage();
		NodeMaterial = nullptr; // 포인터 초기화
	}

	NodeMaterial = UMaterialInstanceDynamic::Create(NodeMesh->GetMaterial(0), this);

	if (MapEvent.IsNull())
	{
		COS_LOG_SCREEN(TEXT("%s 의 MapEvent가 설정되지 않았습니다. MapEvent를 설정해주세요."), *GetNameSafe(this));
		return;
	}
	if (FMapEvent* MapEventData = MapEvent.DataTable->FindRow<FMapEvent>(MapEvent.RowName,TEXT("")))
	{
		NodeMaterial->SetTextureParameterValue(TEXT("Texture"), MapEventData->Icon);
	}
}

void ANodeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ANodeBase::AddConnectionSplines()
{
	// 기존 스플라인 컴포넌트 제거
	TArray<UActorComponent*> ExistingSplineComponents;
	GetComponents(USplineComponent::StaticClass(), ExistingSplineComponents);
	for (UActorComponent* Component : ExistingSplineComponents)
	{
		Component->DestroyComponent();
	}

	int32 ConnectionIndex = 0; // 각 연결에 대한 고유한 스플라인 컴포넌트 이름을 생성하기 위한 인덱스

	for (ANodeBase* ConnectedNode : Connections)
	{
		if (!IsValid(ConnectedNode))
			continue;

		// 스플라인 컴포넌트의 고유한 이름을 생성
		FString SplineName = FString::Printf(TEXT("SplineComponent_%d"), ConnectionIndex++);

		USplineComponent* SplineComponent = NewObject<USplineComponent>(this, USplineComponent::StaticClass(), *SplineName);
		SplineComponent->RegisterComponent();
		AddInstanceComponent(SplineComponent);

		SplineComponent->SetRelativeTransform(FTransform::Identity);

		// 이 플래그를 설정하여 스플라인 컴포넌트가 기본값을 벗어나 편집되었음을 나타냅니다.
		SplineComponent->bSplineHasBeenEdited = true;
		// 이 플래그를 설정하여 스플라인 포인트들을 유저 컨스트럭션 스크립트에 전달하여 추가로 조작할 수 있도록 합니다.
		SplineComponent->bInputSplinePointsToConstructionScript = true;
		SplineComponent->EditorSelectedSplineSegmentColor = FLinearColor(1.0f, 0.1, 0.0f, 1.0f);

		// 시작과 끝 위치 사이의 방향 벡터를 정규화하고 스플라인 시작과 끝 점을 설정
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = ConnectedNode->GetActorLocation();
		FVector DirectionOffset = UKismetMathLibrary::Normal(EndLocation - StartLocation) * 50;

		SplineComponent->SetLocationAtSplinePoint(0, StartLocation + DirectionOffset, ESplineCoordinateSpace::World);
		SplineComponent->SetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, EndLocation - DirectionOffset, ESplineCoordinateSpace::World);

		DisplaySplinePath(SplineComponent,ConnectionIndex);
	}
}

void ANodeBase::DisplaySplinePath(USplineComponent* Spline, int32 ConnectionIndex)
{
	// 스플라인 메시를 추가할 간격을 설정
	double SegmentLength = 64.0f;
	double Remainder = 0.0f; // 나머지를 저장할 변수
	double SplineLength = Spline->GetSplineLength();

	// 스플라인의 길이를 간격으로 나눈 후 나머지를 무시한 마지막 인덱스 계산
	int32 SegmentCount = UKismetMathLibrary::FMod(SplineLength, SegmentLength, Remainder);

	// 각 스플라인 메시 컴포넌트를 추가하고 설정하는 루프
	for (int32 SegmentIndex = 0; SegmentIndex < SegmentCount; ++SegmentIndex)
	{
		// 고유한 이름 생성
		FString SplineMeshName = FString::Printf(TEXT("SplineMesh_%d_%d"), ConnectionIndex, SegmentIndex);

		// 스플라인 메시 컴포넌트를 생성
		if (USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, *SplineMeshName))
		{
			// 스플라인 메시 컴포넌트를 루트 컴포넌트에 첨부하고 등록
			SplineMeshComponent->SetupAttachment(RootComponent);
			SplineMeshComponent->RegisterComponent();
			AddInstanceComponent(SplineMeshComponent);

			// 기본 변환 설정 및 그림자 드리우기 비활성화
			SplineMeshComponent->SetRelativeTransform(FTransform::Identity);
			SplineMeshComponent->SetCastShadow(false);

			// 메시 설정
			SplineMeshComponent->SetStaticMesh(SplineMesh);

			// 현재 스플라인 세그먼트의 시작 및 끝 위치와 탄젠트 계산
			FVector StartPosition = Spline->GetLocationAtDistanceAlongSpline(SegmentIndex * SegmentLength, ESplineCoordinateSpace::World);
			FVector StartTangent = Spline->GetDirectionAtDistanceAlongSpline(SegmentIndex * SegmentLength, ESplineCoordinateSpace::World);
			FVector EndPosition = Spline->GetLocationAtDistanceAlongSpline((SegmentIndex * SegmentLength) + SegmentLength, ESplineCoordinateSpace::World);
			FVector EndTangent = Spline->GetDirectionAtDistanceAlongSpline((SegmentIndex * SegmentLength) + SegmentLength, ESplineCoordinateSpace::World);

			// 스플라인 메시 컴포넌트의 시작과 끝을 설정
			SplineMeshComponent->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
		}
	}
}

void ANodeBase::CrossOut()
{
	CrossMesh->SetVisibility(true);
	NodeMaterial->SetVectorParameterValue(TEXT("Color"), TrueColor);
}

void ANodeBase::HoverOverNode_Implementation()
{
	if (bEnabled)
	{
		ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(nullptr, ACardPlayer::StaticClass()));
		if (!CardPlayer)
		{
			COS_LOG_SCREEN(TEXT("레벨에 ACardPlayer가 존재하지 않습니다."));
			return;
		}

		if (!CardPlayer->PlayerUI->ShouldNodeMapBeBlocked())
		{
			TrueColor = NodeMaterial->K2_GetVectorParameterValue(TEXT("Color"));

			NodeMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::White);
		}
	}
}

void ANodeBase::HoverOffNode_Implementation()
{
	if (bEnabled)
	{
		NodeMaterial->SetVectorParameterValue(TEXT("Color"), TrueColor);
	}
}

void ANodeBase::ClickNode_Implementation()
{
	if (bEnabled)
	{
		ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(nullptr, ACardPlayer::StaticClass()));
		if (!CardPlayer)
		{
			COS_LOG_SCREEN(TEXT("레벨에 ACardPlayer가 존재하지 않습니다."));
			return;
		}

		if (!CardPlayer->PlayerUI->ShouldNodeMapBeBlocked())
		{
			UGameInstance* GameInstance = GetGameInstance();
			if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
			{
				COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
				return;
			}

			IInterface_CardGameInstance::Execute_AddVisitedNodeToInstance(GameInstance, Id);

			if (OnNodeSelected.IsBound())
				OnNodeSelected.Broadcast(this);

			MapEventRef->RunMapEvent(MapEvent);
		}
	}
}
