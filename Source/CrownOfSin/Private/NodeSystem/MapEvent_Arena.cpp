// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSystem/MapEvent_Arena.h"

#include "Animation/WidgetAnimation.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_ScreenFade.h"
#include "Utilities/CosLog.h"


UMapEvent_Arena::UMapEvent_Arena()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ScreenFade(*AssetPath::Blueprint::WBP_ScreenFade_C);
	if (WBP_ScreenFade.Succeeded())
	{
		WBP_ScreenFadeClass = WBP_ScreenFade.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ScreenFade를 로드할 수 없습니다"));
	}
}

FGameplayTagContainer UMapEvent_Arena::GetEncounterTags(const FDataTableRowHandle& EncounterTags)
{
	return EncounterTags.DataTable->FindRow<FEncounterData>(EncounterTags.RowName,TEXT("FEncounter"))->GameplayTags;
}

void UMapEvent_Arena::RunMapEvent(FDataTableRowHandle EventData)
{
	UUW_ScreenFade* NewFadeWidget = CreateWidget<UUW_ScreenFade>(UGameplayStatics::GetGameInstance(this), WBP_ScreenFadeClass);
	NewFadeWidget->AddToViewport();
	NewFadeWidget->PlayAnimation(NewFadeWidget->Occlude);

	// 애니메이션 재생이 끝나고 0.01초 뒤에 ChangeLevel 호출
	FTimerHandle ArenaTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ArenaTimerHandle, FTimerDelegate::CreateLambda(
		                                       [this,EventData]()
		                                       {
			                                       ChangeLevel(EventData); // 레벨을 바꾸고 그 전에 필요한 작업들을 해준다.
		                                       }
	                                       ), NewFadeWidget->Occlude->GetEndTime() + 0.01f, false);
}

void UMapEvent_Arena::ChangeLevel(const FDataTableRowHandle& EventData)
{
	FEncounterData* Encounter = EventData.DataTable->FindRow<FEncounterData>(EventData.RowName,TEXT(""));

	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);

	IInterface_CardGameInstance::Execute_AttemptSaveGame(CardGameInstance, FString(), true);

	IInterface_CardGameInstance::Execute_SetCurrentEncounterInInstance(CardGameInstance, *Encounter);

	IInterface_CardGameInstance::Execute_AddDoneStoryEncounterToInstance(CardGameInstance, EventData);

	UGameplayStatics::OpenLevel(this, FName(*Encounter->Level));
}
