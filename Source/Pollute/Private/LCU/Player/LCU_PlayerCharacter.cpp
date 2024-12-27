// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "HHR/HHR_WeaponItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LCU/InteractActors/LCU_Curse.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "HHR/UI/HHR_PlayerHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NSK/NSK_LadderInstallPoint.h"
#include "EngineUtils.h"
#include <NSK/NSK_Ladder.h>
#include "Engine/Scene.h"
#include "Components/ProgressBar.h"
#include "NSK/NSK_Parachute.h"
#include "HHR/UI/HHR_ItemDialog.h"
#include "P_Settings/P_GameState.h"
#include "LCU/UI/LCU_UIManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

// Sets default values
ALCU_PlayerCharacter::ALCU_PlayerCharacter()
	: Super()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	// 박스 컴포넌트 생성 및 루트 컴포넌트로 설정
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	
	// 박스 크기 설정
	BoxComponent->InitBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	
	// 충돌 설정
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_Visibility);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 플레이어와만 오버랩
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // 아이템과도 오버랩

	// 오버랩 이벤트 바인딩
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALCU_PlayerCharacter::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALCU_PlayerCharacter::OnBoxEndOverlap);
    
}

// Called when the game starts or when spawned
void ALCU_PlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    LCU_Pc = Cast<ALCU_PlayerController>(GetController());
    if(CurseMaterial)
    {
        CurseMatInstance = UMaterialInstanceDynamic::Create(CurseMaterial, this);
    }
    
}

void ALCU_PlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
	
	Super::EndPlay(EndPlayReason);
	
}

void ALCU_PlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    LCU_Pc = Cast<ALCU_PlayerController>(GetController());
}

void ALCU_PlayerCharacter::UpdateCameraTransform()
{
	if (FollowCamera)
	{
		// 카메라 위치와 회전 가져오기
		FVector CameraLocation = FollowCamera->GetComponentLocation();
		FRotator CameraRotation = FollowCamera->GetComponentRotation();

		// 박스 위치를 카메라 앞 BoxDistanceFromCamera 만큼 이동
		FVector BoxLocation = CameraLocation + CameraRotation.Vector() * 100.f;

		// 박스 콜리전 업데이트
		BoxComponent->SetWorldLocation(BoxLocation);
		BoxComponent->SetWorldRotation(CameraRotation);
	}
}

// Called every frame
void ALCU_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateCameraTransform();
	FinalOverapPlayer = Cast<ALCU_PlayerCharacter>(GetClosestActorToCamera(OverlappingPlayers));
	FinalOverapItem = GetClosestActorToCamera(OverlappingItems);
    if(IsValid(FinalOverapItem) && IsLocallyControlled() && !ItemInHand)
    {
        AHHR_Item* item = Cast<AHHR_Item>(FinalOverapItem);
        if(item)
        {
            LCU_Pc->UIManager->PlayerHUD->ItemDialog->SetText(item->ItemData.ItemName);
            LCU_Pc->UIManager->PlayerHUD->SetItemDialogVisibility(true);
        }
    }
    else if(!IsValid(FinalOverapItem) && IsLocallyControlled() || ItemInHand && IsLocallyControlled())
    {
       LCU_Pc->UIManager->PlayerHUD->SetItemDialogVisibility(false);
    }

    if(FinalOverapItem)
    {
        RetrievedItem = Cast<AHHR_Item>(FinalOverapItem);
    }
    
    if(StartCurseCool)
    {
        if(IsLocallyControlled())
        {
            CarryCurseCool -= DeltaTime;
            LCU_Pc->UIManager->PlayerHUD->SetCarryCurseCool(CarryCurseCool);
            
            if(CarryCurseCool <= 0)
            {
                StartCurseCool = false;
                CarryCurseCool = MaxCurseCool;
            }
        }
    }
}

// Called to bind functionality to input
void ALCU_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_CarryCurse, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::CarryCurse);
		EnhancedInputComponent->BindAction(IA_PickUpDropDown, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::PickUpDropDown);
	    EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::Attack);
        //EnhancedInputComponent->BindAction(IA_G, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::OnInteract);
	    EnhancedInputComponent->BindAction(IA_PutItemOnAltar, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::PutItemOnAltar);
	    EnhancedInputComponent->BindAction(IA_RunToggle, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::RunShiftToggle);
        EnhancedInputComponent->BindAction(IA_Ladder, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::OnInstallLadder);
        EnhancedInputComponent->BindAction(IA_ClimingLadder, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::InteractWithLadder);
        EnhancedInputComponent->BindAction(IA_Parachute, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::InteractWithParachute);
	}
}

