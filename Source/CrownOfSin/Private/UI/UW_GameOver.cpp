#include "UI/UW_GameOver.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"

UUW_GameOver::UUW_GameOver(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), GameOverText(nullptr), MenuButton(nullptr), QuitButton(nullptr), FadeIn(nullptr)
{
	TextToShow = FText::FromString(TEXT("Game Over"));
}

void UUW_GameOver::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButton->OnClicked.AddDynamic(this, &UUW_GameOver::OnClicked_MenuButton);
	QuitButton->OnClicked.AddDynamic(this, &UUW_GameOver::OnClicked_QuitButton);
}

void UUW_GameOver::ShowGameOver(const FText& InText, FLinearColor InColorAndOpacitySpecifiedColor)
{
	GameOverText->SetText(InText);
	GameOverText->SetColorAndOpacity(InColorAndOpacitySpecifiedColor);
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(FadeIn);
}

void UUW_GameOver::OnClicked_MenuButton()
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	FString SaveID = IInterface_CardGameInstance::Execute_GetInstanceSaveId(CardGameInstance);
	UGameplayStatics::DeleteGameInSlot(SaveID, 0);
	IInterface_CardGameInstance::Execute_ResetGame(CardGameInstance);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level, true);
}

void UUW_GameOver::OnClicked_QuitButton()
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	FString SaveID = IInterface_CardGameInstance::Execute_GetInstanceSaveId(CardGameInstance);
	UGameplayStatics::DeleteGameInSlot(SaveID, 0);
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
