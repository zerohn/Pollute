// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_PlayerCharacter.h"

#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "HHR/HHR_WeaponItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LCU/InteractActors/LCU_Curse.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMesh.h"
#include "HHR/UI/HHR_PlayerHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHR/UI/HHR_TestPlayerHUD.h"
#include "LCU/Player/LCU_TestWidget.h"
#include "P_Settings/P_GameState.h"


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

    if(LCU_TestWidgetFactory && IsLocallyControlled())
    {
        LCU_TestWidget = CreateWidget<ULCU_TestWidget>(GetWorld()->GetFirstPlayerController(),LCU_TestWidgetFactory);
        if(LCU_TestWidget)
        {
            LCU_TestWidget->AddToViewport();
            LCU_TestWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

	GetWorld()->GetTimerManager().SetTimer(TraceHandle, this, &ALCU_PlayerCharacter::ShootTrace, 0.2f, true);
}

void ALCU_PlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
	
	Super::EndPlay(EndPlayReason);
	
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

	//DrawDebugBox(
	//	GetWorld(),
	//	BoxComponent->GetComponentLocation(),
	//	BoxComponent->GetScaledBoxExtent(),
	//	BoxComponent->GetComponentQuat(), // 박스 회전
	//	FColor::Blue,
	//	false,  // 지속 표시 (Tick마다 새로 그림)
	//	1.f, // 1초 동안 유지
	//	0,     // 디버그 선 우선순위
	//	5.0f   // 선 두께
	//);
}

// Called every frame
void ALCU_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateCameraTransform();
	FinalOverapPlayer = Cast<ALCU_PlayerCharacter>(GetClosestActorToCamera(OverlappingPlayers));
	FinalOverapItem = GetClosestActorToCamera(OverlappingItems);
    RetrievedItem = Cast<AHHR_Item>(FinalOverapItem);

    if(StartCurseCool)
    {
        if(IsLocallyControlled())
        {
            CarryCurseCool -= DeltaTime;
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
        EnhancedInputComponent->BindAction(IA_G, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::OnInteract);
	    EnhancedInputComponent->BindAction(IA_RunToggle, ETriggerEvent::Started, this, &ALCU_PlayerCharacter::RunShiftToggle);
	}
}

void ALCU_PlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ALCU_PlayerCharacter, HealthCount);
    DOREPLIFETIME(ALCU_PlayerCharacter, ItemInHand);
    DOREPLIFETIME(ALCU_PlayerCharacter, bHasCurse);
    DOREPLIFETIME(ALCU_PlayerCharacter, bIsRunning);
    
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
       FString aa = bIsRunning ? "true" : "false";
       P_SCREEN(1.f, FColor::Black, TEXT("%s"), *aa);
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
	if (Actors.Num() == 0) return nullptr;

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

	// 현재 눈 앞에 줄 수 있는 플레이어도 없으니 나가세요.
	if(!FinalOverapPlayer) return;

	ServerRPC_CarryCurse();
}


void ALCU_PlayerCharacter::ServerRPC_CarryCurse_Implementation()
{
	// NetMulticast_CarryCurse();
    // 저주를 옮겨요
    FinalOverapPlayer->SetHasCurse(true);
    FinalOverapPlayer->ClientRPC_HasCurseWidget(true);
    bHasCurse = false;
    ClientRPC_HasCurseWidget(false);
    ALCU_Curse::GetInstance(GetWorld())->SetCharacter(FinalOverapPlayer);

    // 이제 본인이 가지고 있던 FinalOverap 후보들을 전부 삭제해요
    FinalOverapPlayer = nullptr;
    if(!OverlappingPlayers.IsEmpty())
    {
        OverlappingPlayers.Empty();
    }
}

void ALCU_PlayerCharacter::NetMulticast_CarryCurse_Implementation()
{
	// 저주를 옮겨요
	FinalOverapPlayer->SetHasCurse(true);
	bHasCurse = false;
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
    /*if (!RetrievedItem) // Null 수정
    {
        P_LOG(PolluteLog, Error, TEXT("픽업할 아이템이 없습니다."));
        return;
    }

    if (!bHasItem) // 아이템이 없는 경우 픽업
    {
        USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
        if (!SkeletalMeshComp)
        {
            P_LOG(PolluteLog, Error, TEXT("스켈레탈 메시가 없습니다."));
            return;
        }

        // 아이템 손에 부착
        ItemInHand = RetrievedItem;
        ItemInHand->AttachToComponent(
            SkeletalMeshComp,
            FAttachmentTransformRules::SnapToTargetIncludingScale,
            FName("PickUpSocket")
        );

        P_SCREEN(1.f, FColor::Black, TEXT("아이템 픽업 성공"));
        bHasItem = true;

        // 아이템 위치 및 회전 설정
        ItemInHand->SetActorRelativeLocation(ItemInHand->ItemData.ItemLocation);
        ItemInHand->SetActorRelativeRotation(ItemInHand->ItemData.ItemRotation);
        ItemInHand->SetOwner(this);

        // 상태 초기화
        RetrievedItem = nullptr;
    }
    else // 아이템이 있는 경우 드랍
    {
        FVector CharacterLocation = GetActorLocation();
        FVector DropLocation = CharacterLocation - FVector(0.0f, 0.0f, 90.0f);
        FRotator DropRotation = GetActorRotation();

        // 아이템 부모-자식 관계 해제 및 위치 설정
        ItemInHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        ItemInHand->SetActorLocation(DropLocation);
        ItemInHand->SetActorRotation(DropRotation);

        P_SCREEN(1.f, FColor::Black, TEXT("아이템 드랍 완료"));

        // 상태 초기화
        ItemInHand = nullptr;
        bHasItem = false;
    }


    //주울 수 있는 아이템이 없으면 나가야함
    if (!FinalOverapItem) return;
	
	// 현재 아이템이 없으니 픽업

	if(!bHasItem)
	{		
		USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
		if (!SkeletalMeshComp)
		{
			return;
		}
*/


    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Picked up Drop down"));
    //ServerRPC_PickUpDropDown();
    
    ServerRPC_PickUpDropDown();
}

