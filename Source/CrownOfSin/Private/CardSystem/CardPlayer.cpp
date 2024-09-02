// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardPlayer.h"

#include "UI/UW_Layout_Cos.h"


// Sets default values
ACardPlayer::ACardPlayer(): ChanceManagerComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACardPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACardPlayer::DisplayScreenLogMessage(FText Message, FColor Color)
{
	PlayerUI->DisplayScreenLogMessage(Message,Color);
}

