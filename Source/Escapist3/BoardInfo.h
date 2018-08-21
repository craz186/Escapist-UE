// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h"
#include <vector>
#include "Tile.h"
#include "PieceInfo.h"
#include "MoveResult.h"
#include "GoalTile.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ESCAPIST3_API BoardInfo
{
public:

	std::list<PieceInfo*> _aiPieces;
	PieceInfo* _userPiece;
	std::vector<ATile*> _tiles;
	int _xLength;
	int _yLength;
	AGoalTile* _goal;

	BoardInfo(BoardInfo* boardInfo);
	BoardInfo(APiece* userPiece, std::list<APiece*> aiPieces, std::vector<ATile*> tiles, AGoalTile* goal, int _xLength, int _yLength);

	PieceInfo* getPiece(float x, float y);
	PieceInfo* getUserPiece();
	bool IsValidPosition(int x, int y);
	MoveResult MovePiece(PieceInfo* piece, AMove move);
	BoardInfo* Clone();
	AGoalTile* getGoal();
	~BoardInfo();
};
