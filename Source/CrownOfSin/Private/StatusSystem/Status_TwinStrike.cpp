#include "StatusSystem/Status_TwinStrike.h"

#include "CardSystem/CardBase.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_TwinStrike::UStatus_TwinStrike()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Dagger(*AssetPath::Texture::T_Dagger);
	if (T_Dagger.Succeeded())
	{
		Icon = T_Dagger.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Dagger를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("TwinStrike"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}
	Tint = FLinearColor::White;
	TextAlignment = EStatusTextAlignment::BottomRight;
	FriendlyName = FText::FromString(TEXT("Twin Strike"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}


// Called when the game starts
void UStatus_TwinStrike::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_Card_PrePlay);
}

void UStatus_TwinStrike::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 카드 재생 이벤트가 발생했는지 확인
	if (EventTag == CosGameTags::Event_Card_PrePlay)
	{
		// SourceObject가 카드인지 확인하고 ACardBase로 캐스팅
		ACardBase* TargetCard = Cast<ACardBase>(CallingObject);
		COS_IF_CHECK_VOID(TargetCard, TEXT("CallingObject가 ACardBase로 캐스팅되지 않았습니다."));

		AffectedCard =TargetCard;
		// 카드 소유자가 이 컴포넌트의 소유자인지 확인
		if (AffectedCard->GetOwner() == GetOwner())
		{
			// 카드가 공격 태그를 가지고 있는지 확인
			if (AffectedCard->GetGameplayTags().HasTagExact(CosGameTags::Effect_Attack))
			{
				// 상태 값이 0보다 큰 경우에만 실행
				if (StatusValue > 0)
				{
					// 카드의 현재 반복 횟수를 가져와 새로운 반복 횟수 계산
					int32 CurrentRepetitions = AffectedCard->GetCardRepetitions(ECardDataType::Hand);
					int32 NewRepetitionCount = (CurrentRepetitions + 1) * (StatusValue + 1) - 1;

					// 카드의 반복 횟수 설정
					AffectedCard->SetCardRepetitions(ECardDataType::Hand, NewRepetitionCount);

					// TwinStrike 상태 값을 0으로 설정하여 상태 초기화
					SetStatusValue(0, false, false, nullptr);
				}
			}
		}
	}
}
