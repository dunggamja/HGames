// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Client.h"
#include "Components/SceneComponent.h"
#include "MyCollisionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLIENT_API UMyCollisionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:
	TSharedPtr<Physics::OBBCollider>	Collider;
	
};
