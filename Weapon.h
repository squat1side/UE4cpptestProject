// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


//��ӻ������������õ�״̬���ڶ�ս - �����򻯵�enum
UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	//����pickup��Equipped״̬
	EWS_Pickup UMETA(DisplayName = "Pickip"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	//����һ��Ĭ��ֵ
	EWS_MAX UMETA(DisplayName = "DefaultMax")
};



/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:

	AWeapon();

	//��Weaponһ��WeaponState����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState;


	//����һ������ֵ - ������һ��Weapon Particle������������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bWeaponParticles;

	//������������ - ��װ������ʱ��ʾ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	//�������ʲ��ǹ���������� - ����USkeletalMeshComponent -����һ�������������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	//����OnOverlapBegin����
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	//����OnOverlapEnd����
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	//����Equip����ʵ������װ����ɫ
	void Equip(class AMain* Char);


	//��������WeaponState������Setter��getter
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

};
