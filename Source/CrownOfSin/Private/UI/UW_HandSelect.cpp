// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_HandSelect.h"

#include "CardSystem/CardBase.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "UI/UW_CardListCard.h"

UUW_HandSelect::UUW_HandSelect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_HandSelect::NativeConstruct()
{
	Super::NativeConstruct();

	// ConfirmButton의 OnClicked에 함수 바인드
	ConfirmButton->OnClicked.AddDynamic(this, &UUW_HandSelect::ConfirmButtonOnClicked);
}

void UUW_HandSelect::InitializeEvent(int32 CardCount, UUW_CardHand* InSourceHand, const FGameplayTagContainer& InValidCardTags)
{
	if(CardCount<=0)
		return;

	// 멤버 변수들 초기화
	CardSelectionBox->ClearChildren();

	SourceHand = InSourceHand;

	ValidCardTags = InValidCardTags;
	
	const FString string = FString::Printf(TEXT("Select %d Cards"), CardCount);
	
	CommandText->SetText(FText::FromString(string));

	// CardCount만큼 카드 생성
	for(int i=0;i<CardCount;++i)
	{
		FName UniqueComponentName = MakeUniqueObjectName(this, UUW_CardListCard::StaticClass());

		UUW_CardListCard* NewCardListCard = CreateWidget<UUW_CardListCard>(GetWorld(), UUW_CardListCard::StaticClass(),UniqueComponentName);

		CardSelectionBox->AddChild(NewCardListCard);

		NewCardListCard->SetVisibility(ESlateVisibility::Visible);

		NewCardListCard->OnCardClicked.AddDynamic(this, &UUW_HandSelect::ReturnCardIfValid);
	}
}


bool UUW_HandSelect::EnableConfirmIfFull()
{
	TArray<ACardBase*> ContainedCards;
	
	bool bIsFull = GetContainedCards(ContainedCards);

	ConfirmButton->SetIsEnabled(bIsFull);

	return bIsFull;
}

bool UUW_HandSelect::GetContainedCards(TArray<ACardBase*>& ContainedCards)
{
	TArray<UWidget*> Widgets = CardSelectionBox->GetAllChildren();

	for (UWidget* Widget : Widgets)
	{
		if (UUW_CardListCard* CardListCard = Cast<UUW_CardListCard>(Widget))
		{
			ContainedCards.Add(CardListCard->CardActor);
		}
	}
	
	return Widgets.Num()==ContainedCards.Num();
}

void UUW_HandSelect::ConfirmButtonOnClicked()
{
	TArray<ACardBase*> ContainedCards;
	if(!GetContainedCards(ContainedCards))
	{
		return;
	}

	OnConfirmCards.Broadcast(ContainedCards);

	for (UWidget* ChildWidget : CardSelectionBox->GetAllChildren())
	{
		UUW_CardListCard* CardListCard = Cast<UUW_CardListCard>(ChildWidget);

		CardListCard->PairedWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
