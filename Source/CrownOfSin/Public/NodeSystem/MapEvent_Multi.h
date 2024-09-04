// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapEventComponent.h"
#include "MapEvent_Multi.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEvent_Multi : public UMapEventComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMapEvent_Multi();

public:
	virtual void RunMapEvent(FDataTableRowHandle EventData) override;

	// Variables
public:
	// 기본값 아이콘 설정 필요(T_RandomDice)
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "MapEvent Multi|Variables")
	FMapEvent CurrentMapEventStruct;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "MapEvent Multi|Variables|Components")
	TObjectPtr<UActorComponent> CurrentMapEventComponent;

};
