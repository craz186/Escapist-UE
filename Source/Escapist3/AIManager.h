// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h"
#include "Board.h"
#include <list>
#include <cmath>
#include "CoreMinimal.h"

class ABoard;
class ESCAPIST3_API AIManager
{

private:
	std::list<APiece*> _aiPieces;
	const int MaxSearchDepth = 2;

	AMove miniMax(ABoard &board, APiece &currentPiece, APiece &opponentPiece, int currentDepth);

	void printDepthTabs(int currentDepth);

	int scoreMove(APiece &currentPiece, Point beforePoint, Point afterPoint, AMove move, ABoard board);

	static int calculateDistanceBetweenTwoPoints(Point first, Point second);

public:
	explicit AIManager(list<APiece *> &_aiPieces);
	~AIManager();

	void moveAllAiPieces(ABoard &board, APiece &opponent);

	void getAiPieces(list<APiece*>& returnedList);

	void removePiece(APiece &pieceToBeTaken);

};