// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

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