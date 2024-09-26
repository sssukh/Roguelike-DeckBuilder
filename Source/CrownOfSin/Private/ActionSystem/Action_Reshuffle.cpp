#include "ActionSystem/Action_Reshuffle.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/AssetPath.h"
#include "Libraries/DelayedForEachLoopHelper.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Card.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_Layout_Cos.h"
#include "UI/UW_CardListButton.h"
#include "UI/UW_Anchor.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


AAction_Reshuffle::AAction_Reshuffle(): TransferAnimSpeedCurve(nullptr), TransferAnimScaleCurve(nullptr), CardPlayerReference(nullptr), PlayerUIReference(nullptr), HandWidgetReference(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve_Decelerate_Float(*AssetPath::Curve::Curve_Decelerate_Float);
	if (Curve_Decelerate_Float.Succeeded())
	{
		TransferAnimSpeedCurve = Curve_Decelerate_Float.Object;
	}
	else
	{
		COS_LOG_SCREEN(TEXT("Curve_Decelerate_Float를 로드하지 못했습니다."));
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve_Parabel_Float(*AssetPath::Curve::Curve_Parabel_Float);
	if (Curve_Parabel_Float.Succeeded())
	{
		TransferAnimScaleCurve = Curve_Parabel_Float.Object;
	}
	else
	{
		COS_LOG_SCREEN(TEXT("Curve_Parabel_Float를 로드하지 못했습니다."));
	}
}

void AAction_Reshuffle::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_Reshuffle::SetUpAction_Implementation()
{
	CardPlayerReference = UFunctionLibrary_Singletons::GetCardPlayer(this);
	PlayerUIReference = CardPlayerReference->PlayerUI;
	HandWidgetReference = PlayerUIReference->WBP_CardHand;
}

void AAction_Reshuffle::PlayAction_Implementation()
{
	DiscardPilePosition = HandWidgetReference->GetAnchorWidgetPosition(CosGameTags::Pile_Discard);
	DrawPilePosition = HandWidgetReference->GetAnchorWidgetPosition(CosGameTags::Pile_Draw);

	// 루프 바디 함수 정의
	auto LoopBody = [&](ACardBase* InCard, int32 Index)
	{
		int32 LastIndex = Cards.Num() - 1;

		PlayerUIReference->UpdatePileWidgetAmount(CosGameTags::Pile_Discard, LastIndex - Index);

		FWidgetTransform StartTransform = FWidgetTransform(DiscardPilePosition, FVector2D::ZeroVector, FVector2D::ZeroVector, 0.0f);
		UUW_Card* NewCardWidget = HandWidgetReference->CreateAndSetupCardWidget(InCard, StartTransform, false);
		CardWidgets.Add(NewCardWidget);

		NewCardWidget->OnRemoved.AddDynamic(this, &AAction_Reshuffle::UpdateDrawOnReach);
		FTransferAnim NewTransferAnim;
		NewTransferAnim.StartWidget = PlayerUIReference->WBP_CardListButton_Discard;
		NewTransferAnim.MidWidget = PlayerUIReference->WBP_CardHand->WBP_ReshuffleAnchor;
		NewTransferAnim.EndWidget = PlayerUIReference->WBP_CardListButton_Draw;
		NewTransferAnim.ScaleModifier = 0.5f;
		NewTransferAnim.AngleModifier = -0.5f;
		NewTransferAnim.Offset = FVector2D(40.0f, 40.0f);
		NewTransferAnim.SpeedCurve = TransferAnimSpeedCurve;
		NewTransferAnim.ScaleCurve = TransferAnimScaleCurve;
		NewCardWidget->SetupBezierFollow(NewTransferAnim);
	};

	// 완료 함수 정의
	auto Completed = [this]()
	{
		// 루프 완료 후 처리 로직
	};

	// 헬퍼 객체 생성 및 루프 시작
	FDelayedForEachLoopHelper<ACardBase*>* DelayedForEachLoopHelper = new FDelayedForEachLoopHelper<ACardBase*>(GetWorld(), Cards, 1, CardDelay, LoopBody, Completed);
}

void AAction_Reshuffle::UpdateDrawOnReach(ACardBase* InRemovedCard, UUserWidget* InRemovedWidget)
{
	int32 FindIndex = Cards.Find(InRemovedCard);
	PlayerUIReference->UpdatePileWidgetAmount(CosGameTags::Pile_Draw, FindIndex + 1);

	if (InRemovedCard == Cards.Last())
	{
		Execute_EndAction(this);
	}
}
