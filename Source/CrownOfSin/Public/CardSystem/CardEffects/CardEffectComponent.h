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
	UFUNCTION(BlueprintCallable, Category="Card Effect Event")
	virtual void InitializeCardEffect();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	bool bInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	bool bImmediate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	bool bTargeted = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<UTargetingComponent> DefaultTargetingClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	int32 EffectValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<UActorComponent> TargetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TObjectPtr<ACardBase> ParentCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<UTargetingComponent> TargetingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FGameplayTag HeroAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	TSubclassOf<AAction_Effect> EffectAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FDataTableRowHandle UsedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect | Variable")
	FString Identifier;


	/*========================================================================================
	*	Delegate
	=========================================================================================*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Card Effect | Delegate")
	FOnCardResolved OnCardResolved;
};
