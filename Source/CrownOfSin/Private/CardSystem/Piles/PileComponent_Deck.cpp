// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/Piles/PileComponent_Deck.h"


// Sets default values for this component's properties
UPileComponent_Deck::UPileComponent_Deck()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPileComponent_Deck::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPileComponent_Deck::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

