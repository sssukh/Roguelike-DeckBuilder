#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CosEnumStruct.generated.h"


class UUseRuleComponent;
class UNiagaraSystem;
class AAction_Effect;
class UTargetingComponent;
class UCardEffectComponent;
class UAttackPatternComponent;
class UStatusComponent;

UENUM(BlueprintType)
enum class ECallGlobal : uint8
{
	OnlyLocal,
	CallBefore,
	CallAfter
};

UENUM(BlueprintType)

enum class ECardDataType : uint8
{
	Hand UMETA(Description = "Modify until it leaves the hand. 손에서 떠날 때 까지 변형된다."),
	Deck UMETA(Description = "Modifies between level loads. 레벨 사이에서 변형된다."),
	Base UMETA(Description = "Used to get the non-modified version. 변형되지 않은 원본을 받을 때 사용"),
	Pile UMETA(Description = "Modify until combat ends. 전투가 종료할 때까지 변형된다.")
};

UENUM(BlueprintType)
enum class EStatusSlot : uint8
{
	None,
	Active,
	Passive,
	Bar,
	OverlapBar,
	Artifact,
	Currency
};

UENUM(BlueprintType)
enum class EStatusTextAlignment : uint8
{
	None,
	BottomRight,
	Center,
	BottomLeft
};


UENUM(BlueprintType)
enum class ECardState : uint8
{
	Passive,
	InHand,
	MouseOver,
	FollowMouse,
	Targeting,
	Activating,
	FollowBelzier,
};

USTRUCT(BlueprintType)
struct FObjectArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Object Array")
	TArray<UObject*> Objects;

	// 기본 생성자
	FObjectArray()
	{
	}

	// 복사 생성자
	FObjectArray(const FObjectArray& Other)
	{
		Objects = Other.Objects;
	}

	// 이동 생성자
	FObjectArray(FObjectArray&& Other) noexcept
	{
		Objects = MoveTemp(Other.Objects);
	}

	FObjectArray(const TArray<UObject*>& InObjects)
	{
		Objects = InObjects;
	}

	// 복사 대입 연산자
	FObjectArray& operator=(const FObjectArray& Other)
	{
		if (this != &Other)
		{
			Objects = Other.Objects;
		}
		return *this;
	}

	// 이동 대입 연산자
	FObjectArray& operator=(FObjectArray&& Other) noexcept
	{
		if (this != &Other)
		{
			Objects = MoveTemp(Other.Objects);
		}
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FMinion : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	FString UniqueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	TSubclassOf<AActor> Puppet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	UTexture* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	TArray<UMaterial*> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	FVector2D UIScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	FTransform TransformOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	TSubclassOf<UAttackPatternComponent> AttackPatternComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion", meta = (RowType="/Script/CrownOfSin.CardPattern"))
	FDataTableRowHandle AttackPatternData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	TMap<TSubclassOf<UStatusComponent>, int32> StartingStatuses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion")
	TMap<TSubclassOf<UStatusComponent>, int32> StatusLimits;

	// 기본 생성자
	FMinion() : UniqueID(TEXT("")), Puppet(nullptr)
	            , SkeletalMesh(nullptr), AnimationBlueprint(nullptr), Texture(nullptr), UIScale(FVector2D(200, 500))
	            , TransformOffset(FTransform::Identity)
	{
	}

	// 복사 생성자
	FMinion(const FMinion& Other)
	{
		*this = Other; // 대입 연산자 호출을 통한 복사
	}

	// 대입 연산자
	FMinion& operator=(const FMinion& Other)
	{
		if (this != &Other)
		{
			UniqueID = Other.UniqueID;
			Name = Other.Name;
			SkeletalMesh = Other.SkeletalMesh;
			Materials = Other.Materials;
			Texture = Other.Texture;
			Puppet = Other.Puppet;
			AnimationBlueprint = Other.AnimationBlueprint;
			GameplayTags = Other.GameplayTags;
			UIScale = Other.UIScale;
			TransformOffset = Other.TransformOffset;
			AttackPatternData = Other.AttackPatternData;
			AttackPatternComponent = Other.AttackPatternComponent;
			StartingStatuses = Other.StartingStatuses;
			StatusLimits = Other.StatusLimits;
		}
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FToolTip : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Tip")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Tip")
	FText UnValued;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Tip")
	FText Valued;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Tip")
	FText Flavor;
};

