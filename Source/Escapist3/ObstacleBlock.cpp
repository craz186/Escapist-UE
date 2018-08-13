// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstacleBlock.h"


AObstacleBlock::AObstacleBlock() : ATile(baseMaterial) {}

void AObstacleBlock::Init(int x, int y, bool allowsPiece) {
	Super::Init(x, y, allowsPiece);
}