void ALCU_PlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ALCU_PlayerCharacter, HealthCount);
    DOREPLIFETIME(ALCU_PlayerCharacter, ItemInHand);
    DOREPLIFETIME(ALCU_PlayerCharacter, bHasCurse);
    DOREPLIFETIME(ALCU_PlayerCharacter, bIsRunning);
    DOREPLIFETIME(ALCU_PlayerCharacter, StartCurseCool);

    // hr altar
    DOREPLIFETIME(ALCU_PlayerCharacter, bNearByAltar);
    DOREPLIFETIME(ALCU_PlayerCharacter, LCU_Pc);
}

void ALCU_PlayerCharacter::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();
    
    if (Controller != nullptr)
    {        
        // 입력 방향 벡터 가져오기
        const float MoveForwardValue = MovementVector.Y;
        const float MoveRightValue = MovementVector.X;

        if(IsLocallyControlled())
        {
            ServerRPC_UpdateSpeed(MoveForwardValue, MoveRightValue);
        }
        
        // 방향 벡터 계산
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // 입력 벡터를 기반으로 이동 적용
        AddMovementInput(ForwardDirection, MoveForwardValue);
        AddMovementInput(RightDirection, MoveRightValue);
    }
}

void ALCU_PlayerCharacter::RunShiftToggle()
{
   if(IsLocallyControlled())
   {
       ServerRPC_SetRunning(!bIsRunning);
   }
}

void ALCU_PlayerCharacter::ServerRPC_SetRunning_Implementation(bool run)
{
    bIsRunning = run;
}

void ALCU_PlayerCharacter::ServerRPC_UpdateSpeed_Implementation(float MoveForwardValue, float MoveRightValue)
{    
    // 현재 속도를 WalkSpeed로 초기화
    float NewSpeed = WalkSpeed;
    
    // 달리기 조건 확인
    if (MoveForwardValue > 0 && MoveRightValue <= 0.3f && MoveRightValue >= -0.3f  && bIsRunning)
    {
        NewSpeed = RunSpeed;
    }
    
    // MaxWalkSpeed 업데이트
    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

    NetMulticast_UpdateSpeed(NewSpeed);
}

void ALCU_PlayerCharacter::NetMulticast_UpdateSpeed_Implementation(float NewSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ALCU_PlayerCharacter::Interact()
{
    UAnimInstance* animInstance= GetMesh()->GetAnimInstance();
    if(animInstance && animInstance->IsAnyMontagePlaying())
    {
        animInstance->StopAllMontages(0.f);
    }
    animInstance->Montage_Play(HitMontage);
    
	HealthCount--;
    if(HealthCount == 2)
    {
        bInjuredBody = true;
        WalkSpeed = 300.f;
        RunSpeed = 600.f;
        P_SCREEN(5.f, FColor::Black, TEXT("SpeedChange %f , %f"), WalkSpeed, RunSpeed);
    }
    if(HealthCount <= 0)
    {
        HealthCount = 0;
        DieProcess();
    }
}

AActor* ALCU_PlayerCharacter::GetClosestActorToCamera(const TSet<AActor*>& Actors)
{
	// 자료구조 안에 데이터가 없으면 반환
	if (Actors.Num() == 0)
	{
	    return nullptr;
	}

	FVector CameraLocation = FollowCamera->GetComponentLocation();

	AActor* ClosestActor = nullptr;
	float MinDistanceSquared = FLT_MAX;

	// 액터들 중에 카메라와의 거리가 가장 가까운 액터를 선택합니다.
	for (AActor* Actor : Actors)
	{
		if (!Actor) continue;

		float DistanceSquared = FVector::DistSquared(Actor->GetActorLocation(), CameraLocation);
		if (DistanceSquared < MinDistanceSquared)
		{
			MinDistanceSquared = DistanceSquared;
			ClosestActor = Actor;
		}
	}
	return ClosestActor;
}

void ALCU_PlayerCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 저주 pass 동작 등 
    
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// 충돌 채널 가져오기
		ECollisionChannel ObjectType = OtherComp->GetCollisionObjectType();

		// 채널에 따른 처리
		switch (ObjectType)
		{
		case ECC_Pawn:
			if(OtherActor == this || !bHasCurse) break;
			AddOverlappingPlayer(OtherActor);
			break;
		case ECC_GameTraceChannel1:
			{				
				AddOverlappingItem(OtherActor);
				break;
			}
		default:
			break;
		}
	}
}

