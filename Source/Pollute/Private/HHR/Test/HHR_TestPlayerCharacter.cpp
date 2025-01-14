// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/Test/HHR_TestPlayerCharacter.h"

#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "HHR/HHR_Knife.h"
#include "HHR/HHR_WeaponItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LCU/Player/LCU_PlayerCharacter.h"


class UEnhancedInputComponent;
// Sets default values
AHHR_TestPlayerCharacter::AHHR_TestPlayerCharacter()
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
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHHR_TestPlayerCharacter::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHHR_TestPlayerCharacter::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void AHHR_TestPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TraceHandle, this, &AHHR_TestPlayerCharacter::ShootTrace, 0.2f, true);
}

void AHHR_TestPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
	
	Super::EndPlay(EndPlayReason);
	
}

void AHHR_TestPlayerCharacter::UpdateCameraTransform()
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
void AHHR_TestPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateCameraTransform();
	FinalOverapPlayer = Cast<ALCU_PlayerCharacter>(GetClosestActorToCamera(OverlappingPlayers));
	FinalOverapItem = GetClosestActorToCamera(OverlappingItems);
}

// Called to bind functionality to input
void AHHR_TestPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_CarryCurse, ETriggerEvent::Started, this, &AHHR_TestPlayerCharacter::CarryCurse);
		EnhancedInputComponent->BindAction(IA_PickUpDropDown, ETriggerEvent::Started, this, &AHHR_TestPlayerCharacter::PickUpDropDown);

		// ## 추가
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AHHR_TestPlayerCharacter::Attack);
	}
}

void AHHR_TestPlayerCharacter::Interact()
{
	
}

AActor* AHHR_TestPlayerCharacter::GetClosestActorToCamera(const TSet<AActor*>& Actors)
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

void AHHR_TestPlayerCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// 충돌 채널 가져오기
		ECollisionChannel ObjectType = OtherComp->GetCollisionObjectType();

		// 채널에 따른 처리
		switch (ObjectType)
		{
		case ECC_Pawn: 
			AddOverlappingPlayer(OtherActor);
			break;
		case ECC_GameTraceChannel1:
			AddOverlappingItem(OtherActor);
			break;
		default:
			break;
		}
	}
}

void AHHR_TestPlayerCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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

void AHHR_TestPlayerCharacter::AddOverlappingPlayer(AActor* otherActor)
{
	if(!OverlappingPlayers.Contains(otherActor))
	{
		OverlappingPlayers.Add(otherActor);
	}
}

void AHHR_TestPlayerCharacter::RemoveOverlappingPlayer(AActor* otherActor)
{
	if(OverlappingPlayers.Contains(otherActor))
	{
		OverlappingPlayers.Remove(otherActor);
	}
}

void AHHR_TestPlayerCharacter::AddOverlappingItem(AActor* otherActor)
{
	if(!OverlappingItems.Contains(otherActor))
	{
		OverlappingItems.Add(otherActor);
	}
}

void AHHR_TestPlayerCharacter::RemoveOverlappingItem(AActor* otherActor)
{
	if(OverlappingItems.Contains(otherActor))
	{
		OverlappingItems.Remove(otherActor);
	}
}

void AHHR_TestPlayerCharacter::CarryCurse()
{
	// 저주가 없으면 줄 것도 없으니 나가세요
	if(!bHasCurse) return;

	// 현재 눈 앞에 줄 수 있는 플레이어도 없으니 나가세요.
	if(!FinalOverapPlayer) return;

	
	
	
}

void AHHR_TestPlayerCharacter::PickUpDropDown()
{
	// 주울 수 있는 아이템이 없으면 나가야함
	if(!FinalOverapItem) return;
	
	// 현재 아이템이 없으니 픽업
	if(!bHasItem)
	{		
		USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
		if (!SkeletalMeshComp)
		{
			return;
		}
	
		FinalOverapItem->AttachToComponent(
				SkeletalMeshComp,                      
				FAttachmentTransformRules::SnapToTargetIncludingScale, 
				FName("PickUpSocket")                   
			);
			bHasItem = true;
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
	}	
}

void AHHR_TestPlayerCharacter::ShootTrace()
{
	if(!IsLocallyControlled()) return;
	// 카메라 위치와 방향 가져오기
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 트레이스 시작점과 끝점 계산
	FVector Start = CameraLocation;
	FVector End = Start + CameraRotation.Vector() * 1000.0f; // 카메라 방향으로 1000 단위 거리

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

		ILCU_InteractInterface* InteractInterface = Cast<ILCU_InteractInterface>(HitActor);
		if(InteractInterface)
		{
			InteractInterface->Interact();
		}
	}
}

void AHHR_TestPlayerCharacter::Attack()
{
	// Item 구하는 코드는 나중에 처리님이 들고 있는 아이템 추가하면 없애도 될듯
	AHHR_WeaponItem* weapon = Cast<AHHR_WeaponItem>(FinalOverapItem);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Attack Click"));
	
	// Item 종류(칼, 총)에 따라 다른 Montage 실행
	if(weapon)
	{
		AHHR_Knife* knife = Cast<AHHR_Knife>(weapon);
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if(animInstance)
		{
			if(knife)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Montage play"));
				animInstance->Montage_Play(KnifeAttackMontage, 1.0f);
			}
			else
			{
				animInstance->Montage_Play(GunAttackMontage, 1.0f);
			}
		}
	}
	
}



