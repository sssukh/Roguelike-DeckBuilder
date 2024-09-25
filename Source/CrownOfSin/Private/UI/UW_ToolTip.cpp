#include "UI/UW_ToolTip.h"

#include "Components/RichTextBlock.h"
#include "Core/CosEnumStruct.h"
#include "Kismet/KismetStringLibrary.h"

UUW_ToolTip::UUW_ToolTip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), RichTooltipText(nullptr), Value(0)
{
}

void UUW_ToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateToolTip(Value);
}

void UUW_ToolTip::UpdateToolTip(int32 InValue)
{
	// 만약 TextOverride가 비어있지 않다면, 이를 우선적으로 사용합니다.
	if (!TextOverride.IsEmpty())
	{
		RichTooltipText->SetText(TextOverride);
		return;
	}

	// ToolTipData가 유효하지 않으면 함수 종료.
	if (ToolTipData.IsNull()) return;

	// DataTable에서 해당하는 ToolTip 데이터를 찾습니다.
	FToolTip* FoundToolTip = ToolTipData.DataTable->FindRow<FToolTip>(ToolTipData.RowName, TEXT(""));
	if (!FoundToolTip) return;

	// 찾은 데이터를 로컬 변수로 저장합니다.
	const FToolTip& ToolTip = *FoundToolTip;

	// Value 값에 따라 적절한 텍스트를 선택합니다.
	FText DisplayText = (Value == 0 || ToolTip.Valued.IsEmpty()) ? ToolTip.UnValued : ToolTip.Valued;

	// 툴팁의 타이틀과 내용 텍스트를 결합하여 최종 문자열을 만듭니다.
	FString TitleText = FString::Printf(TEXT("<title> %s </>"), *ToolTip.Title.ToString());
	FString NewLine = DisplayText.IsEmpty() ? FString(TEXT(" ")) : FString(TEXT("\n\n"));
	FString CombinedText = FString::Printf(TEXT("%s%s%s"), *TitleText, *NewLine, *DisplayText.ToString());

	// Value 값을 텍스트에 반영합니다.
	FString From = FString(TEXT("*0"));
	FString ValueString = UKismetStringLibrary::Conv_IntToString(Value);
	FString To = FString::Printf(TEXT("<b> %s </>"), *ValueString);

	// 문자열을 대체하여 새로운 텍스트로 설정합니다.
	FText FinalText = FText::AsCultureInvariant(UKismetStringLibrary::Replace(CombinedText, From, To, ESearchCase::CaseSensitive));
	RichTooltipText->SetText(FinalText);
}
