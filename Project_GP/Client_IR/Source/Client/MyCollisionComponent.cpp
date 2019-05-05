// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCollisionComponent.h"


// Sets default values for this component's properties
UMyCollisionComponent::UMyCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TSharedPtr<Physics::AABBCollider> A(new Physics::AABBCollider(gmtl::Vec3f(0.f, 0.f, 0.f), gmtl::Vec3f(1.f, 1.f, 1.f)));
	TSharedPtr<Physics::AABBCollider> B(new Physics::AABBCollider(gmtl::Vec3f(0.5f, 0.f, 0.f), gmtl::Vec3f(1.f, 1.f, 1.f)));

	auto GJKValue = Physics::Collider::CheckGJK(A.Get(), B.Get());

	LOG(Warning, TEXT("GJK : %d"), std::get<0>(GJKValue));

}


// Called every frame
void UMyCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