void ALCU_PlayerCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// 충돌 채널 가져오기
		ECollisionChannel ObjectType = OtherComp->GetCollisionObjectType();

		// 채널에 따른 처리
		switch (ObjectType)
		{
		case ECC_Pawn: // 플레이어 채널 (캐릭터와 오버랩)
			RemoveOverlappingPlayer(OtherActor);
			break;

		case ECC_GameTraceChannel1: // 아이템 채널 ( 아이템 오브젝트)
			RemoveOverlappingItem(OtherActor);
			break;

		default:
			break;
		}
	}
}

void ALCU_PlayerCharacter::AddOverlappingPlayer(AActor* otherActor)
{
	if(!OverlappingPlayers.Contains(otherActor))
	{
		OverlappingPlayers.Add(otherActor);
	}
}

void ALCU_PlayerCharacter::RemoveOverlappingPlayer(AActor* otherActor)
{
	if(OverlappingPlayers.Contains(otherActor))
	{
		OverlappingPlayers.Remove(otherActor);
	}
}

void ALCU_PlayerCharacter::AddOverlappingItem(AActor* otherActor)
{
	if(!OverlappingItems.Contains(otherActor))
	{
		OverlappingItems.Add(otherActor);
	} 
}

void ALCU_PlayerCharacter::RemoveOverlappingItem(AActor* otherActor)
{
	if(OverlappingItems.Contains(otherActor))
	{
		OverlappingItems.Remove(otherActor);
	}
}

void ALCU_PlayerCharacter::CarryCurse()
{
	// 저주가 없으면 줄 것도 없으니 나가세요
	if(!bHasCurse) return;

    // 아직 저주 넘기기 쿨타임 상태면 나가세여
    if(StartCurseCool) return;
    StartCurseCool = true;
    LCU_Pc->UIManager->PlayerHUD->CarryCurseCool->SetPercent(1.0f);
    
	// 현재 눈 앞에 줄 수 있는 플레이어도 없으니 나가세요.
	if(!FinalOverapPlayer) return;

	ServerRPC_CarryCurse();
}

bool ALCU_PlayerCharacter::IsInMatToCamera(UMaterialInstanceDynamic* DynaminMat)
{
    if (DynaminMat)
    {
        // Blendables 리스트에서 해당 머티리얼이 포함되어 있는지 확인
        for (const auto& Blendable : FollowCamera->PostProcessSettings.WeightedBlendables.Array)
        {
            if (Blendable.Object == DynaminMat)
            {
                return true; // 머티리얼이 이미 추가됨
            }
        }
    }
    return false; // 현재 머터리얼이 없어요!
}

void ALCU_PlayerCharacter::ClientRPC_SetCurseScalar_Implementation(float scalar)
{
    if (CurseMatInstance)
    {
        CurseMatInstance->SetScalarParameterValue(FName("R_Density"), scalar);
    }
}

void ALCU_PlayerCharacter::ClientRPC_SetCurseMat_Implementation(bool bShow)
{
    if(bShow)
    {
        if(!IsInMatToCamera(CurseMatInstance))
        {
            FollowCamera->PostProcessSettings.AddBlendable(CurseMatInstance, 1.0f);
        }        
    }
    else
    {
        if(IsInMatToCamera(CurseMatInstance))
        {
            FollowCamera->PostProcessSettings.RemoveBlendable(CurseMatInstance);
        }   
    }
}


