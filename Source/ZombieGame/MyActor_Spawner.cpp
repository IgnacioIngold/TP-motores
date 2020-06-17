// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor_Spawner.h"

// Sets default values
AMyActor_Spawner::AMyActor_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor_Spawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(timerSpawn, this, &AMyActor_Spawner::SpawnZombie, timerToSpawn, bucleSpawn);
}

// Called every frame
void AMyActor_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyActor_Spawner::SpawnZombie()
{
	if(prefabZombie)
	{
		FActorSpawnParameters p;
		GetWorld()->SpawnActor<AZombie>(prefabZombie, GetActorLocation(), GetActorRotation(), p);
	}
}

