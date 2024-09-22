// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/CardStatuses/Status_Card_InterruptOnPlayerUse.h"


// Sets default values for this component's properties
UStatus_Card_InterruptOnPlayerUse::UStatus_Card_InterruptOnPlayerUse()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatus_Card_InterruptOnPlayerUse::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatus_Card_InterruptOnPlayerUse::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

