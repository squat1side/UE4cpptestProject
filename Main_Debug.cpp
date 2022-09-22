// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Main.h"


APickup::APickup()
{
	CoinCount = 1;
}


void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OnOverlapBegin对Super的调用
	//将需要的参数从实际的OnOverlapBegin中获取，被传递的参数放置父类函数Super
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Pickup::OnOverlapBegin()"));

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->IncrementCoins(CoinCount);
			//获取PickupLocations -收集拾取物，将位置存储下来
			Main->PickupLocations.Add(GetActorLocation());
		}
	}
}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Pickup::OnOverlapEnd()"));
}














