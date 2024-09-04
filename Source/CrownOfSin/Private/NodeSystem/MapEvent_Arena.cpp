// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSystem/MapEvent_Arena.h"

#include "Animation/WidgetAnimation.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UW_ScreenFade.h"


// Sets default values for this component's properties
UMapEvent_Arena::UMapEvent_Arena()
{
	
}



FGameplayTagContainer UMapEvent_Arena::GetEncounterTags(FDataTableRowHandle EncounterTags)
{
	return EncounterTags.DataTable->FindRow<FEncounter>(EncounterTags.RowName,TEXT("FEncounter"))->GameplayTags;
}

void UMapEvent_Arena::RunMapEvent(FDataTableRowHandle EventData)
{
	UUW_ScreenFade* FadeWidget = CreateWidget<UUW_ScreenFade>(UGameplayStatics::GetGameInstance(this),UUW_ScreenFade::StaticClass(),"Screen_Fade");

	FadeWidget->AddToViewport();

	FadeWidget->PlayAnimation(FadeWidget->Occlude);

	// 애니메이션 재생이 끝나고 0.01초 뒤에 ChangeLevel 호출
	FTimerHandle ArenaTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ArenaTimerHandle,FTimerDelegate::CreateLambda(
	[&]()
	{
		// 레벨을 바꾸고 그 전에 필요한 작업들을 해준다.
		ChangeLevel(EventData);
	}
	),FadeWidget->Occlude->GetEndTime()+0.01f,false);
}

void UMapEvent_Arena::ChangeLevel(FDataTableRowHandle EventData)
{
	FEncounter* Encounter = EventData.DataTable->FindRow<FEncounter>(EventData.RowName,TEXT("MapEvent in Arena"));
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetOuter());

	if(GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		// 더미 스트링
		FString tmp;
		IInterface_CardGameInstance::Execute_AttemptSaveGame(GameInstance,tmp,true);

		// GameInstance 내부의 CurrentEncounter를 매개변수 Encounter로 설정해준다.
		IInterface_CardGameInstance::Execute_SetCurrentEncounterInInstance(GameInstance,*Encounter);

		// GameInstance 내부의 DoneStoryEncounters에 EventData를 추가해준다.
		IInterface_CardGameInstance::Execute_AddDoneStoryEncounterToInstance(GameInstance,EventData);

		// 해당 레벨을 연다.
		UGameplayStatics::OpenLevel(this,FName(*Encounter->Level));
	}
}

