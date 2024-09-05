#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "UW_MainMenu.generated.h"

class UUW_HeroAddBox;
class UUW_MenuButton;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_MainMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

protected:
	/**
     * 새로운 게임을 시작하는 버튼이 클릭되었을 때 호출되는 함수입니다.
     * 이 함수는 선택된 영웅과 덱을 게임 인스턴스에 추가하고, 초기 게임 상태를 설정한 후, 새로운 게임 레벨을 로드합니다.
     */
	UFUNCTION(BlueprintCallable, Category="UW Main Menu Event")
	void OnNewGameButtonClicked();

	/**
     * '계속하기' 버튼이 클릭되었을 때 호출됩니다.
     * 현재 진행 중인 게임을 로드하고, 게임의 입력 모드를 설정하여 게임과 UI 상호작용을 모두 허용합니다.
     */
	UFUNCTION(BlueprintCallable, Category="UW Main Menu Event")
	void OnContinueButtonClicked();

	/**
     * '기록 삭제' 버튼이 클릭되었을 때 호출됩니다.
     * 현재 진행 중인 게임 데이터를 삭제하고, 현재 레벨을 새로고침하여 초기 상태로 되돌립니다.
     */
	UFUNCTION(BlueprintCallable, Category="UW Main Menu Event")
	void OnDropRunButtonClicked();

	/**
     * '종료' 버튼이 클릭되었을 때 호출됩니다.
     * 게임을 종료하고 애플리케이션을 닫습니다.
     */
	UFUNCTION(BlueprintCallable, Category="UW Main Menu Event")
	void OnQuitButtonClicked();



protected:
	/**
     * 게임 인스턴스가 유효한지 확인하고, 게임을 초기화하는 함수입니다.
     * @return 게임 인스턴스가 유효하면 그 인스턴스를 반환하고, 그렇지 않으면 nullptr을 반환합니다.
     */
	UGameInstance* ValidateAndResetGameInstance();

	/**
     * 선택된 영웅과 덱을 처리하고, 영웅을 영구적으로 게임 인스턴스에 추가하는 함수입니다.
     */
	void ProcessSelectedHeroesAndDecks(UGameInstance* GameInstance);

	/**
     * 개별 영웅과 덱을 게임 인스턴스에 추가하고, 영웅의 시작 상태를 설정하는 함수입니다.
     */
	void AddHeroAndDeckToGameInstance(UGameInstance* GameInstance, FHeroDeck& HeroDeck);

	/**
     * 영웅의 시작 상태를 게임 인스턴스에 추가하는 함수입니다.
     */
	void AddStartingStatusesToHero(UGameInstance* GameInstance, const FHeroDeck& HeroDeck);

	/**
     * 게임 준비를 완료하고 새로운 레벨을 로드하는 함수입니다.
     */
	void PrepareAndStartNewGame(UGameInstance* GameInstance);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Main Menu|Designer", meta=(BindWidget))
	UUW_MenuButton* WBP_NewGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Main Menu|Designer", meta=(BindWidget))
	UUW_MenuButton* WBP_DropRunButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Main Menu|Designer", meta=(BindWidget))
	UUW_MenuButton* WBP_ContinueButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Main Menu|Designer", meta=(BindWidget))
	UUW_MenuButton* WBP_QuitButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Main Menu|Designer", meta=(BindWidget))
	UUW_HeroAddBox* WBP_HeroAddBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Main Menu")
	FString CurrentRunName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UW Main Menu", meta = ( RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.Deck"))
	UDataTable* DeckTable;
};
