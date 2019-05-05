// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Client.h"
#include "GameFramework/Character.h"
#include "GPCharacter.generated.h"



UCLASS()
class CLIENT_API AGPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
