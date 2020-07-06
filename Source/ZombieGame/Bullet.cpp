// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Zombie.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	TimerToDestruction();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);
}

void ABullet::TimerToDestruction()
{
	GetWorld()->GetTimerManager().SetTimer(timerDestruction, this, &ABullet::SelfDestruction, 3.0f, false);
}

void ABullet::SelfDestruction()
{
	Destroy();
}

void ABullet::CheckCollision(AActor* otherActor, UPrimitiveComponent* compCollision)
{
	/*auto zombie = Cast<AZombie>(otherActor);
	if (zombie != nullptr)
	{
		if (compCollision->IsA(UCapsuleComponent::StaticClass()))
		{
			zombie->GetHit(dmg);
			SelfDestruction();
		}
		if (compCollision->IsA(UBoxComponent::StaticClass()))
		{
			zombie->GetHit(dmg/4);
			SelfDestruction();
		}
	}*/
}



