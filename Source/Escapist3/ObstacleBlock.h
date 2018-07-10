// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Escapist3Block.h"
#include "ObstacleBlock.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPIST3_API AObstacleBlock : public AEscapist3Block
{
	GENERATED_BODY()
	
public: 

	AObstacleBlock();

	FString baseMaterial = "/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss";
	
};
