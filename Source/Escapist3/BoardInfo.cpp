// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardInfo.h"

BoardInfo::BoardInfo(APiece* userPiece, std::list<APiece*> aiPieces, std::vector<ATile*> tiles, AGoalTile* goal, int xLength, int yLength)
{
	_userPiece = userPiece->GetPieceInfo();
	for (APiece* piece: aiPieces) {
		_aiPieces.push_back(piece->GetPieceInfo());
	}
	_tiles = tiles;
	_xLength = xLength;
	_yLength = yLength;
	_goal = goal;
}

BoardInfo::BoardInfo(BoardInfo* boardInfo) {
	_userPiece = boardInfo->_userPiece->Clone();
	for (PieceInfo* piece : boardInfo->_aiPieces) {
		_aiPieces.push_back(piece->Clone());
	}
	_xLength = boardInfo->_xLength;
	_yLength = boardInfo->_yLength;
	_tiles = boardInfo->_tiles;
	_goal = boardInfo->_goal;
}

PieceInfo* BoardInfo::getPiece(float x, float y) {
	if (_userPiece->getX() == x && _userPiece->getY() == y) {
		return _userPiece;
	}
	for (PieceInfo *piece : _aiPieces) {
		if (piece->getX() == x && piece->getY() == y) {
			return piece;
		}
	}
	return nullptr;
}

MoveResult BoardInfo::MovePiece(PieceInfo* piece, AMove move) {
	MoveResult moveResult = MoveResult::ValidMove;
	std::vector<Point> moveCoordinates = piece->GetAllMoveCoordinatesForMove(move);
	for (int i = 0; i < moveCoordinates.size(); i++) {
		Point point = moveCoordinates.at(i);
		// validate move here for obstacles and blocks
		// TODO SG Maybe this should be a valid move but filter it to stop the move early.
		if (!IsValidPosition(point.getX(), point.getY())) {
			UE_LOG(LogTemp, Warning, TEXT("Invalid move"));

			return MoveResult::InvalidMove;
		}
	}
	piece->MovePiece(move);
	return moveResult;
}

bool BoardInfo::IsValidPosition(int x, int y) {
	// need to check for obstacles
	bool inBounds = x > -1 && x < _xLength && y > -1 && y < _yLength;
	return inBounds && _tiles.at((x * (_xLength)) + y)->DoesAllowPiece(); //&& _tiles[(x * _xLength) + y].DoesAllowPiece();
};

PieceInfo* BoardInfo::getUserPiece() {
	return _userPiece;
}

BoardInfo* BoardInfo::Clone() {
	return new BoardInfo(this);
}

AGoalTile* BoardInfo::getGoal() {
	return _goal;
}

BoardInfo::~BoardInfo()
{
}
