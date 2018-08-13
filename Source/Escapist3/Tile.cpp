// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Tile.h"
#include "Board.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

ATile::ATile()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	//BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	//BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATile::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATile::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();

	UE_LOG(LogTemp, Warning, TEXT("Current location is: %s"), *(GetActorLocation().ToString()))//+ " is:" +

}

ATile::ATile(FString& baseMaterial)
{
	// Structure to hold one-time initialization
	
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterialOpt(TEXT("/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss"));
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterialOpt(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"));
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterialOpt(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"));

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	
	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(PlaneMesh.Get());
	//BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	//BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	BlockMesh->SetMaterial(0, BaseMaterialOpt.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATile::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATile::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = BaseMaterialOpt.Get();
	BlueMaterial = BlueMaterialOpt.Get();
	OrangeMaterial = OrangeMaterialOpt.Get();
	
}

void ATile::Init(int x, int y, bool allowsPiece) {
	_allowsPiece = allowsPiece;
	RootComponent->SetRelativeLocation(FVector(x * 250, y * 250, 0.f));
}

void ATile::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void ATile::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ATile::HandleClicked()
{
	// Call back to the BlockGrid indicating to move a piece here
	OwningGrid->MovePiece(this);



	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->AddScore();
		}
	}
}

void ATile::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
}

bool ATile::DoesAllowPiece() {
	return _allowsPiece;
}
