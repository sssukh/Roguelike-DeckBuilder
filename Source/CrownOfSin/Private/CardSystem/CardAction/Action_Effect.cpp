// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardAction/Action_Effect.h"


// Sets default values
AAction_Effect::AAction_Effect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_Effect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_Effect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