void ALCU_PlayerCharacter::ServerRPC_CarryCurse_Implementation()
{
    // 저주를 옮겨요
    FinalOverapPlayer->SetHasCurse(true);
    ALCU_PlayerController* localPc = Cast<ALCU_PlayerController>(FinalOverapPlayer->GetController());
    if(localPc)
    {
       localPc->ClientRPC_CurseUISet(true);
        FinalOverapPlayer->StartCurseCool = true;
        FinalOverapPlayer->ClientRPC_SetCurseMat(true);
    }
    bHasCurse = false;
    if(LCU_Pc)
    {
        LCU_Pc->ClientRPC_CurseUISet(false);
    }
    //LCU_Pc->CurseUISet(false);
    StartCurseCool = false;
    CarryCurseCool = MaxCurseCool;
    ClientRPC_SetCurseMat(false);
    ALCU_Curse::GetInstance(GetWorld())->SetCharacter(FinalOverapPlayer);

    // 이제 본인이 가지고 있던 FinalOverap 후보들을 전부 삭제해요
    FinalOverapPlayer = nullptr;
    if(!OverlappingPlayers.IsEmpty())
    {
        OverlappingPlayers.Empty();
    }
}

void ALCU_PlayerCharacter::DropDown()
{
    if(!ItemInHand) return;
    FVector CharacterLocation = GetActorLocation();
    // 캐릭터 발 아래 위치
    FVector DropLocation = CharacterLocation - FVector(0.0f, 0.0f, 90.0f);
    // 아이템이 캐릭터 방향을 따라 회전하도록 설정
    FRotator DropRotation = GetActorRotation(); 

    // FinalOverlapItem을 월드에 분리
		
    // 아이템의 부모-자식 관계 해제
    FinalOverapItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // 위치 및 회전 설정
    FinalOverapItem->SetActorLocation(DropLocation);
    FinalOverapItem->SetActorRotation(DropRotation);

    // 드롭 이후 초기화
    FinalOverapItem = nullptr;
    bHasItem = false;

    // Drop 후에 핸드에 있는 아이템 null 초기화
    ItemInHand = nullptr;
    
}

void ALCU_PlayerCharacter::PickUpDropDown()
{
    ServerRPC_PickUpDropDown();
}

void ALCU_PlayerCharacter::ServerRPC_PickUpDropDown_Implementation()
{
    
    // 주울 수 있는 아이템이 없으면 나가야함
    if(!FinalOverapItem) return;
    
    // 현재 아이템이 없으니 픽업
    if(!ItemInHand)
    {
        // 근처에 제단 있으면 제단으로부터 아이템 얻었는지 확인
        // 델리게이트 호출
        // TODO : 모든 경우에 다 확인하는 거 에바임... 근데 bNearByAltar만으로는 예외 상황이 존재 -> 주울 수 있는 범위랑 bNearByAltar 설정 범위가 달라서 발생하는 문제
        //if(bNearByAltar)
        //{
        if(OnDettachItemOnAltar.IsBound())
        {
            if(Cast<AHHR_Item>(FinalOverapItem))
            {
                OnDettachItemOnAltar.Execute(Cast<AHHR_Item>(FinalOverapItem));
            }
        }
        //}
        NetMulticast_AttachItem();

    }
    // 아이템을 가지고 있으니 드랍다운
    else
    {
        NetMulticast_DetachItem();

    }
    
}


void ALCU_PlayerCharacter::NetMulticast_AttachItem_Implementation()
{
    ItemInHand = Cast<AHHR_Item>(FinalOverapItem);
    /*if(ItemInHand)
    {
        ItemInHand->SetOwner(this);
    }*/
    AttachItem();
}


