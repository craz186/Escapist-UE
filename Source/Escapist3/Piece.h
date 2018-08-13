// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstance.h"
#include <vector>
#include "Move.h"
#include "Point.h"
#include "Piece.generated.h"

UCLASS()
class ESCAPIST3_API APiece : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APiece();

	void Init(int x, int y);

	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial = (TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"));
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> ClayBrickMaterial = (TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial")); // (TEXT("/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New"));
	UMaterialInstance* OrangeMat;
	UMaterialInstance* ClayBrick;
	int X = 0;
	int Y = 0;
	int LastX = 0;
	int LastY = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* OurVisibleComponent;

	std::vector<Point> GetAllMoveCoordinatesForMove(AMove* move);
	std::vector<Point> GetAllPointsOnPath(AMove* move, float xModifier, float yModifier);
};
