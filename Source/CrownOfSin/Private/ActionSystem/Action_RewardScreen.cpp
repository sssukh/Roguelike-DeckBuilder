#include "ActionSystem/Action_RewardScreen.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Layout_Cos.h"
#include "UI/UW_RewardScreen.h"


AAction_RewardScreen::AAction_RewardScreen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAction_RewardScreen::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_RewardScreen::PlayAction_Implementation()
{
	// PlayerController 객체가 유효한지 확인
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer || !CardPlayer->PlayerUI || !CardPlayer->PlayerUI->WBP_RewardScreen)
	{
		UE_LOG(LogTemp, Warning, TEXT("CardPlayer 또는 PlayerUI가 유효하지 않습니다."));
		return;
	}

	// 보상 화면을 나타냄
	CardPlayer->PlayerUI->WBP_RewardScreen->Appear();

	// 보상 화면의 표시가 끝난 후 액션을 종료
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
	{
		Execute_EndAction(this);
	}));
}
