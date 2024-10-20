# - Roguelike DeckBuilding

로그라이크 덱 빌딩 게임 블루프린트 애셋을 C++ 코드로 컨버팅한 프로젝트입니다.

---

노드에서의 움직임 + 이벤트로 이루어져있다.

이벤트로는 전투, 스토리, 상점이 있다.

현재 노드의 이벤트를 완료하면 노드에서 다음 진행을 선택한다.


# - 노드

![image](https://github.com/user-attachments/assets/15dd16e3-ea3a-4743-91f7-8468b1047c9d)
NodeMap에서 맵에 있는 노드들을 관리하고 업데이트 합니다.

노드 방문시 노드에 설정된 전투, 스토리, 보물에 따라 이벤트를 호출합니다.

스토리의 경우 이번 게임에서 아직 경험하지 않은 스토리만 보여줍니다.

# - 전투

![image](https://github.com/user-attachments/assets/e79b890b-6829-417c-adb7-fda4d4ae6b29)
노드 맵에서 전투 노드를 클릭하면 전투에 돌입한다.

양쪽에 MinionTrack이라는 Minion(캐릭터, 적 등)이 서있을 수 있는 트랙이 존재한다.

# - 스토리

![image](https://github.com/user-attachments/assets/c92a4391-0707-45b7-b1ae-a6207e045e6a)

데이터 테이블을 통해 관리하고 선택지 마다 효과(카드)를 부여해서 선택 시 해당 효과가 발동된다.

# - 카드

![image](https://github.com/user-attachments/assets/975a2d70-9bc3-4b3b-bb6a-be35248da0bd)

일반적인 카드이다.
하지만 이 프로젝트에서는 카드 이외에도 효과발동 클래스로도 사용된다.

반복 횟수, 카드이펙트와 타겟을 설정해줘야 한다.

타겟은 설정된 TargetComponent라는 클래스로 타겟 선택 방법이 결정된다.

데이터 테이블을 통해 관리한다.

# - 스테이터스

![image](https://github.com/user-attachments/assets/5eb5e35d-c63f-4e90-83a4-e35573fcda98)
![image](https://github.com/user-attachments/assets/25142e93-7698-4cc3-91d2-444157ec889c)

전투에서의 방어도, 독, 힘 과 같은 상태를 나타내며 그 정도를 나타내는 Value값이 있다.

이 프로젝트에서는 Artifact라는 캐릭터에게 지속되는 효과에도 이 클래스를 사용한다.


# - 액션
게임플레이 로직을 제외한 애니메이션, 효과를 생성 및 발동시키는 클래스이다.(화면 흔들림, 

ActionManager에게 관리되고 생성 시 자동으로 큐에 추가되고 선입선출로 나갈때 Action이 발동된다.

# - Dispatcher(Local, Global)

게임 내 다양한 이벤트 시스템을 관리하고 조율하는 핵심 컴포넌트로 각 객체가 특정 이벤트에 반응할 수 있도록 이벤트를 바인딩하고 호출하는 역할을 한다.

이벤트 홀더 객체들은 우선순위에 따라 정렬되고 이벤트 호출 시 우선순위에 따라 처리된다.

GameplayTag를 이용해 이벤트들을 관리한다.


# - TurnManager
전투 단계의 유저 캐릭터와 적들의 턴을 관리한다.

또한 승리와 패배 여부도 체크한다.


---
원본 애셋 출처 :https://www.unrealengine.com/marketplace/ko/product/roguelike-deckbuilder-toolkit




