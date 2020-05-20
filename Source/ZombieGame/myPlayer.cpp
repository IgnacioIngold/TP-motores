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

}

// Called every frame
void AmyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*UE_LOG(LogTemp, Warning, TEXT("Mi posici�n es %s y mi rotaci�n es %s"), *GetActorLocation().ToString(), *GetActorRotation().ToString());*/
}

// Called to bind functionality to input
void AmyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AmyPlayer::Shoot);*/
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AmyPlayer::Jump);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AmyPlayer::Reload);

	PlayerInputComponent->BindAxis("Horizontal", this, &AmyPlayer::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AmyPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("RotatePlayer", this, &AmyPlayer::RotatePlayer);
}

void AmyPlayer::MoveHorizontal(float HAxis)
{
	AddMovementInput(GetActorRightVector(), HAxis);
}

void AmyPlayer::MoveVertical(float VAxis)
{
	AddMovementInput(GetActorForwardVector(), VAxis);
}

void AmyPlayer::RotatePlayer(float rot) 
{
	AddControllerYawInput(rot);
}

//void AmyPlayer::Shoot() 
//{
//	UE_LOG(LogTemp, Warning, TEXT("Disparo"));
//
//	UWorld* const World = GetWorld();
//	if (World != NULL) 
//	{
//		if (prefabBullet != NULL)
//		{
//			FActorSpawnParameters spawnParams;
//			spawnParams.Owner = GetOwner();
//
//			ABullet * bullet = World->SpawnActor<ABullet>(prefabBullet, GetActorLocation(), GetActorRotation(), spawnParams);
//		}
//	}
//}

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

}

