#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_Reshuffle.generated.h"

class UUW_Card;
class UUW_CardHand;
class UUW_Layout_Cos;
class ACardPlayer;
class ACardBase;


UCLASS()
class CROWNOFSIN_API AAction_Reshuffle : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_Reshuffle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetUpAction_Implementation() override;

	virtual void PlayAction_Implementation() override;

protected:
	UFUNCTION()
	void UpdateDrawOnReach(ACardBase* InRemovedCard, UUserWidget* InRemovedWidget);

public:
	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle|Config|Setup")
	UCurveFloat* TransferAnimSpeedCurve;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle|Config|Setup")
	UCurveFloat* TransferAnimScaleCurve;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle")
	ACardPlayer* CardPlayerReference;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle")
	UUW_Layout_Cos* PlayerUIReference;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle")
	UUW_CardHand* HandWidgetReference;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle")
	FVector2D DiscardPilePosition;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle")
	FVector2D DrawPilePosition;

	UPROPERTY(BlueprintReadWrite, Category="Action Reshuffle")
	float CardDelay = 0.07f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Reshuffle", meta=(ExposeOnSpawn="true"))
	TArray<ACardBase*> Cards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Reshuffle", meta=(ExposeOnSpawn="true"))
	TArray<UUW_Card*> CardWidgets;
};
