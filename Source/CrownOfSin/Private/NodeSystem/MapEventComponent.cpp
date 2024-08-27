// Fill out your copyright notice in the Description page of Project Settings.


#include "CrownOfSin/Public/NodeSystem/MapEventComponent.h"


// Sets default values for this component's properties
UMapEventComponent::UMapEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMapEventComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMapEventComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMapEventComponent::RunMapEvent(FDataTableRowHandle EventData)
{
}

