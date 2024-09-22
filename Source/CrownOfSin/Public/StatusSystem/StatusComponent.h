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

/* 
 * UStatusComponent는 체력, 중독, 유물 보유 등 다양한 상태 효과를 관리하는 컴포넌트입니다.
 * 이 컴포넌트는 미니언이나 CardPlayer와 같은 캐릭터에 추가되어 상태 값을 변경, 상태 관련 시각적 효과 처리, 최대 값 설정 및 이벤트 트리거 등의 기능을 수행합니다.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatusComponent : public UActorComponent, public IInterface_Utility, public IInterface_Interrupt, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UStatusComponent();

protected:
	virtual void BeginPlay() override;

	/*========================================================================================
	*	상태 값 변경 관련 함수
	=========================================================================================*/
public:
	/**
	 * 상태 값(StatusValue)을 InAmount만큼 증가시키고, 시각적 효과를 처리하며, 상태가 0이면 컴포넌트를 제거합니다.
	 * 상태 변화 전/후에 이벤트를 호출하고, 필요 시 스플래시 텍스트와 아이콘을 화면에 표시합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Status Component|Status Management")
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload);

	/* 상태 값(StatusValue)을 InAmount만큼 감소시키고, 시각적 효과를 처리합니다. */
	UFUNCTION(BlueprintCallable, Category="Status Component|Status Management")
	int32 SubtractStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload);

	/**
	 * 상태 값을 InAmount로 설정하며, 현재 상태 값과 차이를 계산하여 그 차이만큼 상태 값을 변경합니다.
	 * 시각적 효과가 필요할 경우 이를 처리합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Status Component|Status Management")
	int32 SetStatusValue(int32 InAmount, bool bShowSplashNumber=false, bool bShowSplashIcon=false, UObject* Payload=nullptr);

	/**
	 * 상태의 최대 값(MaxValue)을 설정하고, 현재 상태 값(StatusValue)을 새로운 최대 값에 맞춰 조정합니다.
	 * 상태 최대 값 변경 후 이벤트를 전파합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Status Component|Status Management")
	void SetStatusMax(int32 NewMaxValue);

	/* 상태 값을 0으로 설정하고, 상태 값이 0이어도 유지되지 않도록 설정(bCanBeZero = false)합니다. */
	UFUNCTION(BlueprintCallable, Category="Status Component|Status Management")
	void RemoveStatus();

	/* 현재 상태 컴포넌트의 외관 데이터를 FStatusAppearance 구조체로 생성하여 반환합니다. */
	UFUNCTION(BlueprintCallable, Category="Status Component|Status Management")
	FStatusAppearance MakeAppearanceStruct();


	/*========================================================================================
	*	디스패처 관련 함수
	=========================================================================================*/
public:
	/* 현재 상태의 최대 값을 가져오고, 유효하지 않은 경우 현재 상태 값을 최대 값으로 설정합니다. */
	UFUNCTION(BlueprintCallable, Category="Status Component|Dispatcher")
	void GetAndSetMaxValues();

	/**
	 * 소유한 액터에서 UDispatcherHubComponent를 찾아 반환합니다.
	 * 성공 시 OutDispatcherHubComponent에 값을 할당하고 true를 반환, 실패 시 false를 반환합니다.
	 */
	UFUNCTION(BlueprintPure, Category="Status Component|Dispatcher")
	bool GetOwnersDispatcherHub(UDispatcherHubComponent*& OutDispatcherHubComponent);

protected:
	/*========================================================================================
	*	상태 값 변경 관련 로직 및 시각적 효과 처리
	=========================================================================================*/
	/* 상태 값 변경과 관련된 로직을 처리하고, 시각적 효과를 스폰합니다. */
	void ApplyStatusChange(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance);

	/* 상태 변화에 따라 시각적 효과(아이콘 및 텍스트)를 스폰하여 화면에 표시합니다. */
	void SpawnModifyStatusAction(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance);

	/* 상태 변화에 따라 스플래시 텍스트를 생성하여 반환합니다. */
	FText GenerateSplashText(int32 InAmount, bool bShowSplashNumber);

public:
	/*========================================================================================
	*	루프 가드 관련 함수
	=========================================================================================*/
	/**
	 * 루프 가드를 업데이트하고, 트리거 수가 최대 트리거 수 이하인지 여부를 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Status Component|Loop Guard")
	bool CheckLoopGuard();

	/**
	 * 호출될 때마다 트리거 카운트를 증가시키고, 최대 트리거 수를 초과하면 경고를 출력합니다.
	 * 루프 가드를 재설정하여 매 틱마다 트리거 수를 초기화합니다.
	 */
	UFUNCTION()
	void UpdateLoopGuard();

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
	/** 상태 값이 0일 때도 상태가 유지될 수 있는지 여부를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bCanBeZero = false;

	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bArtifact = false;
	
	/** 상태가 인터럽트되었는지 여부를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bInterrupt = false;

	/** 최대 값이 설정 가능한 상태인지 여부를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bMaxAble = false;

	/** 상태가 생성될 때 즉시 UI에 표시할지 여부를 나타냅니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	bool bShowImmediately = false;

	/** 현재 상태 값을 나타냅니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	int32 StatusValue = 0;

	/** 상태 값의 변화량을 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 IncomingStatusChange = 0;

	/** 상태의 최대 값을 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 MaxValue = 0;

	/** 현재 틱에서 발생한 트리거 수를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 CurrentTriggersThisTick = 0;

	/** 한 틱당 최대 트리거 수를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	int32 MaxTriggersPerTick = 10;

	/** 상태의 우선순위를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	float Priority = 0;

	/** 사용자에게 표시될 상태의 이름입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	FText FriendlyName;

	/** 상태가 속한 슬롯 유형을 나타냅니다 (예: Passive, Active). */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	EStatusSlot SlotType = EStatusSlot::Passive;

	/** 상태와 연관된 게임플레이 태그들을 나타냅니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	FGameplayTagContainer GameplayTags;

	/** 상태를 나타내는 아이콘을 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	UTexture2D* Icon;

	/** 아이콘에 적용할 색상을 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	FLinearColor Tint;

	/** 상태 텍스트의 정렬 방식을 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	EStatusTextAlignment TextAlignment = EStatusTextAlignment::None;

	/** 상태에 대한 툴팁 정보를 저장하는 배열입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	TArray<FToolTipValue> Tooltips;

	/** UI에서 상태를 표시하는 객체의 참조입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	UObject* OwnerUiRef;

	/** 상태 인디케이터 객체의 참조입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	UObject* StatusIndicator;
	

	/** RetriggerAbleDelay 인스턴스입니다. 루프 가드를 위한 딜레이 처리를 담당합니다. */
private:
	UPROPERTY()
	URetriggerAbleDelay* RetriggerDelay_LoopGuard;
};
