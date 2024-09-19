#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextSplash.generated.h"

UCLASS()
class CROWNOFSIN_API ATextSplash : public AActor
{
	GENERATED_BODY()

public:
	ATextSplash();

protected:
	virtual void BeginPlay() override;

public:
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Text Splash", meta=(ExposeOnSpawn="true"))
	FText DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Text Splash", meta=(ExposeOnSpawn="true"))
	FColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Text Splash|Component")
	USceneComponent* DefaultSceneRoot;
};
