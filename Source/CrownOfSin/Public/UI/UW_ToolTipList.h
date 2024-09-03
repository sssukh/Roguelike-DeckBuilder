#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ToolTipList.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ToolTipList : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ToolTipList(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="UW Tool Tip Liset Event")
	void AddToolTipFromData(const FDataTableRowHandle& ToolTipData, int32 Value);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip List", meta=(BindWidget))
	UVerticalBox* ToolTipBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip List|Config|Setup")
	TSubclassOf<UUW_ToolTipList> WBP_ToolTipClass;
};