USTRUCT(BlueprintType)
struct FToolTipValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Tip Value")
	FDataTableRowHandle ToolTipTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Tip Value")
	bool bValued;

	FToolTipValue(): bValued(false)
	{
	};

	FToolTipValue(const FDataTableRowHandle& NewToolTipTable, bool NewValued): ToolTipTable(NewToolTipTable), bValued(NewValued)
	{
	};
};

USTRUCT(BlueprintType, Blueprintable)
struct FStatusData : public FTableRowBase
{
	GENERATED_BODY()

	// 상태의 값을 나타내는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Value;

	// 상태와 관련된 게임플레이 태그를 포함하는 컨테이너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FGameplayTagContainer GameplayTags;

	// 상태 클래스를 나타내는 TSubclassOf 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	TSubclassOf<UStatusComponent> StatusClass;

	// 기본 생성자
	FStatusData() : Value(1), StatusClass(nullptr)
	{
	}


	FStatusData(const TSubclassOf<UStatusComponent>& InStatusClass, int32 InValue, const FGameplayTagContainer& InGameplayTags)
		: Value(InValue), GameplayTags(InGameplayTags), StatusClass(InStatusClass)
	{
	}

	FStatusData(const FStatusData& Other)
	{
		*this = Other; // 대입 연산자를 사용하여 복사
	}

	FStatusData& operator=(const FStatusData& Other)
	{
		if (this != &Other) // 자기 자신과의 대입 방지
		{
			Value = Other.Value;
			GameplayTags = Other.GameplayTags;
			StatusClass = Other.StatusClass;
		}
		return *this;
	}

	// == 연산자 오버로딩 (동등성 비교)
	bool operator==(const FStatusData& Other) const;
};


