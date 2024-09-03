#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ToolTip.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ToolTip(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Tool Tip Event")
	void UpdateToolTip(int32 InValue);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip|Designer", meta=(BindWidget))
	URichTextBlock* RichTooltipText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta=(ExposeOnSpawn="true"))
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta=(ExposeOnSpawn="true"))
	FText TextOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta=(ExposeOnSpawn="true"))
	FDataTableRowHandle ToolTipData;
};
