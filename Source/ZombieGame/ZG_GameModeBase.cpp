// Fill out your copyright notice in the Description page of Project Settings.


#include "ZG_GameModeBase.h"
#include "myPlayer.h"

void AZG_GameModeBase::StartPlay()
{
	Super::StartPlay();
	//UE_LOG(LogTemp, Warning, TEXT("CustomGameMode::StartPlay()"));
}

void AZG_GameModeBase::UpdateTrackedGameTime(float time)
{
	if (!lockTimeTrack)
		GameTime += time;
}

void AZG_GameModeBase::AddPoints(int points)
{
	this->Score += points;
	//TestEvent_Implementation(); //Esto permite llamar la parte c++ de una custom event.
}

void AZG_GameModeBase::AddPlayerLifes(int ExtraLifes)
{
	this->RemainingLifes += ExtraLifes;
}

void AZG_GameModeBase::PlayerDied()
{
	this->RemainingLifes--;

	if (this->RemainingLifes <= 0)
	{
		//Cargo la data resultante en el GameInstance.
		UZG_GameInstance* gameInstance = Cast<UZG_GameInstance>(GetWorld()->GetGameInstance());
		gameInstance->AcumulatedPoints = Score;
		gameInstance->TimeSurvived = GameTime;

		if (_playerHUDWigetInstance && _playerHUDWigetInstance->IsInViewport())//Remuevo el Widget del player.
			_playerHUDWigetInstance->RemoveFromViewport();

		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		controller->bShowMouseCursor = true;          //Muestro el ratón.
		controller->SetInputMode(FInputModeUIOnly()); //Seteo el input a UI Only.

		if (_playerDefeatWigetInstance) //Muestro el Widget de Derrota.
		{
			_playerDefeatWigetInstance->AddToViewport();
			if (_playerDefeatWigetInstance->playerDefeatWidgetAnimation)
				_playerDefeatWigetInstance->PlayAnimation(_playerDefeatWigetInstance->playerDefeatWidgetAnimation);
		}
	}
	else //Si el jugador murió, reespawneo el game.
	{
		if (_respawnWidgetInstance && !_respawnWidgetInstance->IsInViewport())
			_respawnWidgetInstance->AddToViewport();//Cambio el HUD del Player por defecto y muestro un tiempo de respawn.
		GetWorld()->GetTimerManager().SetTimer(respawnFeedBack, this, &AZG_GameModeBase::RespawnCallBack, 1.0f, true, 1.0f);
	}
}

void AZG_GameModeBase::RespawnCallBack()
{
	//Resto 1 al preview y actualizo el canvas correspondiente.
	RespawnTimeRemaining--;

	//UE_LOG(LogTemp, Warning, TEXT("Faltan %i segundos para revivir"), RespawnTimeRemaining);

	//Si el tiempo se redujo a 0 respawneo al Player.
	if (RespawnTimeRemaining == 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(respawnFeedBack);
		RespawnTimeRemaining = 5;
		RespawnPlayer();
	}
}

void AZG_GameModeBase::RespawnPlayer()
{
	//Respawneamos al player.
	if (DefaultPlayerStart != nullptr)
	{
		AmyPlayer* player = Cast<AmyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (player != nullptr)
		{
			player->SetActorLocation(DefaultPlayerStart->GetActorTransform().GetLocation());
			player->RespawnPlayer();
		}
	}
	if (_respawnWidgetInstance)
		_respawnWidgetInstance->RemoveFromParent();
}

void AZG_GameModeBase::AddPlayerHudToScene()
{
	if (_playerHUDWigetInstance == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CustomGameMode::AddPlayerToSceene::El hud es nulo, voy a crear una nueva instancia."));
		_playerHUDWigetInstance = CreateWidget(GetWorld(), PlayerHUDWidget);
		_playerHUDWigetInstance->AddToViewport();
	}
}

void AZG_GameModeBase::ZombieSpawned()
{
	//UE_LOG(LogTemp, Warning, TEXT("================================== AZG_GameModeBase::ZombieSpawned() ======================================"));
	_activeZombies++;
	TotalZombiesSpawnedInCurrentWave++;
	spawnLock = !(TotalZombiesSpawnedInCurrentWave < zombiesPerRound[(WaveNumber - 1)]);
	
	//if (spawnLock) UE_LOG(LogTemp, Warning, TEXT("spawn Lock modified to true"))
	//else UE_LOG(LogTemp, Warning, TEXT("spawn Lock modified to false"));
	//UE_LOG(LogTemp, Warning, TEXT("==========================================================================================================="));
}

