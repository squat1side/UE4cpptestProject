// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����Camera Boom ��������ײʱ��������ң�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	//���õ��ɱ� - ���������������һ����
	CameraBoom->TargetArmLength = 600.f;
	//��ת���ڿ����� - ����������Pawn��
	CameraBoom->bUsePawnControlRotation = true;

	//Ϊ��ײ�������趨��С
	GetCapsuleComponent()->SetCapsuleSize(25.f, 74.f);


	//����Follow�����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//����Attach��Attach�������õ�RootComponent�������Ҫ���ӵ�CameraBoom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//����������ӵ�Boomĩ�ˣ�����Pawn���е�������ʵ��ƥ��������ķ���
	//ֻ����������ӵ�CameraBoom��������CameraBoom�ƶ����������ڿ�����
	FollowCamera->bUsePawnControlRotation = false;

	//Ϊ��������ת������
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//��������ת��ʱ��Ҫ��ת
	//��Ӱ�������
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//���ý�ɫ�ƶ�
	GetCharacterMovement()->bOrientRotationToMovement = true;//��ɫ���Ż�������ķ����ƶ�
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);//���ڴ���ת����
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//����Player StatsĬ����ֵ
	MaxHealth = 100.f;
	Health = 65.f;
	MaxStamina = 350.f;
	Stamina = 120.f;
	Coins = 0;

	//��ʼ��Running��SpringSpeed����ֵ
	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	//bShiftKeyDown
	bShiftKeyDown = false;

	//bLMBDown
	bLMBDown = false;

	//Stamina - ��ʼ��Enum
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	//��ʼ�����������ʺ���С�������ֵ
	StaminaDrainRate = 25.f;
	MinSprintStamina = 75.f;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ʹ�ñ�����ʵ����������
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	//ʵ��StaminaStatus������״̬
	switch (StaminaStatus)
	{
		//ʵ��Normal״̬���
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);//��ƣ��״̬�ָ���
		}
		//ShiftKeyUp - �ж������Ƿ�ָ�
		else
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

		//ʵ��BelowMinimum״̬���
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		//shift key up
		else
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

		//ʵ��Exhausted״̬���
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		//shift key up
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		//��Exhausted״̬����sprinting
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

		//ʵ��ExhaustedRecovering״̬���
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);


	//����ӳ�����Ծ�� - һ���Ե�  - �ؼ��� -IE_Pressed - IE_Released - ����/�ɿ���
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//����ӳ��ĳ�̰� - һ���Ե�  - �ؼ��� - IE_Pressed - IE_Released - ����/�ɿ���
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	//����ӳ��ĳ�̰� - һ���Ե�  - �ؼ��� - IE_Pressed - IE_Released - ����/�ɿ���
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	//Ϊ�������
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	//Ϊ������
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
	

}

void AMain::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!bAttacking))
	{
		//�ҳ�ǰ��
		const FRotator Rotation = Controller->GetControlRotation();//����������ת
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//������ǰʸ�� - RotationMatrix��������ȡ�ض������ʸ��������actor��������������겻ͳһʱ������������ǰʸ��
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AMain::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!bAttacking))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void AMain::TurnAtRate(float Rate)
{
	float a = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	float b = FMath::Clamp(a, -1.0f, 1.0f);
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void AMain::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//����LMBDown/Up
void AMain::LMBDown()
{
	bLMBDown = true;
	//���������Ӹ��ӵ�HandSocket�ȵط�
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	//���ù���
	else if (EquippedWeapon)
	{
		Attack();
	}
}
void AMain::LMBUp()
{
	bLMBDown = false;
}



//����:DecrementHealth
void AMain::DecrementHealth(float Amount)
{
	//�жϽ�ɫ�Ƿ�Ӧ��������Ѫ������
	if (Health - Amount <= 0.f)
	{
		Health += Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

//����IncrementCoins
void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}


//����Die
void AMain::Die()
{

}

//����Setter - MovementStatus
void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	//�жϽ�����
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}


//Shift���º�������
void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

//Shift�ɿ���������
void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

//ʵ��ShowPickupLocations����
void AMain::ShowPickupLocations()
{
	//��������ռ���Ӳ��ÿһ����λ��
	for (auto Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, .5f);
	}
}

//����һ������װ��������Setter
void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

//����Attack����
void AMain::Attack()
{
	if (!bAttacking)
	{
		bAttacking = true;

		//��ȡAnimʵ��
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//�ж�Animʵ���Ƿ���Ч
		if (AnimInstance && CombatMontage)
		{
			//���ö��ֹ��������л�
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				//Ӧ��Montage Play������������̫�� -�����ٶ�Ϊ2.2����
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				//�ڲ�����̫��ʱ����ζ����ʲ��ض�����ĳһƬ��
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				//Ӧ��Montage Play������������̫�� -�����ٶ�Ϊ2.2����
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				//�ڲ�����̫��ʱ����ζ����ʲ��ض�����ĳһƬ��
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;

			default:
				;
			}
		}
	}
}


//����AttackEnd����
void AMain::AttackEnd()
{
	bAttacking = false;
	//��סLMB���Գ�������
	if (bLMBDown)
	{
		Attack();
	}
}





































