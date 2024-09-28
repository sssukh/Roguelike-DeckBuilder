#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Armor.generated.h"


UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Armor : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Armor();

protected:
	virtual void BeginPlay() override;


	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	 *	Field Members
	 =========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category = "Status Armor")
	int32 OldArmor = 0;
};
