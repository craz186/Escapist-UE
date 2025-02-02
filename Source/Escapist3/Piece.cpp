// Fill out your copyright notice in the Description page of Project Settings.

#include "Piece.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include <iostream>
#include <sstream>

#include "Engine/StaticMesh.h"


// Sets default values
APiece::APiece()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	
	UE_LOG(LogTemp, Warning, TEXT("Is collision enabled for this piece: %d"), RootComponent->IsCollisionEnabled());
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	OurVisibleComponent->SetStaticMesh(PlaneMesh.Get());
	OrangeMat = BlueMaterial.Get();
	ClayBrick = ClayBrickMaterial.Get();
	OurVisibleComponent->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	OurCamera->SetupAttachment(RootComponent);
	OurVisibleComponent->SetupAttachment(RootComponent);
}

void APiece::Init(int x, int y, bool isUserPiece) {
	X = x;
	Y = y;
	_isUserPiece = isUserPiece;
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurVisibleComponent->SetMaterial(0, OrangeMat);
	
	UE_LOG(LogTemp, Warning, TEXT("PieceActor location is: %s"), *(GetActorLocation().ToString()));//+ " is:" +
	
	UE_LOG(LogTemp, Warning, TEXT("Visible component location is: %s"), *(OurVisibleComponent->GetComponentLocation().ToString()));//+ " is:" +

};

std::vector<Point> APiece::GetAllPointsOnPath(AMove* move, float xModifier, float yModifier) {
	auto * returnPoints = new vector<Point>;
	float tempX = GetActorLocation().X / 250;
	float tempY = GetActorLocation().Y / 250;
	for (int i = 0; i < move->getDistance(); i++) {
		tempX += xModifier;
		tempY += yModifier;
		returnPoints->emplace_back(tempX, tempY);
	}
	return *returnPoints;
}

std::vector<Point> APiece::GetAllMoveCoordinatesForMove(AMove* move) {
	std::vector<Point> returnPoints;

	switch (move->getDirection()) {
	case Direction::Up:
		returnPoints = GetAllPointsOnPath(move, 1, 0);
		break;
	case Direction::Down:
		returnPoints = GetAllPointsOnPath(move, -1, 0);
		break;
	case Direction::Left:
		returnPoints = GetAllPointsOnPath(move, 0, -1);
		break;
	case Direction::Right:
		returnPoints = GetAllPointsOnPath(move, 0, 1);
		break;
	}

	return returnPoints;
}

std::list<AMove> APiece::GetMoves() {
	return _validMoves;
}


// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APiece::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Setup Player Input"));

}

PieceInfo* APiece::GetPieceInfo() {
	return new PieceInfo(GetActorLocation(), GetMoves(), _isUserPiece);
}
