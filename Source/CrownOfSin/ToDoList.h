// 작업상황

// 블루프린트에서 해야할 상황
//1. DT_Minions_Heros 데이터 지정해줘야함
//2. DT_Decks 데이터 지정해줘야함
//3. DT_Cards 데이터 지정해줘야함
//4. DT_Minions_Monsters 데이터 지정해줘야함

//sdsdsdsdsdsdsdsdsdsdsdsd

//데이터테이블 로우 핸들 예시
// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node Base", meta = (ExposeOnSpawn="true",RowType="/Script/CrownOfSin.MapEvent"))
// FDataTableRowHandle;

//데이터테이블 한정자 예시
// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UW Hero Select", meta = (ExposeOnSpawn="true", RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.Minion"))
// UDataTable* HeroTable;