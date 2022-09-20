// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

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
