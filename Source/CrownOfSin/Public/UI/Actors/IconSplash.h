#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IconSplash.generated.h"

/*미니언에 상태가 추가되면 플레이어에게 명확하게 표시됩니다.*/
UCLASS()
class CROWNOFSIN_API AIconSplash : public AActor
{
	GENERATED_BODY()

public:
	AIconSplash();

protected:
	virtual void BeginPlay() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Text Splash|Component")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon Splash", meta=(ExposeOnSpawn="true"))
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon Splash", meta=(ExposeOnSpawn="true"))
	FLinearColor Color;
};
