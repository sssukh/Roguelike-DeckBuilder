#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "UW_HeroAddBox.generated.h"

class UUW_HeroSelect;
class UScrollBox;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_HeroAddBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UW HeroAddBox Event")
	TArray<FHeroDeck> GetSelectedHeroesAndDecks();

	UFUNCTION(BlueprintPure, Category="UW HeroAddBox Event")
	bool GetAddButtonIsEnabled();

	UFUNCTION(BlueprintPure, Category="UW HeroAddBox Event")
	bool GetSubtractButtonIsEnabled();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Hero Add Box|Designer", meta=(BindWidget))
	UScrollBox* HeroesBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Hero Add Box|Designer", meta=(BindWidget))
	UUW_HeroSelect* WBP_HeroSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Hero Add Box", meta=(ExposeOnSpawn="true"))
	int32 MinHeroes = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Hero Add Box", meta=(ExposeOnSpawn="true"))
	int32 MaxHeroes = 1;
};
