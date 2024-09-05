// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Skittish.h"


// Sets default values for this component's properties
UStatus_Skittish::UStatus_Skittish()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatus_Skittish::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatus_Skittish::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

