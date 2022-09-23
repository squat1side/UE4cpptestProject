// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SKeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SKeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//����Weapon���캯��
AWeapon::AWeapon()
{
	//����SkeletalMesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKeletalMesh"));
	//���ӵ������
	SkeletalMesh->SetupAttachment(GetRootComponent());

	//bWeaponParticles
	bWeaponParticles = false;

	//WeaponState����Ĭ��ֵ
	WeaponState = EWeaponState::EWS_Pickup;

}

//����OnOverlapBegin����
void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����Super�����ø����汾
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//��OtherctorҲװ������ - ����װ����ɫ
	if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(this);
		}
	}

}

//����OnOverlapEnd����
void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(nullptr);
		}
	}
}

//ʵ�ֽ�ɫװ������  - Equip����
void AWeapon::Equip(AMain* Char)
{
	if (Char)
	{
		//�������������Խ�ɫ�������֮���Ӱ�죬����Ӱ����ҽǶȵķŴ�
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		//��CollisionResponseToChannel���óɱ�Pawn���� -��������Pawn��ײ
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//ֹͣ����ģ��
		SkeletalMesh->SetSimulatePhysics(false);

		//��ȡRightHandSocket
		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
		//�ж�RightHnadSocket�Ƿ���Ч - ��Ч����Ը��ӵ���ɫ����
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			//����Weapon��װ������ʱ����Ҫratate
			bRotate = false;


			//��EquippedWeapon�趨���ض�������ʵ��
			Char->SetEquippedWeapon(this);
			//��SetActiveOverlappingOverItem��Ϊnullptr
			Char->SetActiveOverlappingItem(nullptr);
		}
		//�ж�EquipSound������UGmaeplayStatics
		if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
		//�ж����ʰȡ�������������ж�Weapon Particles��true����false
		if (!bWeaponParticles)
		{
			IdelParticlesComponent->Deactivate();
		}
	}
}

