// Fill out your copyright notice in the Description page of Project Settings.

#include "GPPawn.h"
#include "MyCollisionComponent.h"


// Sets default values
AGPPawn::AGPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Collision = CreateDefaultSubobject<UMyCollisionComponent>(TEXT("COLLISION"));

	RootComponent = Collision;
	Mesh->SetupAttachment(Collision);
	SpringArm->SetupAttachment(Collision);
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MESH_SPHERE(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UMaterial>		MAT_SPHERE(TEXT("/Game/Geometry/Meshes/CubeMaterial.CubeMaterial"));

	if (MESH_SPHERE.Succeeded())	Mesh->SetStaticMesh(MESH_SPHERE.Object);
	if (MAT_SPHERE.Succeeded())		Mesh->SetMaterial(0, MAT_SPHERE.Object);


	//=============================================================================================================
	//	Pitch	Rotation around the right axis(around Y axis), Looking up and down(0 = Straight Ahead, +Up, -Down)
	//	Roll	Rotation around the forward axis(around X axis), Tilting your head, 0 = Straight, +Clockwise, -CCW.
	//	Yaw		Rotation around the up axis(around Z axis), Running in circles 0 = East, +North, -South.
	//=============================================================================================================

	SpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AGPPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LOG(Warning, TEXT("%f, %f Direction"), DirectionToMove.X, DirectionToMove.Y);
	AddMovementInput(DirectionToMove);
}

// Called to bind functionality to input
void AGPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AGPPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AGPPawn::LeftRight);
}



void AGPPawn::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;

	LOG(Warning, TEXT("%f, %f Direction"), DirectionToMove.X, DirectionToMove.Y);
}

void AGPPawn::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
	LOG(Warning, TEXT("%f, %f Direction"), DirectionToMove.X, DirectionToMove.Y);
}