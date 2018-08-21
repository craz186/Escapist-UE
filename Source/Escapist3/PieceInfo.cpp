// Fill out your copyright notice in the Description page of Project Settings.

#include "PieceInfo.h"

//todo fix compile error related to multiple constructors, might need to store the piece information as a variable always
PieceInfo::PieceInfo(FVector pieceLocation, std::list<AMove> moves, bool isUserPiece)
{
	x = pieceLocation.X / 250.f;
	y = pieceLocation.Y / 250.f;
	_validMoves = moves;
	_isUserPiece = isUserPiece;
}

PieceInfo::PieceInfo(PieceInfo* another) {
	x = another->x;
	y = another->y;
	_validMoves = another->GetMoves();
	_isUserPiece = another->isUserPiece();
	lastMove = another->lastMove;
}

std::list<AMove> PieceInfo::GetMoves() {
	return _validMoves;
}

float PieceInfo::getX() {
	return x;
}

float PieceInfo::getY() {
	return y;
}

bool PieceInfo::isUserPiece() {
	return _isUserPiece;
}

Point* PieceInfo::GetPointLocation() {
	return new Point(x, y);
}

PieceInfo* PieceInfo::Clone() {
	return new PieceInfo(this);
}

std::vector<Point> PieceInfo::GetAllMoveCoordinatesForMove(AMove move) {
	std::vector<Point> returnPoints;

	switch (move.getDirection()) {
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

std::vector<Point> PieceInfo::GetAllPointsOnPath(AMove move, float xModifier, float yModifier) {
	auto * returnPoints = new vector<Point>;
	float tempX = x;
	float tempY = y;
	for (int i = 0; i < move.getDistance(); i++) {
		tempX += xModifier;
		tempY += yModifier;
		returnPoints->emplace_back(tempX, tempY);
	}
	return *returnPoints;
}

void PieceInfo::MovePiece(AMove move) {
	FVector moveVector = move.ToVector();
	x += moveVector.X / 250.f;
	y += moveVector.Y / 250.f;
	lastMove = &move;
}

AMove* PieceInfo::getLastMove() {
	return lastMove;
}

PieceInfo::~PieceInfo()
{
}
