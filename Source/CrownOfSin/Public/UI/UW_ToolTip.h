#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ToolTip.generated.h"

class URichTextBlock;


/**
 * UUW_ToolTip 클래스는 게임 내에서 특정 오브젝트나 기능에 대한 툴팁을 표시하는 UI 위젯입니다.
 * 이 클래스는 동적으로 툴팁 데이터를 받아와 이를 화면에 출력하는 역할을 하며, 
 * 게임 아이템, 스킬, 캐릭터 상태 등 다양한 요소에 대한 정보를 직관적으로 제공하기 위해 사용됩니다.
 * 
 * 툴팁 내용은 DataTable에서 로드된 데이터를 기반으로 설정되며, Value 값에 따라 툴팁 텍스트가 변형됩니다.
 * 또한, 필요에 따라 TextOverride를 통해 기본 툴팁 대신 사용자 정의 텍스트를 사용할 수 있습니다.
 * 
 * 이 클래스는 주로 UI 디자이너와 프로그래머가 협력하여 사용자 인터페이스 상에서 유용한 정보를 플레이어에게 제공하기 위한 용도로 설계되었습니다.
 * 다양한 게임 요소에 대해 동적이고 직관적인 설명을 제공하는 데 적합한 구조로 되어 있습니다.
 */
UCLASS()
class CROWNOFSIN_API UUW_ToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ToolTip(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	/*
	 * Tooltip을 업데이트하는 함수.
	 * 인자로 받은 값(InValue)에 따라 툴팁 데이터를 업데이트하고 텍스트를 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Tool Tip Event")
	void UpdateToolTip(int32 InValue);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="UW Tool Tip|Designer", meta=(BindWidget))
	URichTextBlock* RichTooltipText;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta=(ExposeOnSpawn="true"))
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta=(ExposeOnSpawn="true"))
	FText TextOverride;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta=(ExposeOnSpawn="true"))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Tool Tip", meta = (RowType="/Script/CrownOfSin.ToolTip", ExposeOnSpawn="true"))
	FDataTableRowHandle ToolTipData;
};
