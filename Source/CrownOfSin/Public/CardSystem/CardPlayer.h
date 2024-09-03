#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardTarget.h"
#include "CardPlayer.generated.h"

class UDispatcherHubLocalComponent;
class UPileComponent;
class UInputMappingContext;
class UChanceManagerComponent;
class UUW_Layout_Cos;

UCLASS()
class CROWNOFSIN_API ACardPlayer : public AActor,public IInterface_CardTarget
{
	GENERATED_BODY()

public:
	ACardPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Card Player | Debug")
	void DisplayScreenLogMessage(FText Message, FColor Color);
	
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:

	void GeneratePileTagLookup();

	/*========================================================================================
	*	Iinterface_CardTarget
	=========================================================================================*/
public:
	virtual int32 AddToStatus_Implementation(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad) override;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Config|Setup")
	UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Config|Setup")
	TSubclassOf<UUW_Layout_Cos> WBP_LayoutClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UChanceManagerComponent* ChanceManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UDispatcherHubLocalComponent* DispatcherHubLocalComponent;


	UPROPERTY(BlueprintReadWrite, Category="Card Player")
	TObjectPtr<UUW_Layout_Cos> PlayerUI;

	UPROPERTY(BlueprintReadWrite, Category="Card Player")
	TMap<FGameplayTag, UPileComponent*> PileTagLookup;
};
