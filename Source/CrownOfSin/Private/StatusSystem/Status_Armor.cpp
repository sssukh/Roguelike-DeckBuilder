#include "StatusSystem/Status_Armor.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_SplashText.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UStatus_Armor::UStatus_Armor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Armor"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Shield_Metal(*AssetPath::Texture::T_Shield_Metal);
	if (T_Shield_Metal.Succeeded())
	{
		Icon = T_Shield_Metal.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Shield_Metal를 로드하지 못했습니다."));
	}

	Tint = FLinearColor(0.4f, 0.6f, 1.0f, 1.0f);
	SlotType = EStatusSlot::OverlapBar;
	TextAlignment = EStatusTextAlignment::Center;
	Priority = 50.0f;
	FriendlyName = FText::FromString(TEXT("Armor"));
}

void UStatus_Armor::BeginPlay()
{
	Super::BeginPlay();

	FGameplayTagContainer Tags;
	Tags.AddTag(CosGameTags::Event_IncomingAttack);
	Tags.AddTag(CosGameTags::Event_TurnStart);

	UDispatcherHubComponent* OwnersDispatcherHub;
	if (GetOwnersDispatcherHub(OwnersDispatcherHub))
	{
		OwnersDispatcherHub->BindMultipleEventsToHub(this, Tags);
	}
}

void UStatus_Armor::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 턴이 시작될 때 방어력 값을 0으로 설정
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		SetStatusValue(0, false, false, nullptr);
	}

	// 공격이 들어올 때 방어력을 처리
	else if (EventTag == CosGameTags::Event_IncomingAttack)
	{
		// 공격 효과 컴포넌트가 유효한지 검사
		UCardEffectComponent* CardEffectComponent = Cast<UCardEffectComponent>(CallingObject);
		COS_IF_CHECK_VOID(CardEffectComponent, TEXT("CallingObject를 CardEffectComponent로 변환 할 수없습니다. "));

		// 기존 방어력을 저장
		OldArmor = StatusValue;

		// 방어력 감소 처리
		SubtractStatusValue(CardEffectComponent->EffectValue, false, false, nullptr);

		// 남은 공격력 계산 (방어력이 공격을 모두 막으면 0으로 설정)
		CardEffectComponent->EffectValue = FMath::Max(CardEffectComponent->EffectValue - OldArmor, 0);

		// 공격이 방어력에 의해 완전히 막혔을 때, 'Blocked' 텍스트를 화면에 표시
		if (CardEffectComponent->EffectValue == 0)
		{
			if (GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
			{
				AActor* Puppet = IInterface_CardTarget::Execute_GetPuppet(GetOwner());

				UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
				ActionManagerSubsystem->CreateAndQueueAction<AAction_SplashText>([&](AAction_SplashText* NewActionSplashText)
				{
					NewActionSplashText->Puppet = Puppet;
					NewActionSplashText->DisplayText = FText::FromString(TEXT("Blocked"));
					NewActionSplashText->Color = FLinearColor::White;
					NewActionSplashText->EndDelay = -1.0f;
				}, ESpawnActorCollisionHandlingMethod::Undefined);
			}
		}
	}
}
