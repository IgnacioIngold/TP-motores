// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "ZG_GameInstance.h"
#include "PlayerDefeatWidget.h"
#include "SpawnManagerBase.h"
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
		int RespawnTimeRemaining = 5;  //CountDown para el respawn del jugador.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> LevelStartWidget; //HUD Inicial del Nivel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> LevelEndWidget; //HUD Final del Nivel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> RespawnWidget;  //HUD del player al morir.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPlayerDefeatWidget> PlayerDefeatWidget;  //HUD del player al morir por tercera vez.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> WaveCompleteWidget; //HUD del player al completar una oleada
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> PlayerHUDWidget; //HUD normal del player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASpawnManagerBase> _spawnManagerClass;
	UPROPERTY(EditAnywhere)
		ASpawnManagerBase* _spawnManager;

	UPROPERTY(BlueprintReadOnly, category = "GameFlow")
		int WaveNumber = 0;                       //Número de Oleada.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "GameFlow")
		int RemainingZombiesToEndCurrentWave = 0; //Zombies que faltan para que la oleada actual termine.
	UPROPERTY(BlueprintReadOnly, category = "GameFlow")
		int TotalZombiesSpawnedInCurrentWave = 0; //Zombies spawneados en total desde que inició la oleada actual.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "GameFlow")
		int SceneSpawnLimit = 200;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int> zombiesToKill; //Cantidad de zombies a eliminar para completar cada oleada. Usa .Num() para obtener el count!.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int> maxActiveZombiesInScenePerWave; //Límite máximo de zombies activos en escena.

	//Wave Flow.
	UPROPERTY(EditAnywhere)
		float TimeBetweenWaves = 10.0f; //Tiempo antes de que inicie la siguiente oleada.
	UPROPERTY(BlueprintReadOnly)
		int StartingWaveDelay = 5; //Tiempo restante para iniciar la siguiente oleada. En segundos.
	UPROPERTY(BlueprintReadOnly)
		int TimeRemainingForNextWave = 0; //Tiempo restante para iniciar la siguiente oleada. En segundos.

	//Level Flow.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName nextLevelPath = "";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int LevelStartDisplayTime = 10;//Tiempo para iniciar la Partida. En segundos.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int LevelEndDisplayTime = 10;  //Tiempo en que se muestra el resultado antes de Salir del nivel. En segundos.
	UPROPERTY(BlueprintReadOnly)
		int TimeRemainingToExitLevel = 0;//CountDown antes de salir de un nivel.

private:

	UPROPERTY()
		UWorld* _currentWorld;
	UPROPERTY()
		UUserWidget* _levelStartWidgetInstance;
	UPROPERTY()
		UUserWidget* _levelEndWidgetInstance;
	UPROPERTY()
		UUserWidget* _respawnWidgetInstance;
	UPROPERTY()
		UUserWidget* _playerHUDWigetInstance;
	UPROPERTY()
		UPlayerDefeatWidget* _playerDefeatWigetInstance;
	UPROPERTY()
		UUserWidget* _waveCompleteWidgetInstance;

	int _activeZombies = 0; //Zombies activos en escena.
	bool lockTimeTrack = true; //El tiempo de supervivencia se cuenta mientras el lock sea falso o el Player este muerto.
	UPROPERTY(EditAnywhere)
		bool lockSpawning = true; //Bloqueo del spawn.

	FString _currentLevel;
	FTimerHandle respawnFeedBack;
	FTimerHandle WaveStartCounter;
	FTimerHandle WaveFinishCounter;

	//Testiiiing.
	/*FInputModeUIOnly UIinputmode;
	FInputModeGameOnly gameInputMode;*/

public:
	void StartPlay() override;

	UFUNCTION(BlueprintCallable)
		void AddPlayerHudToScene();
	UFUNCTION(BlueprintCallable)
		void AddPoints(int points);

	/*UFUNCTION(BlueprintNativeEvent, category = "Tests")
		void TestEvent();
	void TestEvent_Implementation();*/

	void AddPlayerLifes(int ExtraLifes);
	void UpdateTrackedGameTime(float time);

	void PlayerDied();
	void RespawnCallBack();
	void RespawnPlayer();

	UFUNCTION(BlueprintCallable)
		void WaveStart();//Se llama al inicio del nivel y cada vez que una oleada es terminada satisfactoriamente.
	void WaveEndCallback(); //Hace un conteo, al final inicia la nueva oleada.
	void WaveEnded(); //Se llama cuando el último zombie, correspondiente a la oleada actual muere (Wave End).
	
	void ZombieDied(bool willRespawn);     //Registra la acción de la muerte.
	void ZombieSpawned(bool isReespawning);//Registra la acción del spawneo.
	bool CanSpawnNewZombies();
	AZombie* getRespawnableZombie();   //Retorna nullptr si el respawnManager No tiene zombies reespawneables.
	void registerZombieToRespawnList(AZombie* reespawneable);//Registra en el respawnManager un zombie que puede ser reespawneable.

	UFUNCTION(BlueprintCallable)
		void SetLevelFlow(TArray<int> _ammountOfZombiesToKill, TArray<int> _maxActiveZombiesInScene); //Permite que cada nivel pueda setear sus settings al inicio del mismo.
	UFUNCTION(BlueprintCallable)
		void StartLevel(); //LLamamos Start level desde el Level Blueprint cada vez que se carga un nuevo nivel.
	UFUNCTION(BlueprintCallable)
		void LoadPreviousData(int acumulatedScore, float survivedTime); //LLamamos esta función en cada nivel consecutivo del primero!

	void LevelCompleted(); //Se llama al finalizar todas las Oleadas.
	void EndGameCallback(); //Se llama cada 1 segundo al finalizar las oleadas: Muestra el resultado del nivel.
};
