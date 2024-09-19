// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_Appear.h"


// Sets default values
AAction_Appear::AAction_Appear()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AAction_Appear::BeginPlay()
{
	Super::BeginPlay();
	
}

