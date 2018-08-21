// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h"
#include <list>
#include <cstdlib>
#include <cmath>
#include "BoardInfo.h"
#include "PieceInfo.h"
#include "Move.h"
#include "Point.h"
#include "GoalTile.h"
#include "CoreMinimal.h"

class ABoard;
class ESCAPIST3_API AIManager
{

private:
	std::list<APiece*> _aiPieces;
	const int MaxSearchDepth = 4;

	AMove miniMax(BoardInfo* board, PieceInfo* currentPiece, PieceInfo* opponentPiece, int currentDepth);

	void printDepthTabs(int currentDepth);

	int scoreMove(PieceInfo* currentPiece, Point beforePoint, Point afterPoint, AMove* move, BoardInfo* board);

	static int calculateDistanceBetweenTwoPoints(Point first, Point second);

public:
	AIManager(std::list<APiece *> _aiPieces);

	void moveAllAiPieces(ABoard* board, APiece* opponent);

	void getAiPieces(std::list<APiece*>& returnedList);

	void removePiece(APiece &pieceToBeTaken);

};