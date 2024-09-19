// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_UpdateTrack.h"


// Sets default values
AAction_UpdateTrack::AAction_UpdateTrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_UpdateTrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_UpdateTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

