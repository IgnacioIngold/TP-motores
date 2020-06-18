// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "AnimI_Zombie.h"
#include "myPlayer.h"
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

class AZG_GameModeBase;

UCLASS()
class ZOMBIEGAME_API AZombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombie();



	UPROPERTY(VisibleAnywhere)
		AActor * target;

	UPROPERTY(visibleAnywhere)
		EBehaviours currentBehaviour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Health;

	UPROPERTY(EditAnywhere)
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeathPercent;

	//Pasa a ataque si está atacando.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsDead;

	UPROPERTY(EditAnywhere)
		float AttackRange;

	UPROPERTY(EditAnywhere)
		float WeightAvoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* closestObstacle;
	
	UPROPERTY(EditAnywhere)
		float attackDuration;

	UPROPERTY(EditAnywhere)
		float speedRot;

	UPROPERTY(EditAnywhere)
		int Damage;

	FTimerHandle timerDead;



	UAnimI_Zombie* _anim;
	AZG_GameModeBase* _gamemode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector dir;

	float dist;

	float timeAttack;

	bool AttackPerform;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void FollowMyTarget(float deltaTime);
	void LookTowardsTarget();
	void AvoidanceObstacles(float deltaTime);
	void Attack(float deltaTime);
	void GetHit(int Damage);
	UFUNCTION(BlueprintCallable)
	void Die();

	void DestroyDead();
	void raycastAttack();
	
	
	UFUNCTION(BlueprintCallable)
		void myBeginOverlap(AActor * ActorOverlap);
	
	UFUNCTION(BlueprintCallable)
		void myEndOverlap(AActor* ActorOverlap);


	
	
};
