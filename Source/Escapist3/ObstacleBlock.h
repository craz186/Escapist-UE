// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "ObstacleBlock.generated.h"

UCLASS()
class ESCAPIST3_API AObstacleBlock : public ATile
{
	GENERATED_BODY()
	
public: 

	AObstacleBlock();

	void Init(int x, int y, bool allowsPiece = false);

	FString baseMaterial = "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss";
	
};