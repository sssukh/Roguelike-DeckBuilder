﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardAction/ActionBase.h"


// Sets default values
AActionBase::AActionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

