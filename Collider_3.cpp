// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"



// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (MeshComponentAsset.Succeeded())//�᷵��һ������ֵ - Ture or False  MeshComponent
//- ture �Ļ� - ConstructorHelpers::FObjectFinder���ǳɹ��ģ��������·���ҵ�StaticMesh������������MeshComponent
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);//�����ͨ�����������Mesh�ķ���������������ͼ����ѡ��
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));//���빹�캯��FVrctor��Ȼ�󴴽�һ����ʱ��FVector;
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));//ʹXYZ��ᰴ0.8����λ�������ţ�

	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;//���ɱ�
	SpringArm->bEnableCameraLag = true;//����ֵbEnableCameraLag- Ϊ��CameraLag�ͻ����Pawn���棻
	SpringArm->CameraLagSpeed = 3.0f;//����Pwan��cameralag���ٶȳ�ʼֵ

	//����Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);//����������ӵ����ɱ���

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	
	//����һ����ʼ��ֵ
	CameraInput = FVector2D(0.f, 0.f);

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

	FRotator NewRotation = GetActorRotation();
	//����Yaw - ���������ת
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	//ΪSpringArm������ת
	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	//���Ƶ��ɱ۵���ת��Χ
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);

	SpringArm->SetWorldRotation(NewSpringArmRotation);

}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
}

//����ACollider::����ȫ����MoveForward��MoveRight
void ACollider::MoveForward(float input)
{
	FVector Forward = GetActorForwardVector();
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(input * Forward);
	}
}

void ACollider::MoveRight(float input)
{
	FVector Right = GetActorRightVector();
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(input * Right);
	}
}

void ACollider::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void ACollider::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}



//��Ҫ��ȫ�޶��������
UPawnMovementComponent* ACollider::GetMovementComponent()const
{
	//�����Զ����OurMovementComponent
	return OurMovementComponent;
}

