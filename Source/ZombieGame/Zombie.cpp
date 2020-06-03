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
	currentBehaviour = EBehaviours::Follow;
	timeAttack = attackDuration;
	_anim = Cast<UAnimI_Zombie>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	dir = target->GetActorLocation() - GetActorLocation();

	switch (currentBehaviour)
	{

	case EBehaviours::Follow:
		FollowMyTarget(DeltaTime);
		break;
	case EBehaviours::LookTarget:
		LookTowardsTarget();
		break;
	case EBehaviours::Avoidance:
		AvoidanceObstacles(DeltaTime);
		break;
	case EBehaviours::Attack:
		Attack(DeltaTime);
		break;
	
	}
	
	
	
}



void AZombie::FollowMyTarget(float deltaTime)
{
	//Me muevo en dirección a mi objetivo.
	LookTowardsTarget();

	//Si la distancia es menor a cierta cantidad me detengo.
	/*FVector distanceToTarget = (target->GetActorLocation() - GetActorLocation());*/
	float distanceToTarget = dir.Size();
	UE_LOG(LogTemp, Warning, TEXT("mi distancia es %f "), distanceToTarget);
	if (distanceToTarget <= AttackRange)
	{
		//Si el enemigo entra en rango de Combate. Se detiene.
		
		timeAttack = attackDuration;
		currentBehaviour = EBehaviours::Attack;
		
	}
	else
	{
		//Si el enemigo no tiene dentro del rango al Player.
		
		SetActorLocation(GetActorLocation() + GetActorForwardVector()*MovementSpeed*deltaTime);
	}

}

void AZombie::LookTowardsTarget()
{	
	SetActorRotation(dir.Rotation());
}

void AZombie::AvoidanceObstacles(float deltaTime)
{

}

void AZombie::Attack(float deltaTime)
{
	LookTowardsTarget();
	

	timeAttack -= deltaTime;
	
	
	if (_anim->Attaking == false && timeAttack<=0)
	{
		UE_LOG(LogTemp, Warning, TEXT("toma broh, chupala"));
		currentBehaviour = EBehaviours::Follow;
		return;
	}
	_anim->ChangeAttackValue(true);
	
}

void AZombie::Die()
{
}


void AZombie::myBeginOverlap(AActor * ActorOverlap) 
{

}

