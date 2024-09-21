// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_SimpleAnim.h"

#include "Interfaces/Interface_CardPuppet.h"
#include "Utilities/CosLog.h"


// Sets default values
AAction_SimpleAnim::AAction_SimpleAnim()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_SimpleAnim::PlayAction_Implementation()
{
	if(!Puppet->GetClass()->ImplementsInterface(UInterface_CardPuppet::StaticClass()))
	{
		COS_SCREEN(TEXT("인터페이스 CardPuppet을 상속받지 않았습니다."));
		return;
	}

	IInterface_CardPuppet::Execute_AnimatePuppet(Puppet,Animation);

	Execute_EndAction(this);
}

