// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Bullet.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "myPlayer.generated.h"

UCLASS()
class ZOMBIEGAME_API AmyPlayer : public ACharacter
{
	GENERATED_BODY()

	/*UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* VR_MuzzleLocation;*/

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int Ammo = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int AmmoInMagazine = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int MaxAmmoInMagazine = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABullet> prefabBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USceneComponent> Dummy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent * DummyComponentTest;

	//Funciones:

	void MoveHorizontal(float HAxis); //Mueve el player en Horizontal.
	void MoveVertical(float VAxis);   //Mueve el player en Vertical.
	void RotatePlayer(float rot);     //Rotación del Player.
	//void Shoot();                     //Función Shoot Original. Dispara desde el centro del Actor Owner.
	void Jump();                      //Función que permite hacer un salto.
	void Reload();                    //Función que permite recargar el arma.
	void Die();                        // Función que llamamos al reducir nuestra vida a 0;
	
	UFUNCTION(BlueprintCallable)
		void Heal(int Ammount);

	UFUNCTION(BlueprintCallable)
		void AddBulletsToBackPack(int Bullets);

	UFUNCTION(BlueprintCallable)
		void AddExtraMagazineSlots(int extraBullets);

	UFUNCTION(BlueprintCallable)
		void ShootWithTransform(FTransform transform);

	UFUNCTION(BlueprintCallable)
		void ShootWithPositionAndRotation(FVector position, FRotator rotation);
};