void AZG_GameModeBase::ZombieDied()
{
	//UE_LOG(LogTemp, Warning, TEXT("================================== AZG_GameModeBase::ZombieDied() ======================================"));
	_activeZombies--;
	RemainingZombiesToEndCurrentWave--;
	if (RemainingZombiesToEndCurrentWave <= 0)
		WaveEnded();
	//UE_LOG(LogTemp, Warning, TEXT("==========================================================================================================="));
}

bool AZG_GameModeBase::CanSpawnNewZombies()
{
	//UE_LOG(LogTemp, Warning, TEXT("================================== AZG_GameModeBase::CanSpawnNewZombies() ======================================"));
	bool canSpawn = _activeZombies < SceneSpawnLimit && RemainingZombiesToEndCurrentWave > 0 && !spawnLock;
	
	//if (spawnLock) UE_LOG(LogTemp, Warning, TEXT("Spawn is Locked!!"))
	//else UE_LOG(LogTemp, Warning, TEXT("CanSpawn is UnLocked."));
	//if (canSpawn) UE_LOG(LogTemp, Warning, TEXT("Result is True."))
	//else UE_LOG(LogTemp, Warning, TEXT("Result is False."));

	//UE_LOG(LogTemp, Warning, TEXT("_activeZombies: %i, SceneSpawnLimit: %i, RemainingZombiesToEndCurrentWave: %i"), _activeZombies, SceneSpawnLimit, RemainingZombiesToEndCurrentWave);
	//UE_LOG(LogTemp, Warning, TEXT("==========================================================================================================="));

	return canSpawn;
}

//Esto es como se hace un Evento personalizado! Para que funcione debemos hacer la version _Implementation!
//Compilar sin _Implementation tira error!. Si queremos encadenar la implementación c++ con el de blueprint llamamos a la base explicitamente dentro de _Implementation.
//void AZG_GameModeBase::TestEvent()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Esto es evento de debug! PARTE 2"));
//}
//void AZG_GameModeBase::TestEvent_Implementation()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Esto es evento de debug!"));
//	AZG_GameModeBase::TestEvent();
//}

void AZG_GameModeBase::WaveEnded()
{
	if (_playerHUDWigetInstance && _playerHUDWigetInstance->IsInViewport())
		_playerHUDWigetInstance->RemoveFromViewport();
	if (_waveCompleteWidgetInstance && !_waveCompleteWidgetInstance->IsInViewport())
		_waveCompleteWidgetInstance->AddToViewport();

	if (zombiesPerRound.Num() - WaveNumber <= 0)
		LevelCompleted(); //Termino el nivel.
	else //Inicio el contador para la siguiente Oleada.
	{
		lockTimeTrack = true;
		TimeRemainingForNextWave = TimeBetweenWaves;
		GetWorld()->GetTimerManager().SetTimer(WaveStartCounter, this, &AZG_GameModeBase::WaveEndCallback, 1.0f, true, 1.0f);
	}
}

void AZG_GameModeBase::WaveEndCallback()
{
	TimeRemainingForNextWave--;
	if (TimeRemainingForNextWave == 0)
	{
		TimeRemainingForNextWave = TimeBetweenWaves;
		WaveStart();
		GetWorld()->GetTimerManager().ClearTimer(WaveStartCounter);
	}
}

