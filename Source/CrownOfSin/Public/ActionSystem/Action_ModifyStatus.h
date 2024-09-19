#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Core/CosEnumStruct.h"
#include "Action_ModifyStatus.generated.h"

class ATextSplash;
class UStatusComponent;
class AIconSplash;

/*true로 설정된 경우 상태 수정으로 인해 발생하는 텍스트 및 아이콘 스플래시 효과를 표시합니다. 또한 UI 상태 표시기에 업데이트를 지시합니다.*/
UCLASS()
class CROWNOFSIN_API AAction_ModifyStatus : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_ModifyStatus();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetUpAction_Implementation() override;

	/**
	 * @brief 상태를 변경하는 액션을 실행하고, 상태 아이콘과 텍스트 효과를 화면에 표시합니다.
	 * 상태 아이콘을 업데이트하고, 스플래시 아이콘 및 텍스트를 화면에 스폰합니다.
	 */	
	virtual void PlayAction_Implementation() override;

public:
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status|Config|SetUp")
	TSubclassOf<AIconSplash> BP_AIconSplashClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status|Config|SetUp")
	TSubclassOf<ATextSplash> BP_TextSplashClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	UStatusComponent* StatusReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	bool bShowSplashIcon = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	bool bShowSplashNumber = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	bool bRefreshAppearance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	bool bCanBeZero = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	int32 NewValue = 0;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	FText StatusName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	FText SplashText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	FText TextOverride;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	FLinearColor IconTint = FLinearColor(0, 0, 0, 0);

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	FLinearColor StatusColor = FLinearColor(0, 0, 0, 0);;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	FStatusAppearance StatusAppearance;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	UObject* StatusIndicator;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	AActor* StatusOwner;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	UTexture2D* IconImage;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	AActor* Puppet;
};
