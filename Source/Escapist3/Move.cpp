// Fill out your copyright notice in the Description page of Project Settings.

#include "Move.h"

AMove::AMove(Direction direction, int distance) {
	_direction = direction;
	_distance = std::abs(distance);
}

int AMove::getScore() {
	return _score;
}

void AMove::setScore(int score) {
	_score = score;
}

Direction AMove::getDirection() {
	return _direction;
}

int AMove::getDistance() {
	return _distance;
}

AMove AMove::reverse() {
	int reverseDirectionInt = ((int)_direction + 2) % 4;
	Direction reverseDirection;
	switch (reverseDirectionInt) {
		case 0: {
			reverseDirection = Direction::Up;
			break;
		}
		case 1: {
			reverseDirection = Direction::Left;
			break;
		}
		case 2: {
			reverseDirection = Direction::Down;
			break;
		}
		case 3: {
			reverseDirection = Direction::Right;
			break;
		}
	}
	return *new AMove(reverseDirection, _distance);
}

FVector AMove::ToVector() {
	FVector returnVector;
	if (_direction == Direction::Down) {
		returnVector = FVector(-(_distance * 250), 0, 0); 
	}
	else if (_direction == Direction::Up) {
		returnVector = FVector(_distance * 250, 0, 0);
	}
	else if (_direction == Direction::Left) {
		returnVector = FVector(0, -(_distance * 250), 0);
	}
	else if (_direction == Direction::Right) {
		returnVector = FVector(0, _distance * 250, 0);
	}

	return returnVector;
}

string AMove::ToString() {
	std::ostringstream oss;
	oss << "Direction: ";
	
	if (_direction == Direction::Up) {
		oss << "Up";
	}
	else if (_direction == Direction::Down) {
		oss << "Down";
	}
	else if (_direction == Direction::Left) {
		oss << "Left";
	}
	else if (_direction == Direction::Right) {
		oss << "Right";
	}
	oss << ", Distance: " << _distance;

	return oss.str();
}

AMove::~AMove()
{
}
