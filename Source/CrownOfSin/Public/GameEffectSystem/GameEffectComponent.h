// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Interrupt.h"
#include "GameEffectComponent.generated.h"

class AEffectActor;
class AAction_Effect;
class ACardBase;
class UTargetingComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEffectResolved);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UGameEffectComponent : public UActorComponent , public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Game Effect Component")
	virtual void InitializeEffect();
	
	UFUNCTION(BlueprintCallable, Category = "Game Effect Component")
	virtual void ApplyEffect();
	
	UFUNCTION(BlueprintCallable, Category = "Game Effect Component")
	virtual bool TakeEffect(AActor* TargetActor);


public:
	// Interface_Interrupt
	virtual void Interrupt_Implementation();
	
	virtual void CancelInterruption_Implementation() override;

	// Member Variables
public:

	// 인터럽트가 발생했는지 여부를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	bool bInterrupt;

	// 즉시 효과가 발생하는지 여부를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	bool bImmediate = true;

	// 타겟이 필요한지 여부를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	bool bTargeted = true;

	// 기본 타겟팅 컴포넌트를 나타내는 클래스입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	TSubclassOf<UTargetingComponent> DefaultTargetingClass;

public:
	// 카드 효과의 값을 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	int32 EffectValue;

	// 타겟팅을 처리하는 컴포넌트 클래스를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	TSubclassOf<UTargetingComponent> TargetingClass;

	// 카드 효과의 타겟팅 대상에 추가할 컴포넌트를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	TSubclassOf<UActorComponent> TargetComponent;

	// 소유자 클래스(EffectActor)를 가리키는 포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	TObjectPtr<AEffectActor> ParentCard;

	// 카드에 연관된 게임플레이 태그를 저장하는 컨테이너입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	FGameplayTagContainer GameplayTags;



	// 카드 사용 시 발생할 영웅 애니메이션을 나타내는 게임플레이 태그입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	FGameplayTag HeroAnim;

	// 카드 효과가 발동될 때 실행될 액션 클래스입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	TSubclassOf<AAction_Effect> EffectActionClass;

	// 사용된 카드의 데이터를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	FDataTableRowHandle UsedData;

	// 카드의 고유 식별자입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Effect Component")
	FString Identifier;

	// Delegate
public:
	FOnEffectResolved OnEffectResolved;
};
