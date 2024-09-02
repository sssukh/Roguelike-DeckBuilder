
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardPlayer.generated.h"

class UChanceManagerComponent;
class UUW_Layout_Cos;

UCLASS()
class CROWNOFSIN_API ACardPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Card Player | Debug")
	void DisplayScreenLogMessage(FText Message, FColor Color);
	
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY()
	TObjectPtr<UUW_Layout_Cos> PlayerUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Player|Component")
	UChanceManagerComponent* ChanceManagerComponent;
};
