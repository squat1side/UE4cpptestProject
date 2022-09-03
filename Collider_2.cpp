// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (MeshComponentAsset.Succeeded())//会返回一个布尔值 - Ture or False  MeshComponent
//- ture 的话 - ConstructorHelpers::FObjectFinder就是成功的，就在这个路径找到StaticMesh，接下来输入MeshComponent
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);//这便是通过编码来获得Mesh的方法，而不是在蓝图里面选；
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));//插入构造函数FVrctor，然后创建一个临时的FVector;
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));//使XYZ轴会按0.8个单位进行缩放；

	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;//弹簧臂
	SpringArm->bEnableCameraLag = true;//布尔值bEnableCameraLag- 为真CameraLag就会紧跟Pawn后面；
	SpringArm->CameraLagSpeed = 3.0f;//紧跟Pwan的cameralag的速度初始值

	//创建Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);//将摄像机附加到弹簧臂上

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

}

//利用ACollider::来完全限制MoveForward和MoveRight
void ACollider::MoveForward(float input)
{
	FVector Forward = GetActorForwardVector();
	AddMovementInput(input * Forward);
}

void ACollider::MoveRight(float input)
{
	FVector Right = GetActorRightVector();
	AddMovementInput(input * Right);
}