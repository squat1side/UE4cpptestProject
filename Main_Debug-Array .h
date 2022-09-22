// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

//创建枚举Enum - 冲刺系统
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayNmae = "Normal"),
	//创建冲刺状态 - Sprinting的enum
	EMS_Sprinting UMETA(DisplayNmae = "Sprinting"),

	EMS_MAX UMETA(DisplayNmae = "DefaultMAX")
};

//创建一个Enum - EStaminStatus用于SprintingStatus
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	//添加EStaninaStatus Normal -设定progressBar的正常状态
	ESS_Normal UMETA(DisplayName = "Normal"),
	//添加BelowMinimum最低限度状态
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	//添加Exhausted疲惫状态
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	//添加ExhaustedRecovering状态
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	//作为结束行，有明确的的最大值
	ESS_MAX UMETA(DisplayName = "DefaultMax")
};


UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	//添加容器模板TArray - 类型为FVector的TArray
	TArray<FVector>PickupLocations;

	//在蓝图中可调用Debug
	UFUNCTION(BlueprintCallable)
	//标记拾取物所在位置
	void ShowPickupLocations();

	//创建第二个Enum - EMovementStatus - 用来描述角色在任意时间内的运动状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	//在AMain为Stamina创建enum
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	//为StaminaStatus创建setter函数：
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	//为体力消耗率添加变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	//添加最小冲刺体力值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	//为MovementStatus添加setter函数 -设定运动状态和奔跑速度
	void SetMovementStatus(EMovementStatus Status);

	//为SetMovementStatus创建奔跑和冲刺之间的奔跑速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprinting")
	float SprintingSpeed;

	//创建布尔变量 -ShiftKeyDown - Bind - Keyboard Shitf
	bool bShiftKeyDown;

	//按下按键后就冲刺
	void ShiftKeyDown();

	//松开按键后就停止冲刺
	void ShiftKeyUp();


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

	//创建血条的减少 - Explosive
	void DecrementHealth(float Amount);

	//创建增加硬币 - Pickup
	void IncrementCoins(int32 Amount);

	//当角色血量小于等于0时，角色死亡；
	void Die();



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
