// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"



//����ö��Enum - ���ϵͳ
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayNmae = "Normal"),
	//�������״̬ - Sprinting��enum
	EMS_Sprinting UMETA(DisplayNmae = "Sprinting"),
	//����Dead״̬
	EMS_Dead UMETA(DisplayNmae = "Dead"),


	EMS_MAX UMETA(DisplayNmae = "DefaultMAX")
};


//����һ��Enum - EStaminStatus����SprintingStatus
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	//���EStaninaStatus Normal -�趨progressBar������״̬
	ESS_Normal UMETA(DisplayName = "Normal"),
	//���BelowMinimum����޶�״̬
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	//���Exhaustedƣ��״̬
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	//���ExhaustedRecovering״̬
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	//��Ϊ�����У�����ȷ�ĵ����ֵ
	ESS_MAX UMETA(DisplayName = "DefaultMax")
};


UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	//���bHasCombatTarget�ж�Main�Ƿ��ж�սĿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	//����bHasCombatTarget��Setter
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	//���õ���Ѫ���ؼ�����Ļ��ʾ��λ�ã���С -  ��ȡ����λ��
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	FVector CombatTargetLocation;

	//��ӿ��������� - ����������ʾ/���ص���Ѫ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	//�������ϵͳ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles;

	//��ӱ�����ʱ����Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	//�������ģ��TArray - ����ΪFVector��TArray
	TArray<FVector>PickupLocations;

	//����ͼ�пɵ���Debug
	UFUNCTION(BlueprintCallable)
	//���ʰȡ������λ��
	void ShowPickupLocations();

	//�����ڶ���Enum - EMovementStatus - ����������ɫ������ʱ���ڵ��˶�״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	//��AMainΪStamina����enum
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	//ΪStaminaStatus����setter������
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	//Ϊ������������ӱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	//�����С�������ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	//����һ����ֵ���õĹ���Enemy
	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	//����һ����ײĿ������Ϊ��ֵ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTarget;

	//����AEnemy��setter
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	//����һ��Get Look At Rotation Yaw���� - ���ڵ�����ɫս���ĳ��򣨲�ֵ��
	FRotator GetLookAtRotationYaw(FVector Target);

	//ΪMovementStatus���setter���� -�趨�˶�״̬�ͱ����ٶ�
	void SetMovementStatus(EMovementStatus Status);

	//ΪSetMovementStatus�������ܺͳ��֮��ı����ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprinting")
	float SprintingSpeed;

	//������������ -ShiftKeyDown - Bind - Keyboard Shitf
	bool bShiftKeyDown;

	//���°�����ͳ��
	void ShiftKeyDown();

	//�ɿ��������ֹͣ���
	void ShiftKeyUp();

	//Camera Boom�ǰ������������Һ��棬����ue����ʹ�õıȽ�ͳһ��Լ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//����USpringArmComponent����һ��ָ�� CameraBoom
	class USpringArmComponent* CameraBoom;

	//FllowCamera�Ǹ�����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//����UCameraComponent��ָ��ΪFollowCmaera
	class UCameraComponent* FollowCamera;

	//���������ת���ܵĻ���ת����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


	//������� - Ѫ����������Ӳ�ҵ�
//�����ٷֱ�Ѫ����Ѫ����������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	//����Ѫ���ļ��� - Explosive
	void DecrementHealth(float Amount);

	//����TakeDamge�˺�����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	//��������Ӳ�� - Pickup
	void IncrementCoins(int32 Amount);

	//����ɫѪ��С�ڵ���0ʱ����ɫ������
	void Die();

	//��ɫ�������ܵ���Jump
	virtual void Jump() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//��Ҫ��ǰ/��������
	void MoveForward(float Value);

	//��Ҫ��һ�ߵ���һ�ߵ�����
	void MoveRight(float Value);

	bool bMovingForward;
	bool bMovingRight;


	//ͨ�����룬ʵ���ض����ʵ�ת��
	//@���� - ���ʵĲ���  - @param Rate - ��׼���ı���- ����1.0��ʾ100%��ת������
	void TurnAtRate(float Rate);

	//ͨ�����룬ʵ���ض����ʵ�̧��/��Look up/down
	//@���� - ���ʵĲ���  - @param Rate - ��׼���ı���- ����1.0��ʾ100%��̧��/������
	void LookUpAtRate(float Rate);

	//����LMB�������󶨺���
	bool bLMBDown;
	void LMBDown();
	void LMBUp();


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//���һ��Weapon����,��װ�����������������������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	//��������AItem��ʵ�����ɫ�ص���Item�������Ȳ����䱸
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;


	//����һ������װ��������Setter
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	//ΪEquippedWeapon����getter
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	//ΪActiveOverlappingItem����һ��setter
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }


	//ΪAnimMotage��ӹ��ܣ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	//����Attack����
	void Attack();

	//�ڹ�����ɺ󣬴���AttackEnd����ֹͣ��������
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	//����������̫��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	//����PlaySwingSound
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	//����DeathEnd
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	//����һ���µĺ��� UpdateCombatTarget - �ھɵ�Ŀ����������ܻ�ȡ�µ�ս��Ŀ��
	void UpdateCombatTarget();

	//����TSubclassOf<AActor>�ı���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter;
};
