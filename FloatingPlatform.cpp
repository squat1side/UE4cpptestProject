// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//��ʼ��BeginPlay�󣬸�����FVectorһЩĬ����ֵ
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	//��ʼ��bInterping
	bInterping = false;

	//��ʼ��InterpSpeed - ��������Floating�ƶ��ٶ�
	InterpSpeed = 2.0f;

	//��ʼ����ʱ��ʱ�����InterpTime
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	
	//�����յ�λ�� - Endpoint������FloatingPlatform�ľֲ�������
	EndPoint += StartPoint;

	//������ֵ��ΪFalse
	bInterping = false;

	//���ü�ʱ�� - WorldTimerMamager
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	//��ʼ�������յ�ľ��� -�������յ�󷵻ص���һ��Vector
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����Ƿ��в�ֵ
	if (bInterping)
	{
		//�����������յ���float��Ư�� - ʹ�ò�ֵ
        //��ȡ�����ÿһ֡��λ��
		FVector CurrentLocation = GetActorLocation();

		//��ȡλ�ú����Բ�ֵ - V - Vector; InterpTo��ʾ���뵽3D�ռ�ָ��λ��
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		//����Interp���趨�������յ��λ�� - Interp�����趨��λ��
		SetActorLocation(Interp);

		//�����ƶ��ľ��벢�ж�λ��
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 1.f)
		{
			ToggleInterping();

			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			//�滻�����յ��λ��
			SwapVectors(StartPoint, EndPoint);
		}
	}
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

//д��Swap���������� - ����һ����ʱ����Temp
void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}

















