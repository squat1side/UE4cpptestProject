// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


//����һ��enum - EEnemyMovementStatus��ʵ�ִ���׷���ɫ����
UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	//Idle/MoveToTarget/Attack����״̬
	EMS_Idle				UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget		UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking			UMETA(DeplayName = "Attacking"),
	EMS_Dead				UMETA(DeplayName = "Dead"),

	//��һ��Ĭ�ϵ�enum���ֵ
	EMS_MAX					UMETA(DeplayName = "DefaultMAX")
};


UCLASS()
class FIRSTPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//��������EEnemyMovementStatus
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	//����Setter
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	//����Getter
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	//����һ��������ײ  - �ڽ�ɫ�����趨��Χ�ͻ�׷���ɫ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	//����һ��С��������ײ�����з�Ӧ�Խ�ɫ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* CombatSphere;

	//ΪAI Move To����AI Controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	//Ϊ�������Ѫ���Ͱٷֱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxHealth;
	//���˶Խ�ɫ����˺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Damage;

	//��Enemy�����������ϵͳ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* HitParticles;

	//���enemy������ʱ����Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound;

	//���enemy����ʱ����Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USoundCue* SwingSound;

	//��Ӻ��幥����ײ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* CombatCollision;

	//�����̫��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* CombatMontage;

	//����TimeerHanle -�ǵ��˹������Ӽ��
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackMaxTime;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	//����һ��������ʱ�� -��������Enemy�������ʬ�岻ռ���ڴ�
	FTimerHandle DeathTimer;
	//ʱ���ӳ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//����AgroSphereOnOverlapBegin����
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//����AgroSphereOnOverlapEnd����
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//����CombatSphereOnOverlapBegin����
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//����CombatSphereOnOverlapEnd����
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//��������Move To Target - ʹEnemy����Ŀ�ĵ�/��ɫ
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain* Target);

	//����һ���������� - bOverlappingCombatSphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	AMain* CombatTarget;

	//����CombatOnOverlapBegin����
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//����CombatOnOverlapEnd����
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//����Activate/DeactivateCollision����
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	//ΪAttacking��Ӳ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bAttacking;

	//����Attacking����
	void Attack();


	//��ӹ���ֹͣ
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	//����TakeDamge�˺�����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//����Die����
	void Die();

	//����DeathEnd����
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	//��������Alive
	bool Alive();

	// Disappear -����һ�������ܹ�����Destroy -�����õ���ʱ��
	void Disappear();

};
