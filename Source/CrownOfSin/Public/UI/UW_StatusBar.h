#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_StatusBar.generated.h"

class UUW_StatusIcon;
class UVerticalBox;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StatusBar : public UUserWidget, public IInterface_StatusWidget
{
	GENERATED_BODY()

public:
	UUW_StatusBar(const FObjectInitializer& Initializer);

public:
	/**
	 * 현재 활성화된 StatusBox(수직 또는 수평)의 모든 상태 아이콘을 제거합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Status Bar Event")
	void ClearAllStatuses();

	/**
	 * 현재 활성화된 StatusBox(수직 또는 수평)에 있는 상태 아이콘의 개수를 반환합니다.
	 */
	UFUNCTION(BlueprintPure, Category="UW Status Bar Event")
	int32 GetStatusCount();

	/**
	 * 현재 StatusBox(수직 또는 수평)에 있는 상태 아이콘 중 가시 상태인 아이콘의 개수를 반환합니다.
	 * OutVisibleRewardCount에 가시 상태의 아이콘 개수를 저장하며, 성공 시 true를 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Status Bar Event")
	bool GetNumberOfVisibleRewards(int32& OutVisibleRewardCount);

	/**
	 * 현재 StatusBox(수직 또는 수평)에 있는 가시 상태의 모든 상태 아이콘과 상호작용합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Status Bar Event")
	void InteractWithAllStatuses();


	/*========================================================================================
	*	Interface_StatusWidget
	=========================================================================================*/
public:
	/**
	 * 상태 컴포넌트에 해당하는 새로운 상태 아이콘을 생성하고, 레이아웃에 추가합니다.
	 * 상태 아이콘은 bVertical 플래그에 따라 수직 또는 수평 레이아웃에 추가됩니다.
	 */
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadOnly, Category="UW Status Bar|Designer", meta=(BindWidget))
	TObjectPtr<UHorizontalBox> StatusBoxHorizontal;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Bar|Designer", meta=(BindWidget))
	TObjectPtr<UVerticalBox> StatusBoxVertical;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Bar|Config|Setup")
	TSubclassOf<UUW_StatusIcon> WBP_StatusIconClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Bar", meta=(ExposeOnSpawn="true"))
	bool bVertical = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Bar", meta=(ExposeOnSpawn="true"))
	FGameplayTag StatusBarTag;
};
