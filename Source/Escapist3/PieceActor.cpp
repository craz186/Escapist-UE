// Fill out your copyright notice in the Description page of Project Settings.

#include "PieceActor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include <iostream>
#include <sstream>

#include "Engine/StaticMesh.h"


// Sets default values
APieceActor::APieceActor()
{
	

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UE_LOG(LogTemp, Warning, TEXT("Is collision enabled for this piece: %d"), RootComponent->IsCollisionEnabled());
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	OurVisibleComponent->SetStaticMesh(PlaneMesh.Get());
	OrangeMat = BlueMaterial.Get();
	ClayBrick = ClayBrickMaterial.Get();
	//OurVisibleComponent->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	//OurVisibleComponent->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
}

void APieceActor::Init(int playerNum, int x, int y) {
	// Set this pawn to be controlled by the lowest-numbered player

	// Create a dummy root component we can attach things to.
	// Create a camera and a visible object
	if (playerNum % 2 == 1) {
		AutoPossessPlayer = EAutoReceiveInput::Player0;
	}
	OurVisibleComponent->SetRelativeLocation(FVector(x * 250, y * 250, 0.f));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	if (playerNum % 2 == 0) {
		OurVisibleComponent->SetMaterial(0, OrangeMat);
	}
	else {
		OurVisibleComponent->SetMaterial(0, ClayBrick);
	}
	
		//OurVisibleComponent->GetComponentLocation().at(0) << ", " << OurVisibleComponent->GetComponentLocation()[1] << ", " << OurVisibleComponent->GetComponentLocation()[2];
	UE_LOG(LogTemp, Warning, TEXT("Current location is: %s"), *(GetActorLocation().ToString()))//+ " is:" +
}


// Called when the game starts or when spawned
void APieceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			// Grow to double size over the course of one second
			CurrentScale += DeltaTime;
		}
		else
		{
			// Shrink half as fast as we grow
			CurrentScale -= (DeltaTime * 0.5f);
		}
		// Make sure we never drop below our starting size, or increase past double size.
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	}

	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void APieceActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &APieceActor::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &APieceActor::Move_YAxis);
}

void APieceActor::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void APieceActor::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void APieceActor::StartGrowing()
{
	bGrowing = true;
}

void APieceActor::StopGrowing()
{
	bGrowing = false;
}
