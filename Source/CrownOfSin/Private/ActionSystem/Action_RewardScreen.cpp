// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_RewardScreen.h"


// Sets default values
AAction_RewardScreen::AAction_RewardScreen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_RewardScreen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_RewardScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

