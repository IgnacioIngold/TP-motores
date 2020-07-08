// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Zombie.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MyActor_Spawner.generated.h"

class AZG_GameModeBase;

UCLASS()
class ZOMBIEGAME_API AMyActor_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor_Spawner();
	UPROPERTY(EditAnyWhere)
		TSubclassOf<AZombie> prefabZombie;
	UPROPERTY(EditAnyWhere)
		float timerToSpawn;
	UPROPERTY(EditAnyWhere)
		bool bucleSpawn;
	/*UPROPERTY(EditAnywhere)
		TArray<AZombie> spawned;*/
	UPROPERTY(EditAnywhere)
		TArray<AZombie*> toRespawn;

	FTimerHandle timerSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnZombie();
	void RegisterToRespawn(AZombie* zombie);
};
