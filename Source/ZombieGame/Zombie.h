// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EBehaviours : uint8
{
	Follow UMETA(DisplayName = "FollowTarget"),
	LookTarget UMETA(DisplayName = "Look at Target"),
	Avoidance UMETA(DisplayName = "Avoid Obstacles")
};

UCLASS()
class ZOMBIEGAME_API AZombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombie();

	UPROPERTY(VisibleAnywhere)
		AActor * target;

	/*UPROPERTY(EditAnywhere)
		EBehaviours currentBehaviour;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		float MovementAnimState;

	//Pasa a ataque si está atacando.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomVariables")
		float AttackRange;

	UPROPERTY(EditAnywhere)
		float WeightAvoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* closestObstacle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FollowMyTarget(float deltaTime);
	void LookTowardsTarget();
	void AvoidanceObstacles(float deltaTime);
	void Attack();
	void Die();

	UFUNCTION(BlueprintCallable)
		void myBeginOverlap(AActor * ActorOverlap);
	
};
