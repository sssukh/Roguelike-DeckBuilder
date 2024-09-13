#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_Interrupt.h"
#include "Interfaces/Interface_Utility.h"
#include "StatusComponent.generated.h"


class UDispatcherHubComponent;

class URetriggerAbleDelay;

/*체력, 중독, 유물 보유 등과 같은 상태 효과, 속성 등을 나타내기 위해 미니언 또는 CardPlayer에 추가됩니다.*/
UCLASS(Blueprintable, BlueprintType, ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatusComponent : public UActorComponent, public IInterface_Utility, public IInterface_Interrupt, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	void UpdateLoopGuard();

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	bool CheckLoopGuard();

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload);

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	int32 SubtractStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload);

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	int32 SetStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload);

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	void SetStatusMax(int32 NewMaxValue);

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	void RemoveStatus();

	UFUNCTION(BlueprintCallable, Category="Status Component Event")
	FStatusAppearance MakeAppearanceStruct();

public:
	UFUNCTION(BlueprintCallable, Category="Status Component")
	void GetAndSetMaxValues();

	UFUNCTION(BlueprintPure, Category="Status Component")
	UDispatcherHubComponent* GetOwnersDispatcherHub();


	/*========================================================================================
	*	IInterface_Utility
	=========================================================================================*/
public:
	virtual bool Interact_Implementation(const FGameplayTagContainer& Tags) override;

	/*========================================================================================
	*	IInterface_Interrupt
	=========================================================================================*/
public:
	virtual void Interrupt_Implementation() override;

	virtual void CancelInterruption_Implementation() override;

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual float GetPriority_Implementation() override;

	virtual void SetPriority_Implementation(float InPriority) override;

	virtual FText GetFriendlyName_Implementation() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bCanBeZero = false;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bInterrupt = false;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bMaxAble = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	bool bShowImmediately = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	int32 StatusValue = 0;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 IncomingStatusChange = 0;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 MaxValue = 0;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 CurrentTriggersThisTick = 0;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 MaxTriggersPerTick = 10;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	float Priority = 0;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	FText FriendlyName;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	EStatusSlot SlotType = EStatusSlot::Passive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	FGameplayTagContainer GameplayTags;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	FLinearColor Tint;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	EStatusTextAlignment TextAlignment = EStatusTextAlignment::None;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	TArray<FToolTipValue> Tooltips;

	UPROPERTY(BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	UObject* OwnerUiRef;

	UPROPERTY(BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	UObject* StatusIndicator;

private:
	UPROPERTY()
	URetriggerAbleDelay* RetriggerDelay_LoopGuard;
};
