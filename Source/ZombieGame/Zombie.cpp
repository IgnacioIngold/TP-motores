// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "ZG_GameModeBase.h"

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
	currentBehaviour = EBehaviours::Avoidance;
	timeAttack = attackDuration;
	_anim = Cast<UAnimI_Zombie>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());
	myAudioComp = FindComponentByClass<UAudioComponent>();
	_gamemode = Cast<AZG_GameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	dir = target->GetActorLocation() - GetActorLocation();
	if (!IsDead && !_anim->GetHited)
	{
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
	distanceToTarget = dir.Size();
}

void AZombie::FollowMyTarget(float deltaTime)
{
	//Me muevo en direcci�n a mi objetivo.
	LookTowardsTarget();
	

	if (closestObstacle)
	{
		currentBehaviour = EBehaviours::Avoidance;

	}

	
	
	
	if (distanceToTarget <= AttackRange)
	{
		//Si el enemigo entra en rango de Combate. Se detiene.

		
		AttackPerform = false;
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
	if (!closestObstacle)
	{
		currentBehaviour = EBehaviours::Follow;
		return;
	}
	FVector direction = GetActorLocation() - closestObstacle->GetActorLocation();
	direction = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal() + (direction.GetSafeNormal() * WeightAvoid);

	FVector Rot = FMath::Lerp(GetActorForwardVector(), direction, deltaTime * speedRot);

	Rot.Z = 0;

	SetActorRotation(Rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed * deltaTime);
	
	if (distanceToTarget <= AttackRange)
	{
		AttackPerform = false;
		timeAttack = attackDuration;
		currentBehaviour = EBehaviours::Attack;

	}
}

void AZombie::Attack(float deltaTime)
{
	LookTowardsTarget();
	

	timeAttack -= deltaTime;
	
	if (timeAttack <= 0.854f && AttackPerform == false)
	{
		AttackPerform = true;
		raycastAttack();
	}
	
	if (_anim != nullptr)
	{
		if (_anim->Attaking == false && timeAttack <= 0)
		{

			currentBehaviour = EBehaviours::Follow;
			return;
		}
	}

	_anim->ChangeAttackValue(true);
	

	
}
void AZombie::GetHit(int damage)
{
	if (Health > 0)
	{
		Health -= damage;
		_anim->ChangeHitValue(true);
		UE_LOG(LogTemp, Warning, TEXT("%f"), HeathPercent);
		HeathPercent = Health/100.0f;
		UE_LOG(LogTemp, Warning, TEXT("%f"), HeathPercent);

		if (Health <= 0)
		{
			Health = 0;
			Die();
		}
	}
}

void AZombie::Die()
{
	bool enterOneTime;
	enterOneTime = true;
	if (myAudioComp && enterOneTime)
	{
		enterOneTime = false;
		myAudioComp->Stop();

		if (dieCue)
		{
			myAudioComp->SetSound(dieCue);
			myAudioComp->Play();
		}

		AZG_GameModeBase* gameMode = Cast<AZG_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (gameMode != nullptr)
		{
			gameMode->ZombieDied();
		}
	}

	_anim->ChangeLifeValue(true);
	IsDead = true;
	_gamemode->AddPoints(100);
	GetWorld()->GetTimerManager().SetTimer(timerDead, this , &AZombie::DestroyDead, 10.0f , false);
}
void AZombie::DestroyDead()
{
	Destroy();
}


void AZombie::raycastAttack()
{
	if (distanceToTarget <= AttackRange + 100)
	{
		AmyPlayer* CharacterHit = Cast<AmyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (CharacterHit != nullptr)
			{
					CharacterHit->GetHit(Damage);
			}
	}
	//FHitResult hit;

	//FCollisionQueryParams p = FCollisionQueryParams(TEXT(""), false, this);
	//GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 150, ECollisionChannel::ECC_PhysicsBody, p);
	//if (hit.GetActor())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *hit.GetActor()->GetName());
	//}
	//AmyPlayer* CharacterHit = Cast<AmyPlayer>(hit.GetActor());
	//if (CharacterHit != nullptr)
	//{
	//	if (CharacterHit)
	//	{
	//		CharacterHit->GetHit(Damage);
	//		//UE_LOG(LogTemp, Warning, TEXT("toma broh, flores y rosas"));
	//	}
	//}
}


void AZombie::myBeginOverlap(AActor * ActorOverlap) 
{
	if (ActorOverlap == this || ActorOverlap == target)
		return;

	if (closestObstacle)
	{
		FVector DistA = closestObstacle->GetActorLocation() - GetActorLocation();
		FVector DistB = ActorOverlap->GetActorLocation() - GetActorLocation();
		if (DistA.Size() > DistB.Size())
			closestObstacle = ActorOverlap;
	}
	else
		closestObstacle = ActorOverlap;
}

void AZombie::myEndOverlap(AActor* ActorOverlap)
{
	if (ActorOverlap == closestObstacle)
	{
		closestObstacle = NULL;
	}
}


