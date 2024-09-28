
#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_SplashText.generated.h"

class ATextSplash;

UCLASS()
class CROWNOFSIN_API AAction_SplashText : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_SplashText();

protected:
	virtual void BeginPlay() override;


public:

	/**
 	 * 이 함수는 Splash 텍스트를 화면에 스폰하여 나타냅니다.
 	 * 주로 액터의 중심에 텍스트를 나타내고, 해당 액션이 끝난 후 실행을 완료합니다.
 	 */
	virtual  void PlayAction_Implementation() override;
	
	virtual bool AttemptDestroyAction_Implementation() override;

	
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action Splash Text|Config|Setup")
	TSubclassOf<ATextSplash> BP_TextSplashClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action Splash Text",meta=(ExposeOnSpawn="true"))
	TObjectPtr<AActor> Puppet;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action Splash Text",meta=(ExposeOnSpawn="true"))
	FText DisplayText = FText::FromString(TEXT("Debug"));
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action Splash Text",meta=(ExposeOnSpawn="true"))
	FLinearColor Color =FLinearColor::White;
};
