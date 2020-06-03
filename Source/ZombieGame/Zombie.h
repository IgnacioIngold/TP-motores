// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "AnimI_Zombie.h"
#include "GameFramework/Actor.h"
#include "Zombie.generated.h"




UENUM(BlueprintType)
enum class EBehaviours : uint8
{
	Follow UMETA(DisplayName = "FollowTarget"),
	LookTarget UMETA(DisplayName = "Look at Target"),
	Avoidance UMETA(DisplayName = "Avoid Obstacles"),
	Attack UMETA(DisplayName = "Attacking")
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

	UPROPERTY(EditAnywhere, Category = Enum)
		EBehaviours currentBehaviour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int Health;

	UPROPERTY(EditAnywhere)
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

	UPROPERTY(EditAnywhere)
		float AttackRange;

	UPROPERTY(EditAnywhere)
		float WeightAvoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* closestObstacle;
	
	UPROPERTY(EditAnywhere)
		float attackDuration;

	UAnimI_Zombie* _anim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector dir;

	float dist;

	float timeAttack;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void FollowMyTarget(float deltaTime);
	void LookTowardsTarget();
	void AvoidanceObstacles(float deltaTime);
	void Attack(float deltaTime);
	void Die();

	UFUNCTION(BlueprintCallable)
		void myBeginOverlap(AActor * ActorOverlap);
	
};