void AZG_GameModeBase::WaveStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("AZG_GameModeBase::WaveStart()"))
	WaveNumber++;          //Actualizo la oleada.
	spawnLock = false;     //Desbloqueo el spawn de zombies.
	lockTimeTrack = false; //Desbloqueo el track del tiempo.
	_activeZombies = 0;//Actualizo la cantidad de zombies spawneados y el límite x escena.
	TotalZombiesSpawnedInCurrentWave = 0;

	if (waveSpawnLimits.Num() > 0)
		SceneSpawnLimit = waveSpawnLimits[(WaveNumber - 1)];	
	if (zombiesPerRound.Num() > 0) //Actualizo la cantidad de zombies que faltan para terminar la oleada.
		RemainingZombiesToEndCurrentWave = zombiesPerRound[(WaveNumber - 1)];

	if (_levelStartWidgetInstance && _levelStartWidgetInstance->IsInViewport())
		_levelStartWidgetInstance->RemoveFromViewport();
	if (_playerHUDWigetInstance && !_playerHUDWigetInstance->IsInViewport())
		_playerHUDWigetInstance->AddToViewport();
	if (_waveCompleteWidgetInstance && _waveCompleteWidgetInstance->IsInViewport())
		_waveCompleteWidgetInstance->RemoveFromViewport();
}

void AZG_GameModeBase::LevelCompleted()
{
	lockTimeTrack = true;
	spawnLock = true; //Bloqueo el spawn. Por si las dudas.
	if (_levelEndWidgetInstance && !_levelEndWidgetInstance->IsInViewport()) //Activo el widget del final de nivel!.
		_levelEndWidgetInstance->AddToViewport();
	if (_waveCompleteWidgetInstance && _waveCompleteWidgetInstance->IsInViewport())
		_waveCompleteWidgetInstance->RemoveFromViewport();

	TimeRemainingToExitLevel = LevelEndDisplayTime;
	GetWorld()->GetTimerManager().SetTimer(WaveFinishCounter, this, &AZG_GameModeBase::EndGameCallback, 1.0f, true, 0.0f);
}

void AZG_GameModeBase::EndGameCallback()
{
	TimeRemainingToExitLevel--;
	if (TimeRemainingToExitLevel <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaveFinishCounter);
		if (_levelEndWidgetInstance && _levelEndWidgetInstance->IsInViewport()) //Desactivo el widget del final de nivel!.
			_levelEndWidgetInstance->RemoveFromViewport();
		if (_currentWorld)
			UGameplayStatics::OpenLevel(GetWorld(), "/Game/Levels/WinLevel", false);//Cargo la escena de Victoria. Por ahora.
	}
}

void AZG_GameModeBase::SetLevelFlow(TArray<int> AmmountOfZombiesPerWave, TArray<int> ZombieSpawnLimitPerRound)
{
	zombiesPerRound = AmmountOfZombiesPerWave;
	waveSpawnLimits = ZombieSpawnLimitPerRound;
}

void AZG_GameModeBase::StartLevel()
{
	//UE_LOG(LogTemp, Warning, TEXT("AZG_GameModeBase::StartLevel()"));
	_currentWorld = GetWorld();
	_currentLevel = _currentWorld->GetMapName();
	lockTimeTrack = false;

	if (RespawnWidget && _respawnWidgetInstance == nullptr)
		_respawnWidgetInstance = CreateWidget(GetWorld(), RespawnWidget);
	if (PlayerHUDWidget && _playerHUDWigetInstance == nullptr)
		_playerHUDWigetInstance = CreateWidget(GetWorld(), PlayerHUDWidget);
	if (PlayerDefeatWidget && _playerDefeatWigetInstance == nullptr)
		_playerDefeatWigetInstance =  Cast<UPlayerDefeatWidget>(CreateWidget(GetWorld(), PlayerDefeatWidget));
	if (WaveCompleteWidget && _waveCompleteWidgetInstance == nullptr)
		_waveCompleteWidgetInstance = CreateWidget(GetWorld(), WaveCompleteWidget);
	if (LevelStartWidget && _levelStartWidgetInstance == nullptr)
		_levelStartWidgetInstance = CreateWidget(GetWorld(), LevelStartWidget);
	if (LevelEndWidget && _levelEndWidgetInstance == nullptr)
		_levelEndWidgetInstance = CreateWidget(GetWorld(), LevelEndWidget);

	if (_levelStartWidgetInstance)
	{
		_levelStartWidgetInstance->AddToViewport();
		TimeRemainingForNextWave = StartingWaveDelay;
		//UE_LOG(LogTemp, Warning, TEXT("AZG_GameModeBase::StartLevel::Widget Exists and Start Level Timer"));
		GetWorld()->GetTimerManager().SetTimer(WaveStartCounter, this, &AZG_GameModeBase::WaveEndCallback, 1.0f, true, 0.0f);
	}
}
