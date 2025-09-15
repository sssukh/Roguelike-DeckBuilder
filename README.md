# - Roguelike DeckBuilding

로그라이크 덱 빌딩 게임 블루프린트 애셋을 C++ 코드로 컨버팅한 프로젝트입니다.

진행방식은 슬레이어 더 스파이어(Slayer the spire)와 유사합니다.

# - 카드

![image](https://github.com/user-attachments/assets/975a2d70-9bc3-4b3b-bb6a-be35248da0bd)

일반적인 카드입니다.

타겟은 설정된 TargetComponent라는 클래스로 타겟 선택 방법이 결정됩니다.
대상은 설정된 TargetComponent에 따라 적이 되거나 플레이어가 되거나 다른 무엇이 될 수 있습니다.

이 프로젝트에서는 전투에서 사용하는 카드 이외에도 여러가지 효과발동 클래스로도 사용됩니다.
플레이어에게 유물을 제공하는 효과로 사용하거나 스토리에서 적과의 조우, 플레이어에게 피해 주기 등 여러가지 사용법이 있습니다.

UseRule이라는 카드 사용 조건과 비용을 설정할 수 있습니다.

데이터 테이블을 통해 관리합니다.

# - 노드

![image](https://github.com/user-attachments/assets/15dd16e3-ea3a-4743-91f7-8468b1047c9d)

NodeMap에서 맵에 있는 노드들을 관리하고 업데이트 합니다.

노드는 MapEvent 데이터를 가지며, 이는 휴식, 상점, 스토리, 전투 라는 인카운터 유형을 결정합니다.

맵 이벤트 데이터 테이블에 희귀도나 태그에 따라 어떤 아이콘, 이벤트를 나타낼지 지정할 수 있습니다.

스토리의 경우 이번 게임에서 아직 경험하지 않은 스토리만 보여줍니다.

# - 전투

![image](https://github.com/user-attachments/assets/e79b890b-6829-417c-adb7-fda4d4ae6b29)

노드 맵에서 전투 노드를 클릭하거나 조우한 스토리에 따라 전투에 돌입합니다.

턴매니저를 통해 턴을 관리합니다.

양쪽에 MinionTrack이라는 Minion(캐릭터, 적 등)이 서있을 수 있는 트랙이 존재합니다.

카드를 이용해 적을 쓰러뜨려 플레이를 이어나가거나 패배할 수 있습니다.

# - 스토리

![image](https://github.com/user-attachments/assets/c92a4391-0707-45b7-b1ae-a6207e045e6a)

데이터 테이블을 통해 관리하고 선택지 마다 효과(카드)를 부여해서 선택 시 해당 효과가 발동됩니다.



# - 스테이터스

![image](https://github.com/user-attachments/assets/5eb5e35d-c63f-4e90-83a4-e35573fcda98)
![image](https://github.com/user-attachments/assets/25142e93-7698-4cc3-91d2-444157ec889c)

전투에서의 방어도, 독, 힘 과 같은 상태를 나타내며 그 정도를 나타내는 Value값이 있습니다.

이 프로젝트에서는 Artifact라는 캐릭터에게 지속되는 효과에도 이 클래스를 사용합니다.


# - 액션

게임플레이 로직을 제외한 애니메이션, 효과를 생성 및 발동시키는 클래스입니다.(예. 화면 흔들림 등)

ActionManager에게 관리되고 생성 시 자동으로 큐에 추가되고 선입선출로 나갈때 Action이 발동됩니다.


# - Dispatcher(Local, Global)

게임 내 다양한 Event 시스템을 관리하고 조율하는 핵심 컴포넌트로 각 객체가 특정 GameplayTag를 이용한 Event에 반응할 수 있도록 이벤트를 바인딩하고 호출하는 역할을 합니다.

Event 홀더 객체들은 우선순위에 따라 정렬되고 Event 호출 시 우선순위에 따라 처리됩니다.

GameplayTag를 이용해 이벤트들을 관리하고 .

모든 액터에게 관련이 있는 Event를 위한 Global Dispatcher Hub와 특정 액터에게만 적용되는 Event를 위한 Local Dispatcher Hub가 있습니다.

예) 

    Global Dispatcher Hub에서 Turn.Start Event에 바인딩을 해두면 턴이 시작될 때 마다 게임 전반에 걸친 효과를 처리할 수 있습니다.
    
    미니언의 Local Dispatcher Hub에서 Turn.Start Event에 바인딩을 해두면 해당 미니언이 턴마다 힘을 얻는 효과를 처리할 수 있습니다.

---
원본 애셋 출처 :https://www.unrealengine.com/marketplace/ko/product/roguelike-deckbuilder-toolkit




