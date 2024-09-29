#include "ActionSystem/Action_CardRewardScreen.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardRewardScreen.h"
#include "UI/UW_Layout_Cos.h"


AAction_CardRewardScreen::AAction_CardRewardScreen()
{
	bAllowSkip = false;
	ScreenText = FText::FromString(TEXT("Choose a Card"));
}

void AAction_CardRewardScreen::PlayAction_Implementation()
{
	// PlayerController가 올바르게 설정되었는지 확인합니다.
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer || !CardPlayer->PlayerUI || !CardPlayer->PlayerUI->WBP_CardRewardScreen)
	{
		UE_LOG(LogTemp, Warning, TEXT("CardPlayer 또는 PlayerUI가 유효하지 않습니다."));
		return;
	}

	// 이전에 연결된 델리게이트가 있으면 해제하여 중복 실행 방지
	CardPlayer->PlayerUI->WBP_CardRewardScreen->OnReturnSelectedCardInRewardScreen.RemoveDynamic(this, &AAction_CardRewardScreen::ContinueActionOnCardSelect);

	// 선택된 카드에 대한 델리게이트 추가
	CardPlayer->PlayerUI->WBP_CardRewardScreen->OnReturnSelectedCardInRewardScreen.AddDynamic(this, &AAction_CardRewardScreen::ContinueActionOnCardSelect);

	// 카드 보상 화면을 업데이트하여 선택할 카드와 텍스트, 스킵 가능 여부를 설정
	CardPlayer->PlayerUI->WBP_CardRewardScreen->UpdateRewardScreen(CardOptions, ScreenText, bAllowSkip);
}

void AAction_CardRewardScreen::ContinueActionOnCardSelect(bool bSkipped, ACardBase* InCard)
{
	Execute_EndAction(this);
}
