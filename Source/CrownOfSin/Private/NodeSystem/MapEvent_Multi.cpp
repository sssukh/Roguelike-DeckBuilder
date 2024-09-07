#include "NodeSystem/MapEvent_Multi.h"

#include "Libraries/AssetTableRef.h"


UMapEvent_Multi::UMapEvent_Multi()
{
	UTexture2D* MapEventIcon = FAssetReferenceUtility::LoadAssetFromDataTable<UTexture2D>(AssetRefPath::TexturesPath, FName("T_RandomDice"));
	CurrentMapEventStruct.Icon = MapEventIcon;
}

void UMapEvent_Multi::RunMapEvent(FDataTableRowHandle EventData)
{
	CurrentMapEventStruct = *EventData.DataTable->FindRow<FMapEvent>(EventData.RowName,TEXT("FMapEvent in MapEvent_Multi"));

	UActorComponent* FindComponent = GetOwner()->GetComponentByClass(CurrentMapEventStruct.MapEventClass);

	// 전달받은 EventData에 담긴 MapEventClass 정보로 찾은 걸 쓰거나 없으면 새로 생성
	if (IsValid(FindComponent))
	{
		CurrentMapEventComponent = FindComponent;
	}
	else
	{
		CurrentMapEventComponent = NewObject<UActorComponent>(GetOwner(), CurrentMapEventStruct.MapEventClass, FName(TEXT("MapEventComponentCreated")));
		CurrentMapEventComponent->RegisterComponent();
	}

	UMapEventComponent* MapEventComponent = Cast<UMapEventComponent>(CurrentMapEventComponent);

	FDataTableRowHandle RowHandle = CurrentMapEventStruct.SpecificEvent;
	// 있으면 해당 이벤트 전달시키고 실행 
	if (!RowHandle.IsNull())
	{
		MapEventComponent->RunMapEvent(CurrentMapEventStruct.SpecificEvent);
		return;
	}


	// 없으면 랜덤 인카운터를 통해 이벤트 받아오기
	FDataTableRowHandle RandomEncounter;
	MapEventComponent->ChooseRandomEncounter(CurrentMapEventStruct, TMap<FGameplayTag, float>(), RandomEncounter);

	// 받아온 랜덤 이벤트 실행
	MapEventComponent->RunMapEvent(RandomEncounter);
}
