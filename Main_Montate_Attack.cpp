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

	//创建Camera Boom （出现碰撞时，推向玩家）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	//设置弹簧臂 - 并将摄像机跟随这一方向
	CameraBoom->TargetArmLength = 600.f;
	//旋转基于控制器 - 控制器属于Pawn类
	CameraBoom->bUsePawnControlRotation = true;

	//为碰撞胶囊体设定大小
	GetCapsuleComponent()->SetCapsuleSize(25.f, 74.f);


	//创建Follow摄像机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//设置Attach，Attach不是设置到RootComponent，摄像机要附加到CameraBoom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//将摄像机附加到Boom末端，并让Pawn进行调整，来实现匹配控制器的方向
	//只将摄像机附加到CameraBoom，并跟随CameraBoom移动，不依赖于控制器
	FollowCamera->bUsePawnControlRotation = false;

	//为输入设置转向速率
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//控制器旋转的时候不要旋转
	//仅影响摄像机
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//设置角色移动
	GetCharacterMovement()->bOrientRotationToMovement = true;//角色沿着基于输入的方向移动
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);//基于此旋转速率
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//设置Player Stats默认数值
	MaxHealth = 100.f;
	Health = 65.f;
	MaxStamina = 350.f;
	Stamina = 120.f;
	Coins = 0;

	//初始化Running和SpringSpeed并赋值
	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	//bShiftKeyDown
	bShiftKeyDown = false;

	//bLMBDown
	bLMBDown = false;

	//Stamina - 初始化Enum
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	//初始化体力消耗率和最小冲刺体力值
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

	//使用比率来实现消耗体力
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	//实现StaminaStatus的四种状态
	switch (StaminaStatus)
	{
		//实现Normal状态情况
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
			SetMovementStatus(EMovementStatus::EMS_Sprinting);//在疲惫状态恢复中
		}
		//ShiftKeyUp - 判定体力是否恢复
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

		//实现BelowMinimum状态情况
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

		//实现Exhausted状态情况
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
		//在Exhausted状态不会sprinting
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

		//实现ExhaustedRecovering状态情况
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


	//动作映射的跳跃绑定 - 一次性的  - 关键词 -IE_Pressed - IE_Released - 按下/松开键
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//动作映射的冲刺绑定 - 一次性的  - 关键词 - IE_Pressed - IE_Released - 按下/松开键
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	//动作映射的冲刺绑定 - 一次性的  - 关键词 - IE_Pressed - IE_Released - 按下/松开键
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	//为键盘添加
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	//为鼠标添加
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
	

}

void AMain::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!bAttacking))
	{
		//找出前方
		const FRotator Rotation = Controller->GetControlRotation();//控制器的旋转
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//创建向前矢量 - RotationMatrix是用来获取特定方向的矢量，例如actor的坐标和世界坐标不统一时；控制器的向前矢量
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

//创建LMBDown/Up
void AMain::LMBDown()
{
	bLMBDown = true;
	//将武器附加附加到HandSocket等地方
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	//设置攻击
	else if (EquippedWeapon)
	{
		Attack();
	}
}
void AMain::LMBUp()
{
	bLMBDown = false;
}



//创建:DecrementHealth
void AMain::DecrementHealth(float Amount)
{
	//判断角色是否应该死亡及血条减少
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

//创建IncrementCoins
void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}


//创建Die
void AMain::Die()
{

}

//创建Setter - MovementStatus
void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	//判断进入冲刺
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}


//Shift按下函数主体
void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

//Shift松开函数主体
void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

//实现ShowPickupLocations功能
void AMain::ShowPickupLocations()
{
	//标记所有收集的硬币每一个的位置
	for (auto Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, .5f);
	}
}

//创建一个设置装备武器的Setter
void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

//设置Attack函数
void AMain::Attack()
{
	if (!bAttacking)
	{
		bAttacking = true;

		//获取Anim实例
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//判定Anim实例是否有效
		if (AnimInstance && CombatMontage)
		{
			//设置多种攻击类型切换
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				//应用Montage Play函数来播放蒙太奇 -播放速度为2.2倍速
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				//在播放蒙太奇时，多段动画资产特定跳至某一片段
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				//应用Montage Play函数来播放蒙太奇 -播放速度为2.2倍速
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				//在播放蒙太奇时，多段动画资产特定跳至某一片段
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;

			default:
				;
			}
		}
	}
}


//创建AttackEnd函数
void AMain::AttackEnd()
{
	bAttacking = false;
	//按住LMB可以持续攻击
	if (bLMBDown)
	{
		Attack();
	}
}





































