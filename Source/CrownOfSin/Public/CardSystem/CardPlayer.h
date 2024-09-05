#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "CardPlayer.generated.h"

class UPileDestroyComponent;
class UDispatcherHubLocalComponent;
class UPileComponent;
class UInputMappingContext;
class UChanceManagerComponent;
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
	UFUNCTION(BlueprintCallable, Category = "Card Player | Debug")
	void DisplayScreenLogMessage(FText Message, FColor Color);


public:
	/**
	 * @brief 현재 액터에 추가된 모든 UPileComponent를 검색하고, 그들의 PileTag와 해당 컴포넌트를 PileTagLookup 맵에 추가합니다.
	 * 
	 * 이 함수는 카드 파일 컴포넌트(UPileComponent)를 검색하고, 이를 태그(PileTag)를 기준으로 매핑하여 관리할 수 있도록 합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Card Player")
	void GeneratePileTagLookup();




protected:
	// 상태 클래스가 UStatusComponent의 하위 클래스인지 확인하는 함수
	bool IsValidStatusClass(TSubclassOf<UStatusComponent> InStatusClass);
	
	// 새로운 상태 컴포넌트를 생성하는 함수
	UStatusComponent* CreateNewStatusComponent(TSubclassOf<UStatusComponent> InStatusClass);


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
	UPileDestroyComponent* PileDestroyComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UDispatcherHubLocalComponent* DispatcherHubLocalComponent;



public:
	UPROPERTY(BlueprintReadWrite, Category="Card Player")
	TObjectPtr<UUW_Layout_Cos> PlayerUI;

	UPROPERTY(BlueprintReadWrite, Category="Card Player")
	TMap<FGameplayTag, UPileComponent*> PileTagLookup;
};
