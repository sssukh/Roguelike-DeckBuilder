// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/Story/CardEffect_Story_ArenaEncounter.h"


// Sets default values for this component's properties
UCardEffect_Story_ArenaEncounter::UCardEffect_Story_ArenaEncounter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_Story_ArenaEncounter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCardEffect_Story_ArenaEncounter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

