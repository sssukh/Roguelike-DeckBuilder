#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileDeckComponent.generated.h"

/**
 * UPileDeckComponent 클래스는 카드 게임에서 덱(Deck)을 관리하는 역할을 하는 컴포넌트입니다.
 * 이 클래스는 덱의 초기화, 카드 생성, 그리고 덱을 다른 카드 더미(예: 드로우 더미)로 이동시키는 등의 다양한 기능을 제공합니다.
 * 게임 시작 시 덱 데이터를 설정하고, 각 카드의 소유자를 지정하는 등의 로직을 처리하며, 덱 설정 완료 시 관련 이벤트를 전역 이벤트 디스패처에 전달합니다.
 * 
 * 주요 기능:
 * - 게임 인스턴스에서 덱 데이터를 가져오거나, 데이터 테이블에서 덱을 로드합니다.
 * - 카드를 덱에 생성하고, 카드의 소유자 정보를 설정합니다.
 * - 드로우 더미와 같은 다른 카드 더미로 카드를 이동시키는 기능을 제공합니다.
 * 
 * 이 클래스는 게임 시작 시 덱을 초기화하는 중요한 역할을 하며, 게임 내 카드의 흐름을 제어합니다.
 */

UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileDeckComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileDeckComponent();

protected:
	virtual void BeginPlay() override;

	/**
 	 * SetupCardsInDrawPile 함수는 덱에 있는 카드를 드로우 더미(PileDrawComponent)로 이동시키고,각 카드의 소유자(Minion)를 설정하는 작업을 수행합니다.
 	 * 이 함수는 덱이 설정된 후 각 카드를 올바른 소유자에게 할당하는 기능을 담당합니다.
 	 * 덱 설정이 완료되면 전역 이벤트 디스패처에서 덱 설정 완료 이벤트를 호출합니다.
 	 */
	void SetupCardsInDrawPile();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pile Deck Component")
	FDataTableRowHandle DeckData;
};
