// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//���һ��ȷ�����ж�����Ч�ģ��������ƶ�
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	//��Collider��ȡ�����ʸ��
	//ͨ��InputVector��ʵ��Consume Vector
	//��Collider���Movementʱ�򣬻���������ӵ�InputVector
	//GetClampedToMaxSize���ֵΪ1
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.f;

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		//���ײ�������������ű�ײ���󻬶���ȥ��
		//����SlideAlongSurface - װ��ĳһ����󣬿��Ի�ȡ��ײ����ķ���ʸ���ʹ�ֱʸ��
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}


}