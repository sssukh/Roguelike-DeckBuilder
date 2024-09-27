#include "UI/UW_MinionUiTop.h"

#include "Components/HorizontalBox.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_StatusIcon.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_MinionUiTop::UUW_MinionUiTop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_StatusIconClass_C(*AssetPath::Blueprint::WBP_StatusIcon_C);
	if (WBP_StatusIconClass_C.Succeeded())
	{
		WBP_StatusIconClass = WBP_StatusIconClass_C.Class;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("WBP_StatusIconClass를 로드하지 못했습니다"));
	}
}

void UUW_MinionUiTop::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(ActiveStatusWobble, 0, 0, EUMGSequencePlayMode::PingPong);
}

UObject* UUW_MinionUiTop::AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately)
{
	COS_IF_CHECK_RETURN(WBP_StatusIconClass, TEXT("UUW_MinionUiTop에서 WBP_StatusIconClass를 설정해주세요!"), nullptr);

	UUserWidget* NewStatusIconWidget = CreateWidget(GetOwningPlayer(), WBP_StatusIconClass);
	ActiveStatusBox->AddChild(NewStatusIconWidget);
	IInterface_StatusIcon::Execute_InitStatusWidget(NewStatusIconWidget, Status, CosGameTags::StatusBar_MinionBar, bShowImmediately);
	return NewStatusIconWidget;
}
