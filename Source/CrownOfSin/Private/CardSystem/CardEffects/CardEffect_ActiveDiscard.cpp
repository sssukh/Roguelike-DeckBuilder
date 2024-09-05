// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_ActiveDiscard.h"


// Sets default values for this component's properties
UCardEffect_ActiveDiscard::UCardEffect_ActiveDiscard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_ActiveDiscard::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCardEffect_ActiveDiscard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

