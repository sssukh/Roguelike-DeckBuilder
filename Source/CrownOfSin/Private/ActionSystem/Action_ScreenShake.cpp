// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_ScreenShake.h"


// Sets default values
AAction_ScreenShake::AAction_ScreenShake()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_ScreenShake::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_ScreenShake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

