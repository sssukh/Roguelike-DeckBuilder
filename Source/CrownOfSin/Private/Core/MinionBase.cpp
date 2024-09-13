// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MinionBase.h"

#include "Core/GameplayTagComponent.h"


// Sets default values
AMinionBase::AMinionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

// Called when the game starts or when spawned
void AMinionBase::BeginPlay()
{
	Super::BeginPlay();
}

FGameplayTagContainer AMinionBase::GetGameplayTags()
{
	return GameplayTagComponent->GameplayTags;
}
