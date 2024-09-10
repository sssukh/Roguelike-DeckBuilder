// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MinionTrack.h"


// Sets default values
AMinionTrack::AMinionTrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMinionTrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinionTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

