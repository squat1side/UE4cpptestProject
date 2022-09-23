// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


//添加基于武器所设置的状态用于对战 - 创建域化的enum
UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	//设置pickup和Equipped状态
	EWS_Pickup UMETA(DisplayName = "Pickip"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	//设置一个默认值
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

	//给Weapon一个WeaponState变量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState;


	//设置一个布尔值 - 来创建一个Weapon Particle武器粒子属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bWeaponParticles;

	//创建声音变量 - 在装备武器时提示
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	//在武器资产是骨骼网格情况 - 声明USkeletalMeshComponent -创建一个骨骼网格组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	//创建OnOverlapBegin函数
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	//创建OnOverlapEnd函数
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	//创建Equip函数实现武器装备角色
	void Equip(class AMain* Char);


	//创建用于WeaponState变量的Setter和getter
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

};