void ALCU_PlayerCharacter::AttachItem()
{

    if (ItemInHand && ItemInHand->IsA<ANSK_Ladder>())
    {
        ANSK_Ladder* Ladder = Cast<ANSK_Ladder>(ItemInHand);
        if (Ladder && Ladder->bIsInstalled) // 사다리가 설치 됐다면
        {
            P_LOG(PolluteLog, Warning, TEXT("사다리가 설치되어 있어 아이템을 다시 들 수 없다"));
            return;
        }
    }

    // Item의 Interactive 허용
    if(IsLocallyControlled())
    {
        ItemInHand->GetItemInteractWidgetComponent()->SetVisibility(false);
    }

    USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
    if (!SkeletalMeshComp)
    {
        return;
    }

    // HHR 수정 
    if(ItemInHand)
    {
        // TODO : Attach를 Multicast로 싸줘야 함 
        ItemInHand->AttachToComponent(
            SkeletalMeshComp,                      
            FAttachmentTransformRules::SnapToTargetIncludingScale, 
            FName("PickUpSocket")                   
        );
        bHasItem = true;
        // 각 아이템 마다 위치 수정
        ItemInHand->SetActorRelativeLocation(ItemInHand->ItemData.ItemLocation);
        ItemInHand->SetActorRelativeRotation(ItemInHand->ItemData.ItemRotation);
        // Item의 Owner 설정
        ItemInHand->SetOwner(this);
        
        if(IsLocallyControlled() && LCU_Pc && LCU_Pc->UIManager)
        {
            LCU_Pc->UIManager->PlayerHUD->ChangeItemImage(ItemInHand->ItemData.ItemImage);
        }
    }
}

void ALCU_PlayerCharacter::NetMulticast_DetachItem_Implementation()
{
    DetachItem();
   
}

void ALCU_PlayerCharacter::DetachItem()
{
    if (!ItemInHand) return;
    FVector CharacterLocation = GetActorLocation();
    // 캐릭터 발 아래 위치
    FVector DropLocation = CharacterLocation - FVector(0.0f, 0.0f, 90.0f);
    // 아이템이 캐릭터 방향을 따라 회전하도록 설정
    FRotator DropRotation = GetActorRotation();

    // FinalOverlapItem을 월드에 분리
		
    // 아이템의 부모-자식 관계 해제
    // TODO : Detach를 Multicast로 싸줘야 함 
    ItemInHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // 위치 및 회전 설정
    ItemInHand->SetActorLocation(DropLocation);
    ItemInHand->SetActorRotation(DropRotation);

    // 드롭 이후 초기화
    ItemInHand = nullptr;
    bHasItem = false;

    if(IsLocallyControlled() && LCU_Pc && LCU_Pc->UIManager)
    {
        LCU_Pc->UIManager->PlayerHUD->ChangeItemImageNull();
        
    }
}

void ALCU_PlayerCharacter::DieProcess()
{
    if(LCU_Pc && IsLocallyControlled())
    {
        LCU_Pc->CurseUISet(false);
        LCU_Pc->ServerRPC_ChangeToSpector();
    }
}

void ALCU_PlayerCharacter::Attack()
{
    if(IsLocallyControlled() && ItemInHand)
    {
        ServerRPC_Attack();
    }
}

void ALCU_PlayerCharacter::InitItem()
{
    ItemInHand = nullptr;
}

void ALCU_PlayerCharacter::NetMulticast_Attack_Implementation()
{
    // Item 구하는 코드는 나중에 처리님이 들고 있는 아이템 추가하면 없애도 될듯
    AHHR_WeaponItem* weapon = Cast<AHHR_WeaponItem>(ItemInHand);
	
    // Item 종류(칼, 총)에 따라 다른 Montage 실행
    if(weapon)
    {
        UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
        
        if(animInstance)
        {
            if(weapon->GetIsUsed()) return;

            switch(weapon->GetWeaponType())
            {
            case EWeaponType::Knife:
                animInstance->Montage_Play(KnifeAttackMontage, 1.0f);
                break;
            case EWeaponType::TaserGun:
                animInstance->Montage_Play(GunAttackMontage, 1.0f);
                break;
            default:
                break;
            }

        }
        
    }
}

// NSK //
void ALCU_PlayerCharacter::SetNearbyAltar(ANSK_Altar* Altar, int32 SlotIndex)
{
    NearbyAltar = Altar;
    if (SlotIndex == -1)
    {
        SelectedSlotIndex = -1; // 슬롯 인덱스를 무효 상태로 설정
    }
    else
    {
        SelectedSlotIndex = SlotIndex; // 유효한 슬롯 인덱스 설정
    }
}

void ALCU_PlayerCharacter::ClearNearbyAltar()
{
    NearbyAltar = nullptr;
    SelectedSlotIndex = INDEX_NONE;
}