USTRUCT(BlueprintType)
struct FStatusAppearance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	int32 StatusValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	FLinearColor Tint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	EStatusSlot SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	EStatusTextAlignment TextAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	TArray<FToolTipValue> Tooltips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Appearance")
	FText FriendlyName;

	// 기본 생성자
	FStatusAppearance()
		: StatusValue(0)
		  , Icon(nullptr)
		  , Tint(FLinearColor::White)
		  , SlotType(EStatusSlot::None) // 기본값 설정, 필요시 적절한 기본값 사용
		  , TextAlignment(EStatusTextAlignment::None) // 기본값 설정, 필요시 적절한 기본값 사용
		  , FriendlyName(FText::FromString(TEXT("Default")))
	{
	}

	// 모든 멤버 변수를 인수로 받는 생성자
	FStatusAppearance(int32 InStatusValue, UTexture2D* InIcon, FLinearColor InTint, EStatusSlot InSlotType,
	                  EStatusTextAlignment InTextAlignment, const TArray<FToolTipValue>& InTooltips, FText InFriendlyName)
		: StatusValue(InStatusValue)
		  , Icon(InIcon)
		  , Tint(InTint)
		  , SlotType(InSlotType)
		  , TextAlignment(InTextAlignment)
		  , Tooltips(InTooltips)
		  , FriendlyName(InFriendlyName)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardEffect : public FTableRowBase
{
	GENERATED_BODY()

	// 카드 효과 클래스를 나타내는 서브클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UCardEffectComponent> EffectClass;

	// 효과의 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	int32 EffectValue;

	// 액션 효과를 나타내는 클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<AAction_Effect> EffectAction;

	// 히어로 애니메이션을 나타내는 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FGameplayTag HeroAnim;

	// 관련된 게임플레이 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FGameplayTagContainer GameplayTags;

	// 사용할 파티클 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	UNiagaraSystem* Particle;

	// 타겟 컴포넌트를 나타내는 클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UActorComponent> TargetComponent;

	// 타겟팅 컴포넌트를 나타내는 클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UTargetingComponent> Targeting;

	// 사용된 데이터 테이블 행 핸들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FDataTableRowHandle UsedData;

	// 고유 식별자
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FString Identifier;

	// 기본 생성자
	FCardEffect()
		: EffectClass(nullptr)
		  , EffectValue(0)
		  , EffectAction(nullptr)
		  , Particle(nullptr)
		  , TargetComponent(nullptr)
		  , Targeting(nullptr)
		  , Identifier(TEXT(""))
	{
		// 필요한 초기화를 추가할 수 있습니다.
	}

	// 매개 변수가 있는 생성자
	FCardEffect(
		const TSubclassOf<UCardEffectComponent>& InEffectClass,
		int32 InEffectValue,
		const TSubclassOf<AAction_Effect>& InEffectAction,
		UNiagaraSystem* InParticle,
		const FGameplayTagContainer& InGameplayTags,
		const TSubclassOf<UActorComponent>& InTargetComponent,
		const TSubclassOf<UTargetingComponent>& InTargeting,
		const FDataTableRowHandle& InUsedData,
		const FGameplayTag& InHeroAnim,
		const FString& InIdentifier
	)
		: EffectClass(InEffectClass)
		  , EffectValue(InEffectValue)
		  , EffectAction(InEffectAction)
		  , HeroAnim(InHeroAnim)
		  , GameplayTags(InGameplayTags)
		  , Particle(InParticle)
		  , TargetComponent(InTargetComponent)
		  , Targeting(InTargeting)
		  , UsedData(InUsedData)
		  , Identifier(InIdentifier)
	{
		// 초기화와 동시에 멤버 변수들을 설정합니다.
	}

	// 복사 생성자
	FCardEffect(const FCardEffect& Other)
	{
		*this = Other; // 대입 연산자를 사용하여 복사
	}

	// 대입 연산자
	FCardEffect& operator=(const FCardEffect& Other)
	{
		if (this != &Other) // 자기 자신과의 대입을 방지
		{
			EffectClass = Other.EffectClass;
			EffectValue = Other.EffectValue;
			EffectAction = Other.EffectAction;
			Particle = Other.Particle;
			GameplayTags = Other.GameplayTags;
			TargetComponent = Other.TargetComponent;
			Targeting = Other.Targeting;
			UsedData = Other.UsedData;
			HeroAnim = Other.HeroAnim;
			Identifier = Other.Identifier;
		}
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FUseRule : public FTableRowBase
{
	GENERATED_BODY()

	// 사용 규칙을 나타내는 컴포넌트 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Use Rule")
	TSubclassOf<UUseRuleComponent> Rule;

	// 관련된 상태 컴포넌트 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Use Rule")
	TSubclassOf<UStatusComponent> Status;

	// 사용에 필요한 비용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Use Rule")
	int32 Cost;

	// 기본 생성자
	FUseRule() : Rule(nullptr), Status(nullptr), Cost(0) // 기본 비용을 0으로 설정
	{
		// 추가적으로 초기화가 필요할 경우 여기에 작성
	}

	// 매개 변수가 있는 생성자
	FUseRule(TSubclassOf<UUseRuleComponent> InRule, TSubclassOf<UStatusComponent> InStatus, int32 InCost)
		: Rule(InRule), Status(InStatus), Cost(InCost)
	{
		// 초기화와 동시에 값을 설정합니다.
	}

	// 복사 생성자
	FUseRule(const FUseRule& Other)
	{
		*this = Other; // 대입 연산자를 사용하여 복사
	}

	// 대입 연산자
	FUseRule& operator=(const FUseRule& Other)
	{
		if (this != &Other) // 자기 자신과의 대입을 방지
		{
			Rule = Other.Rule;
			Status = Other.Status;
			Cost = Other.Cost;
		}
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FCard : public FTableRowBase
{
	GENERATED_BODY()

	// 카드 소유자 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FString OwnerID;

	// 카드 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FText CardName;

	// 카드 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FText Description;

	// 카드가 타겟을 지정하는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bTargeted;

	// 카드 반복 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	int32 Repetitions;

	// 카드 태그 (GameplayTags)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FGameplayTagContainer CardTags;

	// 카드의 툴팁 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card", meta = (RowType="/Script/CrownOfSin.ToolTip"))
	TArray<FDataTableRowHandle> Tooltips;

	// 카드 사용 후 발생하는 이벤트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FGameplayTag PostUseEvent;

	// 턴 종료 시 발생하는 이벤트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FGameplayTag EndTurnEvent;

	// 카드 초상화 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTexture2D* Portrait;

	// 카드의 테두리 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTexture2D* Frame;

	// 카드 테두리 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FSlateColor FrameTint;

	// 카드 시각적 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TSubclassOf<UUserWidget> CardVisualWidget;

	// 카드 시작 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TArray<FStatusData> StartingStatuses;

	// 카드 효과
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TArray<FCardEffect> CardEffects;

	// 카드의 사용 규칙
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	TArray<FUseRule> UseRules;

	// 카드 데이터 테이블 행 핸들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FDataTableRowHandle DataRow;

	// 기본 생성자
	FCard()
		: OwnerID(TEXT(""))
		  , CardName(FText::FromString(TEXT("")))
		  , Description(FText::FromString(TEXT("")))
		  , bTargeted(false)
		  , Repetitions(0)
		  , Portrait(nullptr)
		  , Frame(nullptr)
		  , FrameTint(FLinearColor(1, 1, 1, 1))
	{
	}

	// 복사 생성자
	FCard(const FCard& Other)
	{
		*this = Other; // 대입 연산자를 사용하여 복사
	}

	// 대입 연산자
	FCard& operator=(const FCard& Other)
	{
		if (this != &Other) // 자기 자신과의 대입을 방지
		{
			OwnerID = Other.OwnerID;
			CardName = Other.CardName;
			Description = Other.Description;
			Portrait = Other.Portrait;
			CardTags = Other.CardTags;
			Tooltips = Other.Tooltips;
			CardEffects = Other.CardEffects;
			bTargeted = Other.bTargeted;
			UseRules = Other.UseRules;
			PostUseEvent = Other.PostUseEvent;
			EndTurnEvent = Other.EndTurnEvent;
			StartingStatuses = Other.StartingStatuses;
			Repetitions = Other.Repetitions;
			Frame = Other.Frame;
			FrameTint = Other.FrameTint;
			CardVisualWidget = Other.CardVisualWidget;
			DataRow = Other.DataRow;
		}
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FCardOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Option")
	FDataTableRowHandle Card;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Option")
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Option")
	int32 MaxRepeat = 2;
};

USTRUCT(BlueprintType)
struct FCardOptions : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Options")
	TArray<FCardOption> CardOptions;
};

USTRUCT(BlueprintType)
struct FCardPattern : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CardPattern")
	TArray<FCardOptions> CardPattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CardPattern")
	int32 RepeatFrom = 0;

	FCardPattern()
	{
	}
};


USTRUCT(BlueprintType)
struct FHeroDeck
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Deck")
	FDataTableRowHandle Hero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Deck")
	FDataTableRowHandle Deck;
};

USTRUCT(BlueprintType)
struct FDeck : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Deck")
	FText DeckName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Deck", meta = (RowType="/Script/CrownOfSin.Card"))
	TArray<FDataTableRowHandle> Cards;
};

USTRUCT(BlueprintType)
struct FRarityWeights : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rarity Weights")
	TMap<FGameplayTag, float> RarityWeights;
};
