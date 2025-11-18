# [ 🗡️ Roguelike Deck Building ]

<img width="643" height="388" alt="image" src="https://github.com/user-attachments/assets/5b59fd44-ea73-4711-b9d3-f32dfbd2f605" />


로그라이크 덱 빌딩 게임 블루프린트 애셋을 C++ 코드로 컨버팅한 프로젝트입니다. 
<br>
진행방식은 슬레이어 더 스파이어(Slayer the spire)와 유사합니다. 

## 📖 프로젝트 개요

- **개발 기간:** 2024. 09. ~ 2024.10 (약 한달)
- **개발 인원:** [ 2인 (팀 프로젝트) ]
- **GitHub:** [ [https://github.com/sssukh/RogueShooter/new/master](https://github.com/sssukh/Roguelike-DeckBuilder) ]

<br>

## 🎮 주요 시연 

- 시작 화면 및 지도에서 진행 사항 선택
![RLDBStart](https://github.com/user-attachments/assets/8a999b65-a954-47ed-9847-69fd2d3fc4a3)

- 전투 시작 및 전리품 획득
![RLDBBattle](https://github.com/user-attachments/assets/533226fe-b7c9-49a4-aadc-f2001dd9f81c)

- 스토리 진행 및 유물 획득
![RLDBStory](https://github.com/user-attachments/assets/cfa9cbcd-c195-4885-8d42-03636ec59ce4)

- 모닥불 및 상점
![RLDBEvent](https://github.com/user-attachments/assets/0280a005-e6fa-4a34-9128-a7374faf816e)

<br>



## 🛠️ 사용 기술

- **Game Engine:**
    
    ![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.4-black?style=flat-square&logo=unrealengine)
    
- **Language:**
    
    ![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus)
    
- **Tools:** 
    
    ![Rider](https://img.shields.io/badge/Rider-purple?logo=Rider)
    
    ![Git](https://img.shields.io/badge/Git-Orange?logo=Git)
    
- **Version Control:**
  
    ![Github](https://img.shields.io/badge/Github-gray?logo=Github)


<br>

## ✨ 주요 기능 및 구현 내용

### 1. [핵심 기능 1: GameplayTag를 이용한 이벤트 디스패처]

- GameplayTag를 이용한 이벤트 관리
- 옵저버 패턴을 이용한 글로벌 이벤트 디스패처에서 이벤트 관리
- 액터들은 특정 이벤트에 구독하여 해당 이벤트가 발생할 때 정의한 행동을 수행하도록 함
- 모든 액터에게 관련이 있는 Event를 위한 Global Dispatcher Hub와 특정 액터에게만 적용되는 Event를 위한 Local Dispatcher Hub가 존재


    
- 코드 : https://github.com/sssukh/Roguelike-DeckBuilder/blob/main/Source/CrownOfSin/Private/Core/DispatcherHubComponent.cpp

- 예시)

    BrokenShield 아티팩트는 영웅이 공격할 때 발생하는 공격 이벤트를 구독하다가 이벤트가 발생할 때 마다 영웅에게 방어도를 부여하는 행동을 수행한다.

    [Post_Attack 이벤트에 구독]

    <img width="990" height="153" alt="image" src="https://github.com/user-attachments/assets/0412d1c0-abb9-451d-8615-f12bd1a3059f" />
    [Post_Attack 이벤트 발생]
    
    <img width="1209" height="285" alt="image" src="https://github.com/user-attachments/assets/b8a01931-85ae-48cb-be67-3193872519d2" />
  <br>예시 코드 : https://github.com/sssukh/Roguelike-DeckBuilder/blob/main/Source/CrownOfSin/Private/StatusSystem/Artifacts/Status_Artifact_BrokenShield.cpp


### 2. [핵심 기능 2: 액션]

- 특수효과, 화면전환, 애니메이션 등 게임플레이 로직과 관련없는 것들을 액션으로 처리
- 옵저버 패턴을 이용한 글로벌 액션 매니저에서 큐를 이용해 액션 관리
- 코드 : https://github.com/sssukh/Roguelike-DeckBuilder/blob/main/Source/CrownOfSin/Private/ActionSystem/ActionManagerSubsystem.cpp


### 3. [핵심 기능 3: 카드]

![image](https://github.com/user-attachments/assets/975a2d70-9bc3-4b3b-bb6a-be35248da0bd)

- 일반적인 카드 사용 외에도 몬스터의 스킬 사용이나 보상 획득 같은 게임 내 주요 활동도 카드를 통해 이루어짐
- 전략 패턴을 적용하여 카드가 게임 내에서 실제로 수행하는 동작의 정의를 CardEffectComponent 클래스로 쉽게 적용
- TargetComponent를 통해 CardEffect의 대상 설정 가능
- 이름, 설명, 이미지, 희귀도, 카드 타입, 마나 코스트 등을 포함해 핵심 속성을 확인할 수 있고 툴팁을 사용해 동적인 값을 정확히 확인 가능




### 4. [핵심 기능 : 노드]

![image](https://github.com/user-attachments/assets/15dd16e3-ea3a-4743-91f7-8468b1047c9d)

- 플레이어가 이동하며 다양한 인카운터를 만나는 게임 진행 구조
- 노드와 노드 사이를 연결하는 연결선을 따라 이동하며, 최종적으로 보스에게 도달하거나 패배
- 전투 인카운터, 스토리 인카운터, 보물 인카운터 등의 유형 존재
- 노드를 클릭했을 때 Map Event를 참조하여 어떤 종류의 이벤트를 스폰하고 실행할지 결정
- 특정 이벤트를 지정하거나 값 조정을 통해 무작위 인카운터 설정 가능

### 5. [전투]

![image](https://github.com/user-attachments/assets/e79b890b-6829-417c-adb7-fda4d4ae6b29)

- 노드 맵에서 전투 인카운터를 클릭하거나 조우한 스토리에 따라 전투에 돌입
- 양쪽에 MinionTrack이라는 Minion(캐릭터, 적 등)이 서있을 수 있는 트랙이 존재
- 턴매니저를 통해 턴을 관리
- 카드를 이용해 적을 쓰러뜨려 플레이를 이어나가거나 패배

### 6. [스토리]

![image](https://github.com/user-attachments/assets/c92a4391-0707-45b7-b1ae-a6207e045e6a)

- 노드 맵에서 스토리 인카운터를 클릭하면 무작위로 선택되는 무작위 인카운터와 캠프파이어, 상점과 같이 특정 노드에 나타나는 고정된 이벤트가 존재
- 상황을 설명하는 텍스트와 플레이어가 선택할 수 있는 옵션 존재
- 옵션은 실제로는 카드로 정의되어 정의된 카드 효과가 발동됨(예 : 아티팩트 부여, 저주 카드 추가 등)
- 테이블 내에서 희귀도나 설정된 태그를 가진 이벤트를 필터링을 통해 걸러낸 후 랜덤으로 제공



### 7. [스테이터스]

![image](https://github.com/user-attachments/assets/5eb5e35d-c63f-4e90-83a4-e35573fcda98)
![image](https://github.com/user-attachments/assets/25142e93-7698-4cc3-91d2-444157ec889c)

- 일반 상태 효과(예 : 힘, 분노, 중독) 외에도 능력치(체력, 마나 획득), 몬스터의 행동패턴, 카드에 첨부되는 특수효과, 아티팩트 표현
- 대부분 BeginPlay 시 dispatcher hub에 이벤트를 구독 후 특정 이벤트가 발생하면 정의된 효과 발동






---
원본 애셋 출처 :https://www.unrealengine.com/marketplace/ko/product/roguelike-deckbuilder-toolkit




