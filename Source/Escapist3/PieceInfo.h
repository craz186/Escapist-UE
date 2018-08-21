// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <list>
#include "Move.h"
#include "Point.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ESCAPIST3_API PieceInfo
{
public:
	float x, y;
	std::list<AMove> _validMoves;
	bool _isUserPiece;
	AMove* lastMove = NULL;

	PieceInfo(FVector pieceLocation, std::list<AMove> moves, bool isUserPiece);

	PieceInfo(PieceInfo* another);

	std::list<AMove> GetMoves();
	float getX();
	float getY();
	bool isUserPiece();
	AMove* getLastMove();
	Point* GetPointLocation();
	std::vector<Point> GetAllPointsOnPath(AMove move, float xModifier, float yModifier);
	std::vector<Point> GetAllMoveCoordinatesForMove(AMove move);
	void MovePiece(AMove move);
	PieceInfo* Clone();
	~PieceInfo();
};
