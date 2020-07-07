// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "myPlayer.h"
#include "ZombieSpit.generated.h"

UCLASS()
class ZOMBIEGAME_API AZombieSpit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpit();
	UPROPERTY(EditAnywhere)
		float speed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SelfDestruction();

	UFUNCTION(BlueprintCallable)
		void CheckCollision(AActor* otherActor, UPrimitiveComponent* compCollision);

};
