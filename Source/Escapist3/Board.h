// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include "ObstacleBlock.h"
#include "Piece.h"
#include "Tile.h"
#include "Move.h"
#include "Point.h"
#include "GoalTile.h"
#include <cmath>
#include "MoveResult.h"
#include "EscapistUtils.h"
#include "BoardInfo.h"
#include "AIManager.h"
#include "Board.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ABoard : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:
	ABoard();

	int _xLength = 0;
	int _yLength = 0;
	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	std::vector<ATile*> tiles;
	APiece* userPiece;
	std::list<APiece*> aiPieces;

	AIManager* aiManager;

	AGoalTile* _goal;


protected:
	
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:
	/** Handle the block being clicked */
	void AddScore();

	void CreateBoard(std::string path);

	void SpawnActor(std::string character, int x, int y);

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }

	AMove* CalculateMoveToBlock(ATile* block);

	MoveResult MovePiece(ATile* block);

	void HandleUserMovement(ATile* clickedBlock);

	bool IsValidPosition(int x, int y);

	MoveResult MovePiece(APiece* piece, AMove move);

	BoardInfo* GetBoardInfo();

	AGoalTile* getGoal();
};