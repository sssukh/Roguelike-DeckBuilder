#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "CosSaveGame.h"
#include "Engine/GameInstance.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "NodeSystem/NodeEnumStruct.h"
#include "CosGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UCosGameInstance : public UGameInstance, public IInterface_CardGameInstance
{
	GENERATED_BODY()

public:
	UCosGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

public:
	/**
     * 주어진 저장 슬롯 ID를 사용하여 저장된 게임을 로드하거나, 저장된 게임이 없을 경우 새로운 저장 게임 객체를 만듭니다.
     * 이 함수는 저장된 게임 데이터를 가져오거나 새로운 저장 게임을 초기화하기 위해 사용됩니다.
     *
     * @param InSaveID 로드하거나 생성할 저장 슬롯의 고유 ID입니다.
     * @return UCosSaveGame 객체 포인터를 반환합니다. 저장된 게임이 존재하면 해당 객체를 반환하고, 그렇지 않으면 새로 생성된 객체를 반환합니다.
     */
	UCosSaveGame* GetSavedGameOrMakeIfInvalid(const FString& InSaveID);

	/*모든 영구 데이터를 기본 상태로 재설정하여 게임 인스턴스 데이터를 효과적으로 지웁니다.*/
	void ClearPersistentData();

	/*레벨 로드 사이에 특정 영웅을 식별하는 데 사용할 고유 ID를 생성합니다. 입력 선행 문자열(일반적으로 미니언의 데이터 행 이름) 끝에 난수를 추가합니다.*/
	FString MakeUniqueHeroId(FString LeadingString);

	/*미니언의 모든 시작 상태를 찾고 유물이 아닌 모든 것을 반환합니다.
	 *게임 시작 시 미니언의 유물을 필터링하고 카드 플레이어(속해 있는 곳)에 추가하는 데 사용됩니다.
	 *디자이너가 선택한 영웅에 따라 시작 아티팩트를 추가할 수 있도록 하는 해결 방법
	 */
	TMap<TSubclassOf<UStatusComponent>, int32> FilterOutArtifactsFromMinion(const FMinion& InMinion);

	/**
     * 주어진 품질 수준에 따라 그래픽 품질을 설정합니다.
     * 다양한 그래픽 설정을 조정하고, 품질 수준에 따라 해상도 품질을 설정합니다.
     *
     * @param InQuality 설정할 그래픽 품질 수준 (0에서 3까지).
     */
	void SetGraphicsQuality(int32 InQuality);


	/*========================================================================================
	*	IInterface_CardGameInstance
	=========================================================================================*/
public:
	virtual bool AttemptSaveGame_Implementation(const FString& InLevelNameOverride, bool bIsUpdateDeck) override;

	/**
     * 게임 저장 데이터를 로드하고, 저장된 상태를 현재 게임 인스턴스에 복원합니다.
     * 저장된 레벨이 현재 레벨과 다를 경우, 필요에 따라 레벨을 전환할 수도 있습니다.
     * 
     * @param InSaveId 로드할 세이브 슬롯의 ID입니다.
     * @param bOpenLevelIfValid 로드한 세이브 데이터에 따라 다른 레벨을 열지 여부를 결정합니다.
     * @return 로드가 성공적으로 완료되면 true, 그렇지 않으면 false를 반환합니다.
     */
	virtual bool AttemptLoadGame_Implementation(const FString& InSaveId, bool bOpenLevelIfValid) override;

	virtual void ResetGame_Implementation() override;

	/**
     * 주어진 영웅 데이터와 덱 데이터를 사용하여 고유한 영웅을 생성하고 인스턴스에 추가합니다.
     * 이 함수는 영웅의 상태를 초기화하고, 덱에 포함된 카드들을 해당 영웅과 연관시켜 저장합니다.
     *
     * @param HeroData 영웅 데이터 테이블 행 핸들입니다.
     * @param HeroDeck 영웅 덱 데이터 테이블 행 핸들입니다.
     * @param OutUniqueID 생성된 영웅의 고유 ID를 반환합니다.
     * @return 영웅이 성공적으로 인스턴스에 추가되면 true, 그렇지 않으면 false를 반환합니다.
     */
	virtual bool AddPersistentHeroToInstance_Implementation(FDataTableRowHandle HeroData, FDataTableRowHandle HeroDeck, FString& OutUniqueID) override;

	/**
     * 주어진 고유 ID를 가진 영웅을 인스턴스의 영웅 목록에서 찾아 반환합니다.
     * 이 함수는 영웅의 고유 ID를 기반으로 영웅을 검색하고, 해당 영웅이 존재하는 경우 참조와 인덱스를 반환합니다.
     *
     * @param InUniqueID 찾고자 하는 영웅의 고유 ID입니다.
     * @param OutHero 찾은 영웅의 정보를 담을 FMinion 구조체 참조입니다.
     * @param OutArrayIndex 찾은 영웅의 인덱스를 담을 정수형 참조입니다.
     * @return 영웅을 성공적으로 찾으면 true를 반환하고, 그렇지 않으면 false를 반환합니다.
     */
	virtual bool GetHeroWithIdFromInstance_Implementation(const FString& InUniqueID, FMinion& OutHero, int32& OutArrayIndex) override;

	/**
     * 주어진 아티팩트를 인스턴스의 아티팩트 목록에 추가하거나, 기존의 동일한 유형의 아티팩트가 있는 경우 그 값을 증가시킵니다.
     * 이 함수는 아티팩트 목록을 관리하고 새로운 아티팩트를 추가하거나, 기존 아티팩트의 값을 갱신하는 기능을 수행합니다.
     *
     * @param InArtifact 추가하거나 업데이트할 아티팩트를 나타내는 FStatus 구조체입니다.
     * @return 아티팩트가 성공적으로 추가되었거나 업데이트되면 true를 반환합니다.
     */
	virtual bool AddArtifactToInstance_Implementation(const FStatusData& InArtifact) override;

	virtual FString GetCurrentNodeMapFromInstance_Implementation() override;

	virtual void SetGlobalGraphicsQuality_Implementation(int32 InQuality) override;

	virtual int32 GetGlobalGraphicsQuality_Implementation() override;

	/**
     * 게임 인스턴스의 덱을 주어진 카드 배열로 업데이트합니다.
     * 기존 덱을 비우고, 입력된 카드 배열에서 각 카드의 데이터를 덱에 추가합니다.
     *
     * @param InCards 덱을 업데이트할 카드 객체 배열입니다.
     */
	virtual void UpdateDeckInInstance_Implementation(const TArray<ACardBase*>& InCards) override;

	virtual void AddVisitedNodeToInstance_Implementation(int32 InNodeID) override;

	virtual TArray<FMinion> GetPersistentHeroesFromInstance_Implementation() override;

	virtual TArray<int32> GetVisitedMapNodesFromInstance_Implementation() override;

	virtual bool IsDebugMode_Implementation() override;
	
	virtual TArray<FStatusData> GetArtifactsFromInstance_Implementation() override;

	virtual TArray<FDataTableRowHandle> GetDoneStoryEncountersFromInstance_Implementation() override;


	virtual void SetCurrentEncounterInInstance_Implementation(FEncounter InCurrentEncoutner) override;

	virtual void AddDoneStoryEncounterToInstance_Implementation(FDataTableRowHandle StoryEncounter) override;
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Cos Game Instance")
	FName HeroUniqueId;

	UPROPERTY(BlueprintReadWrite, Category="Cos Game Instance")
	TArray<FMinion> CurrentMinions;

	UPROPERTY(BlueprintReadWrite, Category="Cos Game Instance")
	TArray<FDataTableRowHandle> StartingArtifacts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Graphic")
	int32 GlobalGraphicsQuality = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|SetUp")
	TArray<FHeroDeck> StartingHeroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Debug")
	bool bDebugMode = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	FString SaveId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	TArray<FMinion> PersistentHeroes;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	TArray<FCard> Deck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	TArray<int32> VisitedNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	TArray<FStatusData> Artifacts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	FEncounter CurrentEncounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	FString CurrentNodeMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cos Game Instance|Save & Load", meta = (RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.Card"))
	TArray<UDataTable*> RewardTables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	FGameplayTagContainer AllowedCardRewardTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Game Instance|Save & Load")
	TArray<FDataTableRowHandle> DoneStoryEncounters;
};
