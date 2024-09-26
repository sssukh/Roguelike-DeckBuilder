#pragma once

#include "CoreMinimal.h"
#include "UW_ToolTip.h"
#include "Blueprint/UserWidget.h"
#include "UW_ToolTipList.generated.h"

class UVerticalBox;

/**
 * UUW_ToolTipList는 여러 개의 툴팁을 관리하는 위젯 클래스입니다.
 * UVerticalBox를 통해 툴팁을 리스트 형태로 추가하고, 툴팁 데이터를 기반으로 개별 툴팁을 동적으로 생성합니다.
 * 이 클래스는 주로 툴팁 시스템을 관리하고 표시하는 데 사용됩니다.
 */
UCLASS()
class CROWNOFSIN_API UUW_ToolTipList : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ToolTipList(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="UW Tool Tip List")
	void AddToolTipFromData(const FDataTableRowHandle& ToolTipData, int32 Value);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="UW Tool Tip List", meta=(BindWidget))
	UVerticalBox* ToolTipBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip List|Config|Setup")
	TSubclassOf<UUW_ToolTip> WBP_ToolTipClass;
};