void ALCU_PlayerCharacter::ServerRPC_PutItemOnAltar_Implementation()
{
    // ItemInHand 손에서 Detatch
    // TODO : 바로 attach 해서 사실 detatch 안해줘도 됨. ㅇㅅㅇ
    AHHR_Item* tempItem = ItemInHand;
    //ServerRPC_DetatchItem();
    NetMulticast_DetachItem();

    // 델리게이트 실행 -> Attach Item On Altar
    // TODO : NetMulitcast로 
    if(OnAttachItemOnAltar.IsBound())
    {
        OnAttachItemOnAltar.Execute(tempItem);
    }
}


// G키
void ALCU_PlayerCharacter::PutItemOnAltar()
{
    // G 클릭시
    // Altar 아이템과 충돌 되어 있고, itemInHand를 가지고 있으면(+그 아이템이 조합 아이템이여야함) Delegate broadcast
    if(bNearByAltar && ItemInHand && ItemInHand->ItemData.ItemType == EItemType::CombineItem)
    {
        ServerRPC_PutItemOnAltar();
    }
}


void ALCU_PlayerCharacter::ServerRPC_SetPlayerType_Implementation(EPlayerType InPlayerType)
{
    MulticastRPC_UpdatePlayerMesh(InPlayerType);
}

void ALCU_PlayerCharacter::MulticastRPC_UpdatePlayerMesh_Implementation(EPlayerType InPlayerType)
{
    PlayerType = InPlayerType;
    GetMesh()->SetSkeletalMeshAsset(PlayerMeshType[(int32)PlayerType]);
}

void ALCU_PlayerCharacter::SetCurrentSlotIndex(int32 SlotIndex)
{
    SelectedSlotIndex = SlotIndex;
    P_LOG(PolluteLog, Warning, TEXT("현재 슬롯 인덱스 설정: %d"), SelectedSlotIndex);
}

void ALCU_PlayerCharacter::ClearCurrentSlotIndex()
{
    SelectedSlotIndex = INDEX_NONE; // 유효하지 않은 값으로 초기화
    P_LOG(PolluteLog, Warning, TEXT("현재 슬롯 인덱스 초기화"));
}

// NSK 캐릭터 제단 상호작용 로직
/*void ALCU_PlayerCharacter::OnInteract()
{
    if (NearbyAltar && ItemInHand)
    {
        if (SelectedSlotIndex != INDEX_NONE) // 유효한 슬롯 인덱스 확인
        {
            // 제단에 들고 있는 아이템 등록
            NearbyAltar->AddItemToSlot(ItemInHand->ItemData, SelectedSlotIndex);
            P_LOG(PolluteLog, Warning, TEXT("아이템 %s를 제단에 등록"), *ItemInHand->ItemData.ItemName.ToString());

            // 아이템 드랍 상태로 변경하고 손에서 놓기
            ItemInHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

            // 아이템 삭제
            ItemInHand->Destroy();

            ItemInHand = nullptr;
            bHasItem = false;
        }
        else
        {
            P_LOG(PolluteLog, Error, TEXT("유효하지 않은 슬롯 인덱스입니다."));
        }
    }
    else if (!ItemInHand && NearbyAltar)
    {
        if (SelectedSlotIndex != INDEX_NONE) // 유효한 슬롯 인덱스 확인
        {
            // 현재 슬롯에서 아이템 제거
            AHHR_Item* RetrievedItemFromSlot = NearbyAltar->RemoveItemFromSlot(SelectedSlotIndex);
            if (RetrievedItemFromSlot)
            {
                PickUpDropDown();
                P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에서 아이템 픽업"), SelectedSlotIndex);
            }
            else
            {
                P_LOG(PolluteLog, Error, TEXT("슬롯 %d에 아이템이 없습니다."), SelectedSlotIndex);
            }
        }
        else
        {
            P_LOG(PolluteLog, Error, TEXT("유효하지 않은 슬롯 인덱스입니다."));
        }
    }
}*/

void ALCU_PlayerCharacter::ServerRPC_Attack_Implementation()
{
    NetMulticast_Attack();
}

