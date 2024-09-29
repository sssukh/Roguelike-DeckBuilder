// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_BrokenShield.h"

#include "Core/DispatcherHubLocalComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "Libraries/DelayHelper.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Armor.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_BrokenShield::UStatus_Artifact_BrokenShield()
{
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_BrokenShield(*AssetPath::Texture::T_BrokenShield);
	if(T_BrokenShield.Succeeded())
	{
		Icon = T_BrokenShield.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_BrokenShield을 로드할 수 없습니다."));
	}

	Tint = FLinearColor(0.8f,0.5f,0.06f);

	bCanBeZero = true;

	
	
	FToolTipValue ToolTipValue;
	ToolTipValue.bValued=true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Artifacts(*AssetPath::DataTable::DT_Tooltips_Artifacts);
	if(DT_Tooltips_Artifacts.Succeeded())
	{
		ToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Artifacts.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Tooltips_Artifacts를 로드할 수 없습니다."));
	}
	
	ToolTipValue.ToolTipTable.RowName = TEXT("BrokenShield");
	Tooltips.Add(ToolTipValue);

	FToolTipValue ToolTipValue2;
	ToolTipValue2.bValued =false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if(DT_Tooltips_Statuses.Succeeded())
	{
		ToolTipValue2.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Tooltips_Statuses를 로드할 수 없습니다."));
	}
	
	ToolTipValue2.ToolTipTable.RowName = TEXT("Armor");
	Tooltips.Add(ToolTipValue2);
	
	
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	GameplayTags.AddTag(CosGameTags::Rarity_Common);
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}


// Called when the game starts
void UStatus_Artifact_BrokenShield::BeginPlay()
{
	Super::BeginPlay();

	UDelayHelper* DelayHelper = NewObject<UDelayHelper>(this);

	// 델리게이트 생성 및 바인딩
	FConditionDelegate ConditionDelegate;
	ConditionDelegate.BindUObject(this, &UStatus_Artifact_BrokenShield::CheckMainHeroEnabled);

	FOnLoopDelegate OnLoopDelegate;
	
	FOnCompleteDelegate OnCompleteDelegate;
	OnCompleteDelegate.BindUObject(this, &UStatus_Artifact_BrokenShield::BindEvent);
	
	DelayHelper->DelayWhile(ConditionDelegate, OnLoopDelegate, OnCompleteDelegate, 0.0f);
}

void UStatus_Artifact_BrokenShield::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(EventTag == CosGameTags::Event_PostAttack)
	{
		UActorComponent* ActorComponent = Cast<UActorComponent>(CallingObject);

		if(ActorComponent->GetOwner()->GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			IInterface_CardTarget::Execute_AddToStatus(ActorComponent->GetOwner()->GetOwner(),UStatus_Armor::StaticClass(),StatusValue,true,nullptr);
		}
	}
}

bool UStatus_Artifact_BrokenShield::CheckMainHeroEnabled()
{
	// 멤버변수 MainHero로 받아옴
	return !IInterface_CardGameInstance::Execute_GetMainHero(UFunctionLibrary_Singletons::GetCardGameInstance(this),MainHero);
}

void UStatus_Artifact_BrokenShield::BindEvent()
{
	if(UDispatcherHubLocalComponent* MainHeroDispatcher = MainHero->FindComponentByClass<UDispatcherHubLocalComponent>())
	{
		MainHeroDispatcher->BindEventToHub(this,CosGameTags::Event_PostAttack);
	}
}



