#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "Interfaces/Interface_Interrupt.h"
#include "CardEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardResolved);

class AAction_Effect;
class UTargetingComponent;
class ACardBase;

/*카드의 다양한 효과를 처리하는 기본 클래스입니다.
 *카드를 사용하면 카드의 타겟팅 모드에 의해 입력된 대상에 대해 순서대로 모든 카드 효과를 해결하려고 시도합니다.
 *카드 효과는 DT_Cards를 통해 카드에 추가됩니다.*/
UCLASS(ClassGroup=("Cos|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffectComponent : public UActorComponent, public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	UCardEffectComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 카드 효과를 초기화하는 함수입니다.
	UFUNCTION(BlueprintCallable, Category="Card Effect Event")
	virtual void InitializeCardEffect();

	/*
	 * 입력된 타겟에 대해 카드 효과를 해결하려고 시도하는 함수입니다.
	 * @param TargetActor 카드 효과의 대상이 되는 액터
	 * @return 카드 효과 해결 여부를 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Card Effect Event")
	virtual bool ResolveCardEffect(AActor* TargetActor);


	/*========================================================================================
	*	IInterface_Interrupt
	=========================================================================================*/
public:
	virtual void Interrupt_Implementation() override;

	virtual void CancelInterruption_Implementation() override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	// 인터럽트가 발생했는지 여부를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	bool bInterrupt;

	// 즉시 효과가 발생하는지 여부를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	bool bImmediate = true;

	// 타겟이 필요한지 여부를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	bool bTargeted = true;

	// 기본 타겟팅 컴포넌트를 나타내는 클래스입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UTargetingComponent> DefaultTargetingClass;

public:
	// 카드 효과의 값을 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	int32 EffectValue;

	// 타겟팅을 처리하는 컴포넌트 클래스를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<UTargetingComponent> TargetingClass;

	// 카드 효과의 타겟팅 대상에 추가할 컴포넌트를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<UActorComponent> TargetComponent;

	// 카드의 부모 클래스를 가리키는 포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TObjectPtr<ACardBase> ParentCard;

	// 카드에 연관된 게임플레이 태그를 저장하는 컨테이너입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FGameplayTagContainer GameplayTags;



	// 카드 사용 시 발생할 영웅 애니메이션을 나타내는 게임플레이 태그입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FGameplayTag HeroAnim;

	// 카드 효과가 발동될 때 실행될 액션 클래스입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<AAction_Effect> EffectActionClass;

	// 사용된 카드의 데이터를 나타내는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FDataTableRowHandle UsedData;

	// 카드의 고유 식별자입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FString Identifier;


	/*========================================================================================
	*	Delegate
	=========================================================================================*/
public:
	// 카드 효과가 해결되었을 때 발생하는 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Card Effect | Delegate")
	FOnCardResolved OnCardResolved;
};
