// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_JackOLantern.h"

#include "Core/MinionBase.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/Status_Fear.h"
#include "StatusSystem/Status_Health.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_JackOLantern::UStatus_Artifact_JackOLantern()
{
	AffectedHealthComponent = CreateDefaultSubobject<UStatus_Health>(TEXT("Status_Health"));

	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_JackOLantern(*AssetPath::Texture::T_JackOLantern);
	if(T_JackOLantern.Succeeded())
	{
		Icon = T_JackOLantern.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_JackOLantern을 로드할 수 없습니다."));
	}

	Tint = FLinearColor(0.8f,0.56f,0.06f);

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("JackOLantern");
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
	
	ToolTipValue2.ToolTipTable.RowName = TEXT("Fear");
	Tooltips.Add(ToolTipValue2);
	
	FriendlyName = FText::FromString("Jack-O'Lantern");
	
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
void UStatus_Artifact_JackOLantern::BeginPlay()
{
	Super::BeginPlay();
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(CosGameTags::Event_PostTakeDamage);
	GameplayTagContainer.AddTag(CosGameTags::Event_PreTakeDamage);
	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this,GameplayTagContainer);
	
}

void UStatus_Artifact_JackOLantern::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(!IsValid(CallingObject))
	{
		return;
	}

	if(EventTag == CosGameTags::Event_PostTakeDamage)
	{
		if(CallingObject == AffectedHealthComponent)
		{
			COS_CHECK(AffectedHealthComponent->GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()));

			IInterface_CardTarget::Execute_AddToStatus(AffectedHealthComponent->GetOwner(),UStatus_Fear::StaticClass(),StatusValue,true,nullptr);
			return;
		}
	}
	else if(EventTag == CosGameTags::Event_PreTakeDamage)
	{
		AffectedHealthComponent = nullptr;

		UStatus_Health* Status_Health = Cast<UStatus_Health>(CallingObject);

		if(!Status_Health||FMath::Abs(Status_Health->IncomingStatusChange)<ActivationThreshold)
		{
			return;
		}

		AMinionBase* Minion = Cast<AMinionBase>(Status_Health->GetOwner());

		if(Minion->GetGameplayTags().HasTagExact(CosGameTags::Target_Enemy))
		{
			AffectedHealthComponent = Status_Health;
		}
	}
}


