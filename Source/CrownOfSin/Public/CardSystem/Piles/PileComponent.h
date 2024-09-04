#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Pile.h"
#include "PileComponent.generated.h"


class ACardBase;


/*Parent class for all card piles, including the deck and card hand. Holds references to all cards in the pile and methods for adding and removing cards from the pile*/
UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileComponent : public UActorComponent ,public IInterface_Pile
{
	GENERATED_BODY()

public:
	UPileComponent();

protected:
	virtual void BeginPlay() override;

public:

	
	/*========================================================================================
	*	FiledMembers
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	FGameplayTag PileTag;

	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	TArray<ACardBase*> Cards;
};
