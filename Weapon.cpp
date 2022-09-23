// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SKeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SKeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//创建Weapon构造函数
AWeapon::AWeapon()
{
	//完善SkeletalMesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKeletalMesh"));
	//附加到根组件
	SkeletalMesh->SetupAttachment(GetRootComponent());

	//bWeaponParticles
	bWeaponParticles = false;

	//WeaponState设置默认值
	WeaponState = EWeaponState::EWS_Pickup;

}

//创建OnOverlapBegin函数
void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//调用Super来调用父级版本
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//将Otherctor也装备起来 - 可以装备角色
	if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(this);
		}
	}

}

//创建OnOverlapEnd函数
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

//实现角色装备武器  - Equip函数
void AWeapon::Equip(AMain* Char)
{
	if (Char)
	{
		//调整忽略武器对角色和摄像机之间的影响，不会影响玩家角度的放大
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		//将CollisionResponseToChannel设置成被Pawn忽略 -不发生与Pawn碰撞
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//停止物理模拟
		SkeletalMesh->SetSimulatePhysics(false);

		//获取RightHandSocket
		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
		//判定RightHnadSocket是否有效 - 有效则可以附加到角色身上
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			//设置Weapon在装备人物时候不需要ratate
			bRotate = false;


			//将EquippedWeapon设定成特定武器的实例
			Char->SetEquippedWeapon(this);
			//将SetActiveOverlappingOverItem设为nullptr
			Char->SetActiveOverlappingItem(nullptr);
		}
		//判定EquipSound来调用UGmaeplayStatics
		if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
		//判断如果拾取了武器，可以判定Weapon Particles是true还是false
		if (!bWeaponParticles)
		{
			IdelParticlesComponent->Deactivate();
		}
	}
}

