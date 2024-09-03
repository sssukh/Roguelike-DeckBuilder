#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_TargetArrow.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_TargetArrow : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_TargetArrow
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Target Arrow")
	void FocusTargetArrow();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Target Arrow")
	void UpdateTargetArrow(bool bIsVisibility, FLinearColor InTint, bool bIsUpdateSourcePoint, FVector2D InSourcePoint, bool bIsAnimateFocus);
};
