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


	//����ӳ�����Ծ�� - һ���Ե�  - �ؼ��� -IE_Pressed - IE_Released - ����/�ɿ���
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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
	if ((Controller != nullptr) && (Value != 0.0f))
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