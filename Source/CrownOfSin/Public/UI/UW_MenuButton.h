#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MenuButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonClicked);

UCLASS()
class CROWNOFSIN_API UUW_MenuButton : public UUserWidget
{
	GENERATED_BODY()


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Menu Button|Delegate")
	FOnMenuButtonClicked OnMenuButtonClicked;
};
