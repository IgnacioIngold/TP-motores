// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Bullet.generated.h"

class AZombie;

UCLASS()
class ZOMBIEGAME_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	
	UPROPERTY(EditAnywhere)
		float speed;
	UPROPERTY(EditAnywhere)
		float dmg;


	FTimerHandle timerDestruction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TimerToDestruction();
	void SelfDestruction();
	
	UFUNCTION(BlueprintCallable)
		void CheckCollision(AActor* otherActor);
};
