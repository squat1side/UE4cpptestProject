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

	//Camera Boom是把摄像机放在玩家后面，这是ue里面使用的比较统一的约定
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//声明USpringArmComponent，是一个指针 CameraBoom
	class USpringArmComponent* CameraBoom;

	//FllowCamera是跟随玩家摄像机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//声明UCameraComponent，指针为FollowCmaera
	class UCameraComponent* FollowCamera;

	//调节摄像机转向功能的基础转向功率
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	//玩家数据 - 血条，耐力，硬币等
//创建百分比血量的血条和耐力条
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

	//需要向前/向后的输入
	void MoveForward(float Value);

	//需要从一边到另一边的输入
	void MoveRight(float Value);

	//通过输入，实现特定速率的转向
	//@参数 - 比率的参数  - @param Rate - 标准化的比率- 即：1.0表示100%的转向速率
	void TurnAtRate(float Rate);

	//通过输入，实现特定速率的抬起/下Look up/down
	//@参数 - 比率的参数  - @param Rate - 标准化的比率- 即：1.0表示100%的抬起/下速率
	void LookUpAtRate(float Rate);


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
