
#include "CoreMinimal.h"
#incldue "Engine/DataTable.h"

USTRUCT(BlueprintEdit,BlueprintReadWrite)
struct FMapEvent : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY()
    TObjectPtr<UActorComponent> MapEventClass;
    
    UPROPERTY()
    TObjectPtr<UTexture2D> Icon;
    
    UPROPERTY()
    FDataTableRowHandle SpecificEvent;
    
    UPROPERTY()
    TArray<UDataTable> EncounterTables;
    
    UPROPERTY()
    TMap<FGameplayTag,double> TagWeigthts;
    
    UPROPERTY()
    FGameplayTag RequiredTags;
}

