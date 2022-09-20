// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);


	//操作映射的跳跃绑定 - 一次性的  - 关键词 -IE_Pressed - IE_Released - 按下/松开键
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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
	if ((Controller != nullptr) && (Value != 0.0f))
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
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}


void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void AMain::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}