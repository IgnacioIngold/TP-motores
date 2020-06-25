// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "ZG_GameInstance.h"
#include "ZG_GameModeBase.generated.h"

class AmyPlayer;

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AZG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int RemainingLifes = 3;

	UPROPERTY(BlueprintReadOnly)
		int Score = 0;

	UPROPERTY(BlueprintReadOnly)
		float GameTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* DefaultPlayerStart;

	UPROPERTY(BlueprintReadOnly)
		int RespawnTimeRemaining = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ZombieSpawnLimit = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> RespawnWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> PlayerHUDWidget;

private:

	UPROPERTY()
		UWorld* _currentWorld;
	UPROPERTY()
		UUserWidget* _respawnWidgetInstance;
	UPROPERTY()
		UUserWidget* _playerHUDWigetInstance;

	UPROPERTY()
		int _CurrentZombieSpawnedAmmount = 0;

	FString _currentLevel;
	FTimerHandle respawnFeedBack;

public:
	void StartPlay() override;

	UFUNCTION(BlueprintCallable)
		void AddPlayerHudToScene();

	void AddPoints(int points);
	void AddPlayerLifes(int ExtraLifes);
	void PlayerDied();
	void RespawnCallBack();
	void RespawnPlayer();
	void ZombieDied();
	void ZombieSpawned();
	bool CanSpawnNewZombies();
};
