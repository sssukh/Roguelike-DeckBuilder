// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_RevealShop.h"


// Sets default values for this component's properties
UCardEffect_RevealShop::UCardEffect_RevealShop()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_RevealShop::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCardEffect_RevealShop::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

