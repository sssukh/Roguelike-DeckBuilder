#include "UI/UW_MainMenu.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"
#include "StatusSystem/Artifacts/Status_Artifact.h"
#include "UI/UW_HeroAddBox.h"
#include "UI/UW_MenuButton.h"

UUW_MainMenu::UUW_MainMenu(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
                                                                         WBP_NewGameButton(nullptr), WBP_DropRunButton(nullptr),
                                                                         WBP_ContinueButton(nullptr), WBP_QuitButton(nullptr),
                                                                         WBP_HeroAddBox(nullptr), DeckTable(nullptr)
{
	CurrentRunName = FString(TEXT("Run"));
}

void UUW_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("Game Instacne에서 UInterface_CardGameInstance를 상속시켜 재정의 해주세요."));
		return;
	}

	// 저장된 게임 데이터를 로드하려고 시도합니다.
	const bool bLoadSuccess = IInterface_CardGameInstance::Execute_AttemptLoadGame(GameInstance, CurrentRunName, false);

	// 로드에 성공한 경우
	if (bLoadSuccess)
	{
		// '계속하기' 버튼과 '기록 삭제' 버튼을 표시합니다.
		WBP_ContinueButton->SetVisibility(ESlateVisibility::Visible);
		WBP_DropRunButton->SetVisibility(ESlateVisibility::Visible);
	}
	else // 로드에 실패한 경우
	{
		// '새 게임' 버튼과 '영웅 추가' 상자를 표시합니다.
		WBP_NewGameButton->SetVisibility(ESlateVisibility::Visible);
		WBP_HeroAddBox->SetVisibility(ESlateVisibility::Visible);
	}

	// 메뉴 버튼 클릭 이벤트에 대한 바인딩을 설정합니다.
	WBP_NewGameButton->OnMenuButtonClicked.AddDynamic(this, &UUW_MainMenu::OnNewGameButtonClicked);
	WBP_ContinueButton->OnMenuButtonClicked.AddDynamic(this, &UUW_MainMenu::OnContinueButtonClicked);
	WBP_DropRunButton->OnMenuButtonClicked.AddDynamic(this, &UUW_MainMenu::OnDropRunButtonClicked);
	WBP_QuitButton->OnMenuButtonClicked.AddDynamic(this, &UUW_MainMenu::OnQuitButtonClicked);
}

void UUW_MainMenu::OnNewGameButtonClicked()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("Game Instance가 Interface_CardGameInstance 인터페이스를 상속받지 않았습니다."));
		return;
	}

	IInterface_CardGameInstance::Execute_ResetGame(GameInstance);

	// 선택된 영웅과 덱을 가져옵니다.
	TArray<FHeroDeck> SelectedHeroesAndDecks = WBP_HeroAddBox->GetSelectedHeroesAndDecks();

	// 각 영웅과 덱에 대해 반복합니다.
	for (FHeroDeck& HeroesAndDeck : SelectedHeroesAndDecks)
	{
		FString UniqueID;
		// 영웅과 덱을 영구적으로 게임 인스턴스에 추가합니다.
		IInterface_CardGameInstance::Execute_AddPersistentHeroToInstance(GameInstance, HeroesAndDeck.Hero, HeroesAndDeck.Deck, UniqueID);

		// 영웅의 데이터 테이블에서 FMinion 구조체를 찾습니다.
		FMinion* FoundMinion = HeroesAndDeck.Hero.DataTable->FindRow<FMinion>(HeroesAndDeck.Hero.RowName,TEXT(""));
		if (!FoundMinion)
			continue;

		// 영웅의 시작 상태를 반복하여 각 상태를 검사합니다.
		for (TTuple<TSubclassOf<UStatusComponent>, int32> StatusEntry : FoundMinion->StartingStatuses)
		{
			TSubclassOf<UStatusComponent> StatusClass = StatusEntry.Key;
			int32 StatusValue = StatusEntry.Value;

			// 상태 클래스가 UStatus_Artifact의 서브클래스인지 확인합니다.
			if (StatusClass->IsChildOf(UStatus_Artifact::StaticClass()))
			{
				IInterface_CardGameInstance::Execute_AddArtifactToInstance(GameInstance, FStatusData(StatusValue, FGameplayTagContainer(), StatusClass));
			}
		}
	}

	// 플레이어 컨트롤러를 가져옵니다.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// 입력 모드를 게임과 UI를 모두 허용하는 모드로 설정합니다.
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::DoNotLock, true);

	// 현재 노드 맵의 이름을 게임 인스턴스로부터 가져옵니다.
	FString CurrentNodeMap = IInterface_CardGameInstance::Execute_GetCurrentNodeMapFromInstance(GameInstance);

	// 현재 노드 맵으로 레벨을 전환합니다.
	UGameplayStatics::OpenLevel(this, FName(CurrentNodeMap));
}

void UUW_MainMenu::OnContinueButtonClicked()
{
	// 플레이어 컨트롤러를 가져옵니다.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// 입력 모드를 게임과 UI를 모두 허용하는 모드로 설정합니다.
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::DoNotLock, true);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("Game Instance가 Interface_CardGameInstance 인터페이스를 상속받지 않았습니다."));
		return;
	}

	IInterface_CardGameInstance::Execute_AttemptLoadGame(GameInstance, CurrentRunName, true);
}

void UUW_MainMenu::OnDropRunButtonClicked()
{
	// 현재 진행 중인 게임 데이터를 삭제합니다.
	UGameplayStatics::DeleteGameInSlot(CurrentRunName, 0);

	// 현재 레벨의 이름을 가져옵니다.
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);

	// 현재 레벨을 다시 로드하여 초기 상태로 되돌립니다.
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName), true);
}

void UUW_MainMenu::OnQuitButtonClicked()
{
	// 게임을 종료하고 애플리케이션을 닫습니다.
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
