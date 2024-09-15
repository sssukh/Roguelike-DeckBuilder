#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "CardPlayer.generated.h"

class UStatus_Draw;
class UStatus_ManaGain;
class UStatus_Mana;
class UStatus_Initiative;
class UChanceManagerComponent;
class UPileComponent;
class UPileShopComponent;
class UPileVoidComponent;
class UPileHandComponent;
class UPileExhaustComponent;
class UPileDiscardComponent;
class UPileDeckComponent;
class UPileDrawComponent;
class UPileDestroyComponent;

class UGameplayTagComponent;
class UPayloadHolderComponent;
class UDispatcherHubLocalComponent;
class UInputMappingContext;
class UUW_Layout_Cos;

UCLASS()
class CROWNOFSIN_API ACardPlayer : public AActor, public IInterface_CardTarget, public IInterface_StoryEncounter
{
	GENERATED_BODY()

public:
	ACardPlayer();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * @brief 현재 액터에 추가된 모든 UPileComponent를 검색하고, 그들의 PileTag와 해당 컴포넌트를 PileTagLookup 맵에 추가합니다.
	 * 이 함수는 카드 파일 컴포넌트(UPileComponent)를 검색하고, 이를 태그(PileTag)를 기준으로 매핑하여 관리할 수 있도록 합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Card Player")
	void GeneratePileTagLookup();

	/**
	 * 주어진 PileTag에 대응하는 UPileComponent를 찾고, 해당 컴포넌트를 OutPileComponent로 반환합니다.
	 * @param PileTag - 찾고자 하는 태그
	 * @param OutPileComponent - 해당 태그에 매칭되는 PileComponent 포인터
	 * @return 태그에 매칭되는 컴포넌트가 있으면 true, 없으면 false
	 */
	UFUNCTION(BlueprintCallable, Category="Card Player")
	bool GetPileWithPileTag(FGameplayTag PileTag, UPileComponent*& OutPileComponent);

protected:
	/*상태 클래스가 UStatusComponent의 하위 클래스인지 확인하는 함수*/
	bool IsValidStatusClass(TSubclassOf<UStatusComponent> InStatusClass);

	/*새로운 상태 컴포넌트를 생성하는 함수*/
	UStatusComponent* CreateNewStatusComponent(TSubclassOf<UStatusComponent> InStatusClass);


public:
	UFUNCTION(BlueprintCallable, Category = "Card Player | Debug")
	void DisplayScreenLogMessage(FText Message, FColor Color);

	
	/*========================================================================================
	*	Iinterface_CardTarget
	=========================================================================================*/
public:
	virtual int32 AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad) override;


	/*========================================================================================
	*	Iinterface_CardTarget
	=========================================================================================*/
public:
	virtual void InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Config|Setup")
	UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Config|Setup")
	TSubclassOf<UUW_Layout_Cos> WBP_LayoutClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UChanceManagerComponent* ChanceManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UStatus_Initiative* Status_Initiative;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UStatus_Mana* Status_Mana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UStatus_ManaGain* Status_ManaGain;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UStatus_Draw* Status_Draw;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileDestroyComponent* PileDestroyComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileShopComponent* PileShopComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileDrawComponent* PileDrawComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileDeckComponent* PileDeckComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileDiscardComponent* PileDiscardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileExhaustComponent* PileExhaustComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileHandComponent* PileHandComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPileVoidComponent* PileVoidComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UGameplayTagComponent* GameplayTagComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UDispatcherHubLocalComponent* DispatcherHubLocalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UPayloadHolderComponent* PayloadHolderComponent;



public:
	UPROPERTY(BlueprintReadWrite, Category="Card Player")
	TObjectPtr<UUW_Layout_Cos> PlayerUI;

	UPROPERTY(BlueprintReadWrite, Category="Card Player")
	TMap<FGameplayTag, UPileComponent*> PileTagLookup;
};
