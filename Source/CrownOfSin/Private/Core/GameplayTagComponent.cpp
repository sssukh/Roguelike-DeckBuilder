// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameplayTagComponent.h"


// Sets default values for this component's properties
UGameplayTagComponent::UGameplayTagComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


