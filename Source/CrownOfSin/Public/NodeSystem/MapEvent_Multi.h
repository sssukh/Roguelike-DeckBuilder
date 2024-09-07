#pragma once

#include "CoreMinimal.h"
#include "MapEventComponent.h"
#include "MapEvent_Multi.generated.h"


UCLASS(ClassGroup=("Cos"), Blueprintable, meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEvent_Multi : public UMapEventComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMapEvent_Multi();

public:
	virtual void RunMapEvent(FDataTableRowHandle EventData) override;

protected:
	UMapEventComponent* CreateNewMapEventComponent(TSubclassOf<UMapEventComponent> MapEventClass);

	bool RunSpecificEvent(UMapEventComponent* MapEventComponent, FDataTableRowHandle SpecificEvent);

	void RunRandomEvent(UMapEventComponent* MapEventComponent, const FMapEvent& MapEventStruct);

	// Variables
public:
	// 기본값 아이콘 설정 필요(T_RandomDice)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapEvent Multi|Variables")
	FMapEvent CurrentMapEventStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapEvent Multi|Variables|Components")
	TObjectPtr<UMapEventComponent> CurrentMapEventComponent;
};
