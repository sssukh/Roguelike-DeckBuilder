// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Core/CosEnumStruct.h"
#include "Action_ModifyStatus.generated.h"

class UStatusComponent;

UCLASS()
class CROWNOFSIN_API AAction_ModifyStatus : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_ModifyStatus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Modify Status", meta=(ExposeOnSpawn="true"))
	UStatusComponent* Status;

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
	FLinearColor IconTint;

	UPROPERTY(BlueprintReadWrite, Category="Action Modify Status")
	FLinearColor StatusColor;

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
