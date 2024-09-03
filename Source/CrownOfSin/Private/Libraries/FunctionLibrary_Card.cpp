

#include "Libraries/FunctionLibrary_Card.h"

FCard UFunctionLibrary_Card::MakeCardStructFromCardData(FDataTableRowHandle CardData)
{
	FCard* Card = CardData.DataTable->FindRow<FCard>(CardData.RowName,TEXT("FCard in FunctionLibrary_Card"));

	if(!Card)
	{
		GEngine->AddOnScreenDebugMessage(-1,9.0f,FColor::Yellow,
			FString::Printf(TEXT("WARNING: Invalid Card Data passed to library function MakeCardStructFromData")));
		return FCard();
	}

	Card->DataRow = CardData;
	
	return *Card;
}
