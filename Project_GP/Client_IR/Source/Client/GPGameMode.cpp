// Fill out your copyright notice in the Description page of Project Settings.

#include "GPGameMode.h"
#include "GPPawn.h"
#include "GPPlayerController.h"

AGPGameMode::AGPGameMode()
{
	DefaultPawnClass = AGPPawn::StaticClass();
	PlayerControllerClass = AGPPlayerController::StaticClass();
}