void ALCU_PlayerCharacter::ServerRPC_PickUpDropDown_Implementation()
{
    // 주울 수 있는 아이템이 없으면 나가야함
    if(!FinalOverapItem) return;
    
    //P_LOG(PolluteLog, Warning, TEXT("PickUp"))
    // 현재 아이템이 없으니 픽업
    if(!ItemInHand)
    {

        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PickUpDropDown executed on Server"));
        
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
    // multi에서 할 일을 최대한 줄여야할 듯 
    ItemInHand = Cast<AHHR_Item>(FinalOverapItem);
    AttachItem();
}


void ALCU_PlayerCharacter::AttachItem()
{
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
        //P_LOG(PolluteLog, Warning, TEXT("Item In Hand!"))s
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
        
        // UI 변경
        if(PlayerHUD)
        {
            PlayerHUD->ChangeItemImage(ItemInHand->ItemData.ItemImage);
        }
        
    }
}

void ALCU_PlayerCharacter::NetMulticast_DetachItem_Implementation()
{
    DetachItem();
    /*if(ItemInHand)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, (TEXT("%s Item 있음"), *this->GetName()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, (TEXT("%s Item 없음"), *this->GetName()));
        return;
    }
    

    if(!ItemInHand) return;
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
    FinalOverapItem = nullptr;
    bHasItem = false;

    // Drop 후에 핸드에 있는 아이템 null 초기화
    ItemInHand = nullptr;

    // UI 변경
    if(PlayerHUD)
    {
        PlayerHUD->ChangeItemImageNull();
    }*/
}

void ALCU_PlayerCharacter::DetachItem()
{
    if (!ItemInHand) return;
    // Item의 Interactive 허용
    if(IsLocallyControlled())
    {
        ItemInHand->GetItemInteractWidgetComponent()->SetVisibility(true);
    }
    
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

    // Drop 후에 핸드에 있는 아이템 null 초기화
    ItemInHand = nullptr;

    // UI 변경 
    if(PlayerHUD)
    {
        PlayerHUD->ChangeItemImageNull();
    }
    
    /*
	    // HHR 수정 
	    ItemInHand = Cast<AHHR_Item>(FinalOverapItem);
	    if(ItemInHand)
	    {
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
	    }
	}
	// 아이템을 가지고 있으니 드랍다운
	else
	{

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
}

		DropDown();
	}
  */
}


void ALCU_PlayerCharacter::ShootTrace()
{
	if(!IsLocallyControlled()) return;
	// 카메라 위치와 방향 가져오기
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 트레이스 시작점과 끝점 계산
	FVector Start = CameraLocation;
	FVector End = Start + CameraRotation.Vector() * 1000.0f; // 카메라 방향으로 1000 단위 거리
    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.2f);

	//  박스 크기 설정
	FVector BoxHalfSize = FVector(10.0f, 10.0f, 50.0f); // 박스 크기 (너비 20, 높이 100)

	//  트레이스할 객체 유형 설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); 
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));        // Pawn (캐릭터)
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));        // Pawn (캐릭터)
	
	//  무시할 액터 설정
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this); // 자신 제외

	FHitResult HitResult;

	//  BoxTraceSingleForObjects 호출
	bool bHit = UKismetSystemLibrary::BoxTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		BoxHalfSize,
		CameraRotation,
		ObjectTypes,
		false,        
		IgnoredActors,  
		EDrawDebugTrace::None, 
		HitResult,
		true,           
		FLinearColor::Red,   
		FLinearColor::Green, 
		5.0f
	);

	if(bHit)
	{
		AActor* HitActor = HitResult.GetActor();


		//ILCU_InteractInterface* InteractInterface = Cast<ILCU_InteractInterface>(HitActor);
		//if(InteractInterface)
		//{
		//	InteractInterface->Interact();
		//}

	}
}


void ALCU_PlayerCharacter::DieProcess()
{
    ALCU_PlayerController* pc = Cast<ALCU_PlayerController>(GetController());
    if(pc && IsLocallyControlled())
    {
        HasCurseWidget(false);
        pc->ServerRPC_ChangeToSpector();
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
    // hud 변경
    if(PlayerHUD)
    {
        PlayerHUD->ChangeItemImageNull();
    }
    ItemInHand = nullptr;
}

void ALCU_PlayerCharacter::HasCurseWidget(bool bShow)
{
    if(!IsLocallyControlled()) return;


    if(bShow)
    {
        if(LCU_TestWidget)
        {
            LCU_TestWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if(LCU_TestWidget)
        {
            LCU_TestWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void ALCU_PlayerCharacter::ClientRPC_HasCurseWidget_Implementation(bool bShow)
{
    if(bShow)
    {
        if(LCU_TestWidget)
        {
            LCU_TestWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if(LCU_TestWidget)
        {
            LCU_TestWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
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
void ALCU_PlayerCharacter::OnInteract()
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
}

void ALCU_PlayerCharacter::ServerRPC_Attack_Implementation()
{
    NetMulticast_Attack();
}

