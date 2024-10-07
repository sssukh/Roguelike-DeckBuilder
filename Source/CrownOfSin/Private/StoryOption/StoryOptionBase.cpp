// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryOption/StoryOptionBase.h"


// Sets default values
AStoryOptionBase::AStoryOptionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStoryOptionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStoryOptionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

