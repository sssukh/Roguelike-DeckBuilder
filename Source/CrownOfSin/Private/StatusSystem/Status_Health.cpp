// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Health.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_SimpleAnim.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/MinionBase.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_Health::UStatus_Health()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int32 UStatus_Health::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon,
	bool bRefreshAppearance, UObject* InPayload)
{
	if(InAmount>0)
	{
		return Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);
	}

	IncomingStatusChange = InAmount;

	UDispatcherHubComponent* OwnersDispatcherHub = nullptr;
	GetOwnersDispatcherHub(OwnersDispatcherHub);

	OwnersDispatcherHub->CallEvent(CosGameTags::Event_PreTakeDamage,this);

	bool ParentResultLessEqualThanZero = Super::AddStatusValue(IncomingStatusChange, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload)<=0;
	
	AMinionBase* OwnerMinion = Cast<AMinionBase>(GetOwner());
	
	if(ParentResultLessEqualThanZero)
	{
		OwnerMinion->bDead = true;
	}

	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	 ActionManagerSubsystem->CreateAndQueueAction<AAction_SimpleAnim>([&](AAction_SimpleAnim* Action_ModifyStatus)
	{
		Action_ModifyStatus->Puppet = OwnerMinion->Puppet;

		FGameplayTag InAnimTag;
		
		if(ParentResultLessEqualThanZero)
		{
			InAnimTag = CosGameTags::Anim_Die;
		}
		else
		{
			if(IncomingStatusChange!=0)
			{
				InAnimTag = CosGameTags::Anim_Flinch;
			}
			else
			{
				InAnimTag = CosGameTags::Anim_Block;
			}
		}

		Action_ModifyStatus->Animation = InAnimTag;
	});

	if(InAmount<0)
	{
		OwnersDispatcherHub->CallEvent(CosGameTags::Event_PostTakeDamage,this);

		if(StatusValue<=0)
		{
			OwnerMinion->RemoveFromGame();
		}
	}

	return StatusValue;
}



