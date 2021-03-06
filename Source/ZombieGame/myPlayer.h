// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Bullet.h"
#include "BasePlayer_AnimInstance.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "myPlayer.generated.h"

class AZG_GameModeBase;

UCLASS()
class ZOMBIEGAME_API AmyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AmyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Variables:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player References", meta = (DisplayPriority=1))
		UBasePlayer_AnimInstance* animController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References", meta = (DisplayPriority = 1))
		USceneComponent* TestLocator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		int health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		int Ammo = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		int AmmoInMagazine = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		float BaseDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		int MaxAmmoInMagazine = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		float ShootCadency = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayPriority = 2))
		float MovementSpeed = 1.0f;
	
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABullet> prefabBullet;

	UPROPERTY()
		UAudioComponent* myAudioComp;
	UPROPERTY(EditAnyWhere)
		USoundCue* myShootCue;


private:

	UPROPERTY()
		AZG_GameModeBase* _gameMode;
	
	FTimerHandle _ShootCooldown;
	FTimerHandle _SlowCooldown;

	float CurrentSpeed;
	
	bool Slowed;

	void Shoot();


public:
	//Funciones:

	void MoveHorizontal(float HAxis); //Mueve el player en Horizontal.
	void MoveVertical(float VAxis);   //Mueve el player en Vertical.
	void RotatePlayer(float rot);     //Rotaci�n del Player.
	void StartShooting();             //Bindeado al input al presionar el boton.
	void StopShooting();              //Bindeado al input al soltar el bot�n.
	void Jump();                      //Funci�n que permite hacer un salto.
	void Reload();                    //Funci�n que permite recargar el arma.
	void Die();                       // Funci�n que llamamos al reducir nuestra vida a 0;
	void RespawnPlayer();             // Funci�n que llamamos para avisarle al player que se resetee.
	void GetAndLoadWeapon();          //Funci�n que llamamos al obtener un arma.
	void GetHit(int Damage);
	void GetSlowed();
	void GetRecovered();
	
	UFUNCTION(BlueprintCallable)
		void Heal(int Ammount);

	UFUNCTION(BlueprintCallable)
		void AddBulletsToBackPack(int Bullets);

	UFUNCTION(BlueprintCallable)
		void AddExtraMagazineSlots(int extraBullets);

	UFUNCTION(BlueprintCallable)
		void AddExtraDamage(float extraDamage);

	UFUNCTION(BlueprintCallable)
		void ShootWithTransform(FTransform transform);

	UFUNCTION(BlueprintCallable)
		void ShootWithPositionAndRotation(FVector position, FRotator rotation);
};
