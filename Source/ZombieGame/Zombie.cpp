// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"


// Sets default values
AZombie::AZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	target = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowMyTarget(DeltaTime);
}

void AZombie::FollowMyTarget(float deltaTime)
{
	//Me muevo en dirección a mi objetivo.
	LookTowardsTarget();

	//Si la distancia es menor a cierta cantidad me detengo.
	/*FVector distanceToTarget = (target->GetActorLocation() - GetActorLocation());*/
	float distanceToTarget = FVector::Distance(target->GetActorLocation(), GetActorLocation());

	if (distanceToTarget < AttackRange)
	{
		//Si el enemigo entra en rango de Combate. Se detiene.
		IsAttacking = true;
	}
	else
	{
		//Si el enemigo no tiene dentro del rango al Player.
		MovementSpeed = 1.0f;
		SetActorLocation(GetActorLocation() + GetActorForwardVector()*MovementSpeed*deltaTime);
	}

}

void AZombie::LookTowardsTarget()
{
	//Miro en la dirección de mi objetivo.
	FVector dirToTarget = target->GetActorLocation() - GetActorLocation();
	FRotator newRotation = dirToTarget.Rotation();
	SetActorRotation(newRotation);
}

void AZombie::AvoidanceObstacles(float deltaTime)
{

}

void AZombie::Attack()
{

}


void AZombie::myBeginOverlap(AActor * ActorOverlap) 
{

}

