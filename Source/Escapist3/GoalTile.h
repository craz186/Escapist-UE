// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "Point.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ESCAPIST3_API AGoalTile : public ATile
{
public:

	AGoalTile();

	//void Init(int x, int y, bool allowsPiece = true);
	FString baseMaterial = "/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished";	
	~AGoalTile();
};