// NSK 사다리 설치 함수
void ALCU_PlayerCharacter::OnInstallLadder()
{
    if (bHasItem && ItemInHand)
    {
        // 설치 지점 확인
        for (TActorIterator<ANSK_LadderInstallPoint> It(GetWorld()); It; ++It)
        {
            ANSK_LadderInstallPoint* InstallPoint = *It;
            if (InstallPoint && InstallPoint->bPlayerIsNear)
            {
                // 인스톨 포인트에 사다리 액터 생성 추가 로직
                //InstallPoint->SetupInteraction();

                //InstallAndDeleteItem();

                //P_LOG(PolluteLog, Warning, TEXT("사다리가 설치되었습니다."));

                ServerInstallLadder(InstallPoint);
                break;
            }
        }
    }
}

void ALCU_PlayerCharacter::ServerInstallLadder_Implementation(ANSK_LadderInstallPoint* InstallPoint)
{
    if (InstallPoint)
    {
        InstallPoint->InstallLadder(this);
    }
}

// NSK 인스톨->드랍,삭제 함수
void ALCU_PlayerCharacter::InstallAndDeleteItem()
{
    // 아이템이 손에 있을 때만 드랍 처리
    if (bHasItem && ItemInHand)
    {
        ItemInHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        ItemInHand->Destroy();
        ItemInHand = nullptr;
        bHasItem = false;

        P_LOG(PolluteLog, Warning, TEXT("아이템을 손에서 해제했습니다."));
    }
}

// NSK 사다리 인터렉션 함수
void ALCU_PlayerCharacter::InteractWithLadder(const FInputActionValue& Value)
{
    // 입력 값을 불리언으로 가져옴 (Pressed 상태 확인)
    bool bIsPressed = Value.Get<bool>();

    if (bIsPressed)
    {
        if (HasAuthority()) // 서버에서 처리
        {
            Server_InteractWithLadder();
        }
        else
        {
            // 클라이언트가 서버에 요청
            Server_InteractWithLadder();
        }
    }
}

void ALCU_PlayerCharacter::Server_InteractWithLadder_Implementation()
{
    // 플레이어가 설치된 사다리와 상호작용
    FVector PlayerLocation = GetActorLocation();
    float ClosestDistance = 200.f; // 상호작용 거리
    ANSK_Ladder* ClosestLadder = nullptr;

    for (TActorIterator<ANSK_Ladder> It(GetWorld()); It; ++It)
    {
        ANSK_Ladder* Ladder = *It;
        if (Ladder && Ladder->bIsInstalled) // 설치된 사다리만 필터링
        {
            float Distance = FVector::Dist(PlayerLocation, Ladder->GetActorLocation());
            if (Distance < ClosestDistance)
            {
                ClosestLadder = Ladder;
                ClosestDistance = Distance;
            }
        }
    }

    if (ClosestLadder && ClosestLadder->TopPosition)
    {
        FVector TopLocation = ClosestLadder->TopPosition->GetComponentLocation();
        Multicast_InteractWithLadder(TopLocation); // 모든 클라이언트에 전파
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("근처에 설치된 사다리가 없습니다."));
    }
}

void ALCU_PlayerCharacter::Multicast_InteractWithLadder_Implementation(const FVector& TopLocation)
{
    SetActorLocation(TopLocation + FVector(0.f, 0.f, 50.f)); // 사다리 상단으로 이동
    P_LOG(PolluteLog, Warning, TEXT("사다리 맨 위로 이동 완료"));
}

