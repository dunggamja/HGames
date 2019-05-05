// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Client.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GPPawn.generated.h"

class UMyCollisionComponent;

UCLASS()
class CLIENT_API AGPPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGPPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	FVector		DirectionToMove = FVector::ZeroVector;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UMyCollisionComponent*	Collision;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent*	Mesh;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UFloatingPawnMovement*	Movement;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
};
