// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
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

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* NormalGameWidget;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> RespawnWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> PlayerHUDWidget;

private:

	UWorld* _currentWorld;
	FString _currentLevel;
	FTimerHandle respawnFeedBack;

	UUserWidget* _respawnWidgetInstance;
	UUserWidget* _playerHUDWigetInstance;

public:
	void StartPlay() override;

	UFUNCTION(BlueprintCallable)
		void AddPlayerHudToScene();

	void AddPoints(int points);
	void AddPlayerLifes(int ExtraLifes);
	void PlayerDied();
	void RespawnCallBack();
	void RespawnPlayer();
};
