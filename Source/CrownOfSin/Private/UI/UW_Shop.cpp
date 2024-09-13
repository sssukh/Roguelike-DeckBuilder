#include "UI/UW_Shop.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileShopComponent.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Artifacts/Status_Artifact.h"
#include "UI/UW_ShopCard.h"

UUW_Shop::UUW_Shop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CardPanel(nullptr), OutsideButton(nullptr), ExitButton(nullptr)
{
	//설정해야합니다
	// WBP_ShopCardClass
}

void UUW_Shop::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (bDebugDisplay)
	{
		for (int i = 0; i < 20; ++i)
		{
			UUW_ShopCard* NewShopCard = CreateWidget<UUW_ShopCard>(GetWorld(), WBP_ShopCardClass);
			int32 Row = i / RowLength;
			int32 Column = i % RowLength;
			CardPanel->AddChildToUniformGrid(NewShopCard, Row, Column);
		}
	}
}

void UUW_Shop::NativeConstruct()
{
	Super::NativeConstruct();

	ExitButton->OnClicked.AddDynamic(this, &UUW_Shop::OnClicked_ExitButton);
	OutsideButton->OnClicked.AddDynamic(this, &UUW_Shop::OnClicked_OutsideButton);
}

void UUW_Shop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCardsAvailability();
}

void UUW_Shop::UpdateShopCards()
{
	// 상점 카드 패널을 초기화 (모든 자식 위젯 제거)
	CardPanel->ClearChildren();

	// 현재 플레이어의 카드 데이터를 가져옴
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer) return;

	// 상점 카드 리스트를 가져옴
	const TArray<ACardBase*>& ShopCards = CardPlayer->PileShopComponent->Cards;
	
	// 상점 카드 생성 및 그리드에 배치
	for (int32 CardIndex = 0; CardIndex < ShopCards.Num(); ++CardIndex)
	{
		// 새로운 상점 카드 위젯 생성
		UUW_ShopCard* NewShopCard = CreateWidget<UUW_ShopCard>(GetWorld(), WBP_ShopCardClass);

		// 카드 위젯에 해당 카드 데이터를 연결
		NewShopCard->CardActor = ShopCards[CardIndex];

		// 그리드에서 행과 열을 계산
		int32 Row = CardIndex / RowLength;
		int32 Column = CardIndex % RowLength;

		// 카드 위젯을 그리드 패널에 추가
		CardPanel->AddChildToUniformGrid(NewShopCard, Row, Column);
	}

	// 이전 코인 카운트를 초기값(-1)으로 설정
	PreviousCoinCount = -1;

	// 카드 구매 가능 여부를 업데이트
	UpdateCardsAvailability();

	// 상점 UI를 화면에 표시
	SetVisibility(ESlateVisibility::Visible);
}

void UUW_Shop::UpdateCardsAvailability()
{
	// 현재 플레이어의 카드 플레이어 객체를 가져옴
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer) return;

	// 플레이어의 코인 상태(아티팩트)를 가져옴
	UStatus_Artifact* StatusArtifact = CardPlayer->FindComponentByClass<UStatus_Artifact>();

	// 유효한 코인 상태가 존재할 경우
	if (IsValid(StatusArtifact))
	{
		// 코인이 변동되지 않았다면 함수 종료
		if (PreviousCoinCount == StatusArtifact->StatusValue) return;

		// 이전 코인 카운트를 현재 코인 값으로 업데이트
		PreviousCoinCount = StatusArtifact->StatusValue;
	}
	else
	{
		// 유효한 상태가 없을 경우 코인 카운트를 0으로 설정
		PreviousCoinCount = 0;
	}

	// 카드 패널의 모든 자식 위젯(카드)을 순회하며 구매 가능 여부를 업데이트
	for (UWidget* ChildWidget : CardPanel->GetAllChildren())
	{
		// 자식 위젯을 UUW_ShopCard로 캐스팅
		UUW_ShopCard* ShopCard = Cast<UUW_ShopCard>(ChildWidget);
		if (!ShopCard) continue;

		// 코인 수에 따라 카드의 구매 가능 여부를 업데이트
		ShopCard->UpdateFromCoinAmount(PreviousCoinCount);
	}
}

void UUW_Shop::OnClicked_ExitButton()
{
	if (OnOutsideClicked.IsBound())
		OnOutsideClicked.Broadcast();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UUW_Shop::OnClicked_OutsideButton()
{
	if (OnOutsideClicked.IsBound())
		OnOutsideClicked.Broadcast();

	SetVisibility(ESlateVisibility::Collapsed);
}
