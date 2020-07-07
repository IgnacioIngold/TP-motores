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
	currentBehaviour = EBehaviours::Avoidance;
	timeAttack = attackDuration;
	SpitChecked = false;
	_anim = Cast<UAnimI_Zombie>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());
	myAudioComp = FindComponentByClass<UAudioComponent>();
	_gamemode = Cast<AZG_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (refMaterial)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(refMaterial, this);
		USkeletalMeshComponent * mySkeleton = FindComponentByClass<USkeletalMeshComponent>();
		if (mySkeleton && DynMaterial)
		{
			mySkeleton->SetMaterial(0, DynMaterial);
			mySkeleton->SetMaterial(1, DynMaterial);
			mySkeleton->SetMaterial(2, DynMaterial);
		}
	}
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (fadeBody && DynMaterial)
	{
		/*FMaterialParameterInfo parameterInfo;*/
		float currentValue = 0;
		DynMaterial->GetScalarParameterValue(FName(TEXT("Dissolve")) , currentValue);
		currentValue -= DeltaTime;
		DynMaterial->SetScalarParameterValue("Dissolve", currentValue);
	}

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
			case EBehaviours::Spit:
					Spit(DeltaTime);
					break;
		}

		if (currentBehaviour == EBehaviours::Follow || currentBehaviour == EBehaviours::Avoidance)
		{
			if (myAudioComp && chaseCue && myAudioComp->Sound != chaseCue)
			{
				myAudioComp->Stop();
				myAudioComp->SetSound(chaseCue);
				myAudioComp->Play();
			}
		}
	}
	distanceToTarget = dir.Size();

	if (distanceToTarget <= SpitRangeMax && distanceToTarget >= SpitRangeMin && !SpitChecked)
	{
		int result = rand() % 5 + 1;
		
		if (result == 1)
		{
			timeSpitting = SpitDuration;
			currentBehaviour = EBehaviours::Spit;
			
		}
		SpitChecked = true;
		GetWorld()->GetTimerManager().SetTimer(timerSpit, this, &AZombie::ChangeSpitChecked, 5.0f, false);
	}

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
		SetActorLocation(GetActorLocation() + GetActorForwardVector()*MovementSpeed*deltaTime);
	}
}

void AZombie::LookTowardsTarget()
{	
	SetActorRotation(dir.Rotation());
}

void AZombie::ChangeSpitChecked()
{
	SpitChecked = false;
}
void AZombie::Spit(float deltaTime)
{
	LookTowardsTarget();
	timeSpitting -= deltaTime;

	if (timeSpitting <= SpitDuration - 1.321f && !SpitPerform)
	{
		SpitPerform=true;

		FTransform position = this->SpitSpawner->GetComponentTransform();
		AZombieSpit* bullet = GetWorld()->SpawnActor<AZombieSpit>(prefabSpit, position);
	}
	if (timeSpitting<=0)
	{
		currentBehaviour = EBehaviours::Follow;
	}

	_anim->ChangeSpitValue(true);

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
		HeathPercent = Health/100.0f;
		//UE_LOG(LogTemp, Warning, TEXT("AZombie::GetHit::Daño recibido: %i, porcentaje de vida restante: %f."), damage, HeathPercent);

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

		if (_gamemode != nullptr)
		{
			_gamemode->ZombieDied();
		}
	}

	_anim->ChangeLifeValue(true);
	IsDead = true;
	_gamemode->AddPoints(100);
	GetWorld()->GetTimerManager().SetTimer(timerStartDisolve, this, &AZombie::SetDisolveOn, 7.0, false);
	GetWorld()->GetTimerManager().SetTimer(timerDead, this , &AZombie::DestroyDead, 10.0f, false);
}
void AZombie::SetDisolveOn()
{
	fadeBody = true;
}

void AZombie::DestroyDead()
{
	Destroy();
}


void AZombie::raycastAttack()
{
	if (distanceToTarget <= AttackRange + 100)
	{
		if (myAudioComp && attackCue)
		{
			myAudioComp->Stop();
			myAudioComp->SetSound(attackCue);
			myAudioComp->Play();
		}

		AmyPlayer* CharacterHit = Cast<AmyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (CharacterHit != nullptr)
			{
				CharacterHit->GetHit(Damage);
			}
	}
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

void AZombie::BodyCollisionHandler(AActor* other, int bodyPart)
{
	ABullet* bullet = Cast<ABullet>(other);
	if (bullet == nullptr) return;

	FString Part = "";
	switch (bodyPart)
	{
	case 0:
		Part = "Torso";
		if (_gamemode)
			_gamemode->AddPoints(pointsForHitTorso);
		GetHit(DamageForHitTorso);
		break;

	case  1:
		Part = "Left Arm";
		if (_gamemode)
			_gamemode->AddPoints(pointsForHitLeftArm);
		GetHit(DamageForHitLeftArm);
		break;

	case 2:
		Part = "Right Arm";
		if (_gamemode)
			_gamemode->AddPoints(pointsForHitRightArm);
		GetHit(DamageForHitRightArm);
		break;

	default:
		Part = "Invalid!";
		break;
	}

	//FQuat myForward = GetTransform().GetRotation();
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector dirToPlayer = (playerLocation - this->GetActorLocation());

	//FRotator rotation = GetTransform().Rotator();
	FRotator rotation = dirToPlayer.Rotation();
	FVector position = bullet->GetTransform().GetLocation();
	//FTransform newTransform = FTransform(rotation, position, FVector::OneVector);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFeedback,position, rotation, true);

	other->Destroy();
	//UE_LOG(LogTemp, Warning, TEXT("Collisionó la wea en: %s"), *Part);
}

