// Fill out your copyright notice in the Description page of Project Settings.


#include "ZG_GameModeBase.h"
#include "myPlayer.h"

void AZG_GameModeBase::StartPlay()
{
	Super::StartPlay();
	_currentWorld = GetWorld();
	_currentLevel = _currentWorld->GetMapName();
	UE_LOG(LogTemp, Warning, TEXT("CustomGameMode::StartPlay::Referencias seteadas"));

	if (RespawnWidget && _respawnWidgetInstance == nullptr)
		_respawnWidgetInstance = CreateWidget(GetWorld(), RespawnWidget);
	if (PlayerHUDWidget && _playerHUDWigetInstance == nullptr)
		_playerHUDWigetInstance = CreateWidget(GetWorld(), PlayerHUDWidget);
}

void AZG_GameModeBase::AddPoints(int points)
{
	this->Score += points;
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

		//Cargo la escena de derrota.
		if (_currentWorld)
			UGameplayStatics::OpenLevel(GetWorld(), "/Game/Levels/DefeatLevel", false);
	}
	else
	{
		//Si el jugador murió, reespawneo el game.
		//Cambio el HUD del Player por defecto y muestro un tiempo de respawn.
		_respawnWidgetInstance->AddToViewport();

		UE_LOG(LogTemp, Warning, TEXT("El boludo del player se murio una vez"));
		GetWorld()->GetTimerManager().SetTimer(respawnFeedBack, this, &AZG_GameModeBase::RespawnCallBack, 1.0f, true, 1.0f);
	}
}

void AZG_GameModeBase::RespawnCallBack()
{
	//Resto 1 al preview y actualizo el canvas correspondiente.
	RespawnTimeRemaining--;

	UE_LOG(LogTemp, Warning, TEXT("Faltan %i segundos para revivir"), RespawnTimeRemaining);

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
			player->SetActorTransform(DefaultPlayerStart->GetActorTransform());
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
		UE_LOG(LogTemp, Warning, TEXT("CustomGameMode::AddPlayerToSceene::El hud es nulo, voy a crear una nueva instancia."));
		_playerHUDWigetInstance = CreateWidget(GetWorld(), PlayerHUDWidget);
		_playerHUDWigetInstance->AddToViewport();
	}
}

