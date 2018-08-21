// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstance.h"
#include <vector>
#include <list>
#include "Move.h"
#include "Point.h"
#include "PieceInfo.h"
#include "Piece.generated.h"

UCLASS()
class ESCAPIST3_API APiece : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APiece();

	void Init(int x, int y, bool isUserPiece);

	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial = (TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"));
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> ClayBrickMaterial = (TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial")); // (TEXT("/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New"));
	UMaterialInstance* OrangeMat;
	UMaterialInstance* ClayBrick;
	int X = 0;
	int Y = 0;
	int LastX = 0;
	int LastY = 0;
	bool _isUserPiece;

	std::list<AMove> _validMoves{
		AMove(Direction::Up, 3),
		AMove(Direction::Down, 3),
		AMove(Direction::Left, 3),
		AMove(Direction::Right, 3),
		AMove(Direction::Up, 2),
		AMove(Direction::Down, 2),
		AMove(Direction::Left, 2),
		AMove(Direction::Right, 2),
		AMove(Direction::Up, 1),
		AMove(Direction::Down, 1),
		AMove(Direction::Left, 1),
		AMove(Direction::Right, 1)
	};

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

	PieceInfo* GetPieceInfo();

	std::vector<Point> GetAllMoveCoordinatesForMove(AMove* move);
	std::vector<Point> GetAllPointsOnPath(AMove* move, float xModifier, float yModifier);
	std::list<AMove> GetMoves();

};
