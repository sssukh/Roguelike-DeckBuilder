// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MinionBase.h"


// Sets default values
AMinionBase::AMinionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMinionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

