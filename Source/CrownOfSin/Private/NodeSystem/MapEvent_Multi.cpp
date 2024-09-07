#include "NodeSystem/MapEvent_Multi.h"

#include "Libraries/AssetTableRef.h"


UMapEvent_Multi::UMapEvent_Multi()
{
	UTexture2D* MapEventIcon = FAssetReferenceUtility::LoadAssetFromDataTable<UTexture2D>(AssetRefPath::TexturesPath, FName("T_RandomDice"));
	CurrentMapEventStruct.Icon = MapEventIcon;
}

void UMapEvent_Multi::RunMapEvent(FDataTableRowHandle EventData)
{
	// 데이터 테이블이 유효한지 확인
	if (!EventData.DataTable)
	{
		COS_LOG_SCREEN_ERROR(TEXT("EventData.DataTable is nullptr"));
		return;
	}

	// 데이터 테이블에서 Row를 찾음
	FMapEvent* MapEvent = EventData.DataTable->FindRow<FMapEvent>(EventData.RowName, TEXT("MapEvent_Multi의 FMapEvent"));
	if (!MapEvent)
	{
		COS_LOG_SCREEN_ERROR(TEXT("Row not found: %s"), *EventData.RowName.ToString());
		return;
	}

	CurrentMapEventStruct = *MapEvent; // 유효한 Row를 가져왔으므로 할당

	// MapEventClass에 해당하는 컴포넌트를 가져옴
	UMapEventComponent* FindComponent = Cast<UMapEventComponent>(GetOwner()->GetComponentByClass(CurrentMapEventStruct.MapEventClass));

	// 컴포넌트가 유효한지 확인, 없으면 새로 생성
	CurrentMapEventComponent = IsValid(FindComponent) ? FindComponent : CreateNewMapEventComponent(CurrentMapEventStruct.MapEventClass);

	// 특정 이벤트가 있으면 실행, 없으면 랜덤 이벤트 실행
	if (!RunSpecificEvent(CurrentMapEventComponent, CurrentMapEventStruct.SpecificEvent))
	{
		RunRandomEvent(CurrentMapEventComponent, CurrentMapEventStruct);
	}
}

UMapEventComponent* UMapEvent_Multi::CreateNewMapEventComponent(TSubclassOf<UMapEventComponent> MapEventClass)
{
	// 새로운 컴포넌트 생성 및 등록
	UMapEventComponent* NewMapEventComponent = NewObject<UMapEventComponent>(GetOwner(), MapEventClass, FName(TEXT("MapEventComponentCreated")));
	if (NewMapEventComponent)
	{
		NewMapEventComponent->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("새로운 MapEventComponent를 생성하지 못했습니다."));
	}
	return NewMapEventComponent;
}

bool UMapEvent_Multi::RunSpecificEvent(UMapEventComponent* MapEventComponent, FDataTableRowHandle SpecificEvent)
{
	// 특정 이벤트가 있는 경우 해당 이벤트 실행
	if (!SpecificEvent.IsNull())
	{
		MapEventComponent->RunMapEvent(SpecificEvent);
		return true;
	}
	return false;
}

void UMapEvent_Multi::RunRandomEvent(UMapEventComponent* MapEventComponent, const FMapEvent& MapEventStruct)
{
	// 랜덤 이벤트 처리
	FDataTableRowHandle RandomEncounter;
	MapEventComponent->ChooseRandomEncounter(MapEventStruct, RandomEncounter);

	if (!RandomEncounter.IsNull())
	{
		// 랜덤 이벤트 실행
		MapEventComponent->RunMapEvent(RandomEncounter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to choose a random encounter event"));
	}
}
