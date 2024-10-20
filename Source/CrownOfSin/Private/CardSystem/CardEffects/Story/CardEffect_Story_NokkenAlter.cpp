﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/Story/CardEffect_Story_NokkenAlter.h"


// Sets default values for this component's properties
UCardEffect_Story_NokkenAlter::UCardEffect_Story_NokkenAlter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UCardEffect_Story_NokkenAlter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UCardEffect_Story_NokkenAlter::ResolveCardEffect(AActor* TargetActor)
{
	return Super::ResolveCardEffect(TargetActor);
}

