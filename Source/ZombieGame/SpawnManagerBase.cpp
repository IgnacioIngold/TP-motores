// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManagerBase.h"

// Sets default values
ASpawnManagerBase::ASpawnManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManagerBase::RegisterReespawneableZombie(AZombie* reespawneable)
{
	respawneables.Add(reespawneable);
}

//Retorna nulo si no hay disponibles.
AZombie* ASpawnManagerBase::getRespawneableZombie()
{
	if (respawneables.Num() > 0)
	{
		AZombie* toRespawnZ = respawneables[0];
		respawneables.RemoveAt(0);
		return toRespawnZ;
	}

	return nullptr;
}