// NSK 낙하산 인터렉션 함수
void ALCU_PlayerCharacter::InteractWithParachute()
{
    P_LOG(PolluteLog, Warning, TEXT("낙하산 탈출 호출"));

    if (bCanUseParachute && ItemInHand && ItemInHand->IsA<ANSK_Parachute>())
    {
        // 낙하산 액터를 사용 시 스펙터 모드로 전환

        // 캐릭터를 탈출 처리 상태로 설정
        if (ALCU_PlayerController* PlayerController = Cast<ALCU_PlayerController>(GetController()))
        {
            if (IsValid(ItemInHand))
            {
                if (HasAuthority())
                {
                    P_LOG(PolluteLog, Warning, TEXT("낙하산 액터 제거 전"));
                    
                    ItemInHand->Destroy();  // 낙하산 액터 제거
                    ItemInHand = nullptr;  // 참조를 초기화하여 안전하게 처리

                    P_LOG(PolluteLog, Warning, TEXT("낙하산 액터 제거 후"));
                }
            }

            if (PlayerController->IsLocalController())
            {
                // 시퀀스 파일을 로드 (경로 설정)
                ULevelSequence* Sequence = LoadObject<ULevelSequence>(nullptr, TEXT("LevelSequence'/Game/NSK/Sequence/Seq_Parachute.Seq_Parachute'"));

                if (Sequence)
                {
                    // 시퀀스를 재생할 Level Sequence Actor를 생성
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = this;
                    ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), SpawnParams);

                    if (SequenceActor)
                    {
                        // 시퀀스를 Actor에 설정
                        SequenceActor->SetSequence(Sequence);

                        // Level Sequence Player 생성
                        FMovieSceneSequencePlaybackSettings PlaybackSettings;
                        ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, PlaybackSettings, SequenceActor);

                        if (SequencePlayer)
                        {
                            P_LOG(PolluteLog, Warning, TEXT("시퀀스 실행 전 ItemInHand: %s"), *GetNameSafe(ItemInHand));

                            // 시퀀스 재생 시작
                            SequencePlayer->Play();
                            
                            // 시퀀스 재생이 시작된 후 스펙터 모드로 전환
                            PlayerController->ServerRPC_ChangeToSpector();

                            P_LOG(PolluteLog, Warning, TEXT("시퀀스 실행 후 ItemInHand: %s"), *GetNameSafe(ItemInHand));
                        }
                        else
                        {
                            P_LOG(PolluteLog, Warning, TEXT("시퀀스를 LevelSequenceActor에서 추출 실패"));
                        }
                    }
                    else
                    {
                        P_LOG(PolluteLog, Warning, TEXT("SequenceActor 생성 실패"));
                    }
                }
                else
                {
                    P_LOG(PolluteLog, Warning, TEXT("시퀀스를 찾을 수 없습니다."));
                }
            }
        }
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("낙하산을 사용할 수 없습니다."));
    }
}

void ALCU_PlayerCharacter::CanUseParachute(bool bCanUse)
{
    bCanUseParachute = bCanUse; // 낙하산 사용 가능 여부
    if (bCanUseParachute)
    {
        P_LOG(PolluteLog, Warning, TEXT("낙하산을 사용할 수 있습니다."));
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("낙하산을 사용할 수 없습니다."));
    }
}

void ALCU_PlayerCharacter::ServerDestroyParachute_Implementation(ANSK_Parachute* Parachute)
{
    P_LOG(PolluteLog, Warning, TEXT("서버에서 낙하산 제거 요청"));

    if (IsValid(Parachute))
    {
        P_LOG(PolluteLog, Warning, TEXT("낙하산 유효, 삭제 진행"));
        Parachute->Destroy();

        // 모든 클라에게 낙하산 삭제를 얼려주는 멀티 캐스트
        MulticastDestroyParachute(Parachute);
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("낙하산이 유효하지 않음"));
    }
}

bool ALCU_PlayerCharacter::ServerDestroyParachute_Validate(ANSK_Parachute* Parachute)
{
    return true; // 간단한 유효성 검사를 추가할 수 있음.
}

void ALCU_PlayerCharacter::MulticastDestroyParachute_Implementation(ANSK_Parachute* Parachute)
{
    if (IsValid(Parachute))
    {
        // 클라에서 낙하산 객체 삭제
        Parachute->Destroy();
    }
}

// NSK 탈출 포트 함수
void ALCU_PlayerCharacter::PortEscape()
{
    P_LOG(PolluteLog, Warning, TEXT("%s: OnEscape 호출"), *GetName());

    ALCU_PlayerController* PlayerController = Cast<ALCU_PlayerController>(GetController());
    if (!PlayerController)
    {
        P_LOG(PolluteLog, Warning, TEXT("PlayerController 캐스팅 실패"));
        return;
    }

    // 스펙터 상태 전환
    PlayerController->ServerRPC_ChangeToSpector();

    // 시퀀스 재생
    PlayEscapeSequence();
}

void ALCU_PlayerCharacter::PlayEscapeSequence()
{
    ULevelSequence* Sequence = LoadObject<ULevelSequence>(nullptr, TEXT("LevelSequence'/Game/NSK/Sequence/Seq_Parachute.Seq_Parachute'"));
    if (Sequence)
    {
        ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), nullptr);
        if (SequencePlayer)
        {
            SequencePlayer->Play();
        }
    }
}