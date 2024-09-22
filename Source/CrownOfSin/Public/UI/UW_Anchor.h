#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Anchor.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_Anchor : public UUserWidget
{
	GENERATED_BODY()


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Anchor", meta=(BindWidget))
	UImage* Image;
};
