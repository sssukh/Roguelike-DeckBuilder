// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Poisoned.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_SimpleAnim.h"
#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Health.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Poisoned::UStatus_Poisoned()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Poison(*AssetPath::Texture::T_Poison);
	if (T_Poison.Succeeded())
	{
		Icon = T_Poison.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Poison를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Poison"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	TextAlignment = EStatusTextAlignment::BottomRight;
	Priority = 50.0f;
	FriendlyName = FText::FromString(TEXT("Poison"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Debuff);
}


// Called when the game starts
void UStatus_Poisoned::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_TurnStart);
	}
}

void UStatus_Poisoned::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 턴 시작 이벤트인지 확인
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		// 이벤트를 호출한 객체가 이 컴포넌트의 소유자인지 확인
		if (CallingObject == GetOwner())
		{
			// 소유자 액터를 가져옴
			AActor* OwnerActor = Cast<AActor>(CallingObject);
			COS_IF_CHECK_VOID(OwnerActor, TEXT("SourceObject가 AActor로 캐스팅되지 않았습니다."));

			// 소유자에게 Health 상태 컴포넌트가 있는지 확인
			if (UStatus_Health* StatusHealth = OwnerActor->FindComponentByClass<UStatus_Health>())
			{
				// 독으로 인한 피해를 적용
				StatusHealth->SubtractStatusValue(StatusValue, true);

				// 현재 Poison 상태가 유효하면 Poison 상태 값을 감소
				if (IsValid(this))
				{
					SubtractStatusValue(1);

					// 소유자에게서 Puppet 액터를 가져옴
					AActor* Puppet = IInterface_CardTarget::Execute_GetPuppet(OwnerActor);

					// ActionManagerSubsystem 가져오기
					UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();

					// 애니메이션 실행을 위한 액션을 큐에 추가
					ActionManagerSubsystem->CreateAndQueueAction<AAction_SimpleAnim>([&](AAction_SimpleAnim* NewSimpleAnimAction)
					{
						// 독 피해에 따른 피격 애니메이션 설정
						NewSimpleAnimAction->Puppet = Puppet;
						NewSimpleAnimAction->Animation = CosGameTags::Anim_Flinch;
						NewSimpleAnimAction->EndDelay = 1.5f;
					});
				}
			}
		}
	}


	if (EventTag == CosGameTags::Event_TurnStart)
	{
		if (CallingObject == GetOwner())
		{
			if (AActor* CallingActor = Cast<AActor>(CallingObject))
			{
				if (UStatus_Health* StatusHealth = CallingActor->FindComponentByClass<UStatus_Health>())
				{
					StatusHealth->SubtractStatusValue(StatusValue, true);

					if (IsValid(this))
					{
						SubtractStatusValue(1);

						AActor* Puppet = IInterface_CardTarget::Execute_GetPuppet(CallingActor);

						UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
						ActionManagerSubsystem->CreateAndQueueAction<AAction_SimpleAnim>([&](AAction_SimpleAnim* NewSimpleAnimAction)
						{
							NewSimpleAnimAction->Puppet = Puppet;
							NewSimpleAnimAction->Animation = CosGameTags::Anim_Flinch;
							NewSimpleAnimAction->EndDelay = 1.5f;
						});
					}
				}
			}
		}
	}
}
