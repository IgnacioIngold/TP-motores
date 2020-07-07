// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "AnimI_Zombie.h"
#include "myPlayer.h"
#include "ZombieSpit.h"
#include "GameFramework/Actor.h"
#include "ZG_GameModeBase.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EBehaviours : uint8
{
	Follow UMETA(DisplayName = "FollowTarget"),
	LookTarget UMETA(DisplayName = "Look at Target"),
	Avoidance UMETA(DisplayName = "Avoid Obstacles"),
	Attack UMETA(DisplayName = "Attacking"),
	Spit UMETA(DisplayName = "Spitting")
};

UCLASS()
class ZOMBIEGAME_API AZombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombie();

	UPROPERTY()
		UAudioComponent* myAudioComp;

	UPROPERTY(EditAnywhere)
		USoundCue* dieCue;
	UPROPERTY(EditAnywhere)
		USoundCue * attackCue;
	UPROPERTY(EditAnywhere)
		USoundCue* chaseCue;

	UPROPERTY(VisibleAnywhere)
		AActor * target;

	UPROPERTY(visibleAnywhere)
		EBehaviours currentBehaviour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float SpitRangeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float SpitRangeMin;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float WeightAvoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float attackDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float SpitDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float speedRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stats")
		float HeathPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BodyCollision")
		float DamageForHitTorso = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BodyCollision")
		float DamageForHitLeftArm = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BodyCollision")
		float DamageForHitRightArm = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BodyCollision")
		int pointsForHitTorso = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BodyCollision")
		int pointsForHitLeftArm = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BodyCollision")
		int pointsForHitRightArm = 5;

	//Pasa a ataque si está atacando.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controllers")
		bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* closestObstacle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SpitSpawner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AZombieSpit> prefabSpit;
	
	
	FTimerHandle timerDead;

	FTimerHandle timerSpit;

	UPROPERTY()
		UAnimI_Zombie* _anim;
	UPROPERTY()
		AZG_GameModeBase* _gamemode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector dir;

	float dist;

	float timeAttack;

	float timeSpitting;

	bool AttackPerform;

	bool SpitChecked;
	
	bool SpitPerform;

	float distanceToTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void FollowMyTarget(float deltaTime);
	void LookTowardsTarget();
	void AvoidanceObstacles(float deltaTime);
	void Attack(float deltaTime);
	void Spit(float deltaTime);
	void ChangeSpitChecked();
	void GetHit(int Damage);
	UFUNCTION(BlueprintCallable)
	void Die();

	void DestroyDead();
	void raycastAttack();
	
	
	UFUNCTION(BlueprintCallable)
		void myBeginOverlap(AActor * ActorOverlap);
	
	UFUNCTION(BlueprintCallable)
		void myEndOverlap(AActor* ActorOverlap);

	UFUNCTION(BlueprintCallable)
		void BodyCollisionHandler(AActor* other, int bodyPart);

};
