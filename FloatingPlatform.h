// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	//���һ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent* Mesh;

	//��Ӽ���Vector������ռ䷵�صĵ� - ��� �� �յ�
	UPROPERTY(EditAnywhere)
	FVector StartPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector EndPoint;

	//����InterSpeed���� - ���Endpoint  -  DeltaTime��֡��ǰһ֡
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpSpeed;

	//Ϊ��ʱ������һ��ʱ����� -����ȡ��ֵ��ʱ����ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
    float InterpTime;

	//������ʱ�� - InterpTimer���Դ򿪻�رն�Ӧ�Ƿ��ڲ�ֵ
	FTimerHandle InterpTimer;

	//�ж��Ƿ��ڲ�ֵ - ��������ֵ - bInterping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bInterping;

	//�����յ�󣬽������յ㻥������ȡ����ľ���
	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//����Interping -��Tick�´���һ������ - ToggleInterping��void����
	void ToggleInterping();

	//�ж�������λ�ú�Ϊtrue�򴴽�Swap����������λ��
	void SwapVectors(FVector& VecOne, FVector& VecTwo);


};
