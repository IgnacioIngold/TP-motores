// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpit.h"
#include "myPlayer.h"

// Sets default values
AZombieSpit::AZombieSpit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieSpit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieSpit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);

}

void AZombieSpit::SelfDestruction()
{
}

void AZombieSpit::CheckCollision(AActor* otherActor, UPrimitiveComponent* compCollision)
{
	auto P = Cast<AmyPlayer>(otherActor);
	if (P != nullptr)
	{
		P->GetSlowed();
		Destroy();
	}

	
	
}

