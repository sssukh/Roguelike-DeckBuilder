// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardAction/Action_DispatcherEvent.h"


// Sets default values
AAction_DispatcherEvent::AAction_DispatcherEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AAction_DispatcherEvent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_DispatcherEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

