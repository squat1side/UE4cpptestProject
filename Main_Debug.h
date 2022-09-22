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

	//��������Ӳ�� - Pickup
	void IncrementCoins(int32 Amount);

	//����ɫѪ��С�ڵ���0ʱ����ɫ������
	void Die();



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

	//ͨ�����룬ʵ���ض����ʵ�ת��
	//@���� - ���ʵĲ���  - @param Rate - ��׼���ı���- ����1.0��ʾ100%��ת������
	void TurnAtRate(float Rate);

	//ͨ�����룬ʵ���ض����ʵ�̧��/��Look up/down
	//@���� - ���ʵĲ���  - @param Rate - ��׼���ı���- ����1.0��ʾ100%��̧��/������
	void LookUpAtRate(float Rate);


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
