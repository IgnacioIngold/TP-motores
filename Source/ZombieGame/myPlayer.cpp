// Fill out your copyright notice in the Description page of Project Settings.

#include "myPlayer.h"


// Sets default values
AmyPlayer::AmyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AmyPlayer::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* mySkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();
	if (mySkeletalMesh)
	{
		this->animController = Cast<UBasePlayer_AnimInstance>(mySkeletalMesh->GetAnimInstance());
		if (animController)
		{
			animController->hasWeapon = true;
			UE_LOG(LogTemp, Warning, TEXT("La referencia fue seteada correctamente"));
		}
	}

	auto gm = Cast<AZG_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (gm != nullptr)
	{
		_gameMode = gm;
	}

	myAudioComp = FindComponentByClass<UAudioComponent>();
}

// Called every frame
void AmyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*UE_LOG(LogTemp, Warning, TEXT("Mi posici�n es %s y mi rotaci�n es %s"), *GetActorLocation().ToString(), *GetActorRotation().ToString());*/

	if (health > 0)
	{
		_gameMode->GameTime += DeltaTime;
	}
}

// Called to bind functionality to input
void AmyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AmyPlayer::Shoot);*/
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AmyPlayer::Jump);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AmyPlayer::Reload);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AmyPlayer::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AmyPlayer::StopShooting);

	PlayerInputComponent->BindAxis("Horizontal", this, &AmyPlayer::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AmyPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("RotatePlayer", this, &AmyPlayer::RotatePlayer);
}

void AmyPlayer::MoveHorizontal(float HAxis)
{
	if (health > 0)
	{
		this->animController->horizontalAxis = HAxis;
		AddMovementInput(GetActorRightVector(), HAxis);
	}
}

void AmyPlayer::MoveVertical(float VAxis)
{
	if (health > 0)
	{
		this->animController->verticalAxis = VAxis;
		AddMovementInput(GetActorForwardVector(), VAxis);
	}
}

void AmyPlayer::RotatePlayer(float rot) 
{
	if (health > 0)
		AddControllerYawInput(rot);
}

void AmyPlayer::StartShooting()
{
	if (this->TestLocator != nullptr && health > 0)
	{
		FTransform position = this->TestLocator->GetComponentTransform();
		/*UE_LOG(LogTemp, Warning, TEXT("Disparo desde código, la referencia del locator esta seteada :D"));*/
	
		if (AmmoInMagazine > 0)
		{
			UWorld* const World = GetWorld();
			if (World != NULL && prefabBullet != NULL)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = GetOwner();

				ABullet* bullet = World->SpawnActor<ABullet>(prefabBullet, position);

				if (this->animController != NULL)
				{
					this->animController->isShooting = true;
				}
				if (myAudioComp)
				{
					myAudioComp->Stop();

					if (myShootCue)
						myAudioComp->SetSound(myShootCue);
					myAudioComp->Play();
				}
			}
		}
		else
		{
			/*UE_LOG(LogTemp, Warning, TEXT("Intentaste Disparar pero no tenés mas balas... Recarga!"));*/
		}
	}
}

void AmyPlayer::StopShooting()
{
	if (this->animController != NULL)
	{
		this->animController->isShooting = false;
		UE_LOG(LogTemp, Warning, TEXT("Stop Shooting :D"));
	}
}

void AmyPlayer::ShootWithTransform(FTransform transform)
{
	if (AmmoInMagazine > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Disparo desde Blueprint"));

		UWorld* const World = GetWorld();
		if (World != NULL && prefabBullet != NULL)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = GetOwner();

			ABullet * bullet = World->SpawnActor<ABullet>(prefabBullet, transform);
			
			if (this->animController != NULL)
			{
				this->animController->isShooting = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Intentaste Disparar por Blueprint, pero no tenés mas balas... Recarga!"));
	}
}

void AmyPlayer::ShootWithPositionAndRotation(FVector position, FRotator rotation)
{
	if (AmmoInMagazine > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Disparo desde Blueprint"));

		UWorld* const World = GetWorld();
		if (World != NULL && prefabBullet != NULL)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = GetOwner();

			ABullet * bullet = World->SpawnActor<ABullet>(prefabBullet, position, rotation, spawnParams);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Intentaste Disparar por Blueprint, pero no tenés mas balas... Recarga!"));
	}
}

void AmyPlayer::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Salto we"));
}

void AmyPlayer::Reload()
{
	if (Ammo <= 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No hay suficientes balas en la reserva"));
		return;
	}

	if (AmmoInMagazine < MaxAmmoInMagazine)
	{
		UE_LOG(LogTemp, Warning, TEXT("Recargo we"));
		//Primero me fijo cuantas balas me faltan para completar el máximo.
		int faltantes = MaxAmmoInMagazine - AmmoInMagazine;
		int añadibles = 0;

		//Ammo = 4, maxAmmo = 30 --> en magazine = 10 --> 
		if (Ammo > 0 && Ammo > faltantes) //Si la cantidad de balas es mayor a 0 y a faltantes.
			añadibles += faltantes;

		if (Ammo > 0 && Ammo <= faltantes)
			añadibles = Ammo;

		Ammo -= añadibles;
	}
}

void AmyPlayer::AddBulletsToBackPack(int Bullets)
{
	UE_LOG(LogTemp, Warning, TEXT("Balas pickeadas!! Se añadieron %s balas al BackPack."), Bullets);
	Ammo += Bullets;
}

void AmyPlayer::Heal(int Ammount)
{
	UE_LOG(LogTemp, Warning, TEXT("Vida regenerada!! Se añadieron %s puntos de vida."), Ammount);
	health += Ammount;
}

void AmyPlayer::AddExtraMagazineSlots(int extraBullets)
{
	UE_LOG(LogTemp, Warning, TEXT("Magazine extendido!!, Se añadieron %s espacios para balas extra."), extraBullets);
	MaxAmmoInMagazine += extraBullets;
}

void AmyPlayer::Die() 
{
	animController->isDead = true;

	if (_gameMode)
	{
		_gameMode->PlayerDied();
	}
}

void AmyPlayer::RespawnPlayer()
{
	animController->isDead = false;
	health = MaxHealth;
}

void AmyPlayer::GetAndLoadWeapon()
{
	if (this->animController)
	{
		this->animController->hasWeapon = true;
	}
}

void AmyPlayer::GetHit(int Damage)
{
	if (health > 0)
	{
		//Pos me meten la verga :v
		health -= Damage;
		UE_LOG(LogTemp, Warning, TEXT("Recibí Daño, vida restante es: %i"), health);

		if (health <= 0)
		{
			health = 0;
			Die();
		}
	}
}

