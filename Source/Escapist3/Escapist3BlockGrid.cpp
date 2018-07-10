// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Escapist3BlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AEscapist3BlockGrid::AEscapist3BlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	DummyRoot->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	ScoreText->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);
	// Set defaults
	Size = 3;
	BlockSpacing = 250.f;
}

int getNumberOfLinesInFile(std::string filePath) {
	int number_of_lines = 0;
	std::string line;
	std::ifstream file(filePath);

	while (std::getline(file, line))
		++number_of_lines;

	file.close();
	return number_of_lines;
}

std::vector<std::string> split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}


void AEscapist3BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	std::string path = "C:\\Users\\Sean\\Documents\\Chessmate v2\\Assets\\Levels\\TestLevel.txt";
	// Number of blocks
	const int32 NumBlocks = Size * Size;
	int xLength = 1;
	int yLength = getNumberOfLinesInFile(path);
	// read in each lines and parse into Tiles and Pieces
	std::ifstream boardFile;
	boardFile.open(path);

	int numPieces = 0; // TODO remove this line
	for (int x = 0; !boardFile.eof(); x++) {
		std::cout << "Eyyyy ";
		std::string line;
		std::getline(boardFile, line);
		// 'D' is for Dog
		// A 'U' infront of a letter will indicate that this is a users piece
		std::vector<std::string> characters = split(line, ',');

		yLength = characters.size();
		for (int y = 0; y < yLength; y++) {
			std::string character = characters[y];
			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(x * BlockSpacing, y * BlockSpacing, -50.f) + GetActorLocation();
			const FVector PawnLocation = FVector(y * BlockSpacing, x * BlockSpacing, 200.f) + GetActorLocation();
			AEscapist3Block* NewBlock = NULL;
			APieceActor* Pawn = NULL;
			if (character == "0") {
				NewBlock = GetWorld()->SpawnActor<AObstacleBlock>(BlockLocation, FRotator(0, 0, 0));
			}
			// Spawn a block
			else {
				NewBlock = GetWorld()->SpawnActor<AEscapist3Block>(BlockLocation, FRotator(0, 0, 0));
			}

			if (character == "D") {
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				Pawn = GetWorld()->SpawnActor<APieceActor>(PawnLocation, FRotator(0, 0, 0));
				Pawn->Init(numPieces++, x, y);
			}
			// Tell the block about its owner
			if (NewBlock != nullptr) {
				NewBlock->OwningGrid = this;
				NewBlock->Init(x, y);
			}

			//if (character == "0") {
			//	tile = new Obstacle(x, y);
			//}
			//else if (character == "2") {
			//	goal = new GoalTile(x, y);
			//	tile = goal;
			//}
			//else if (character.rfind('U', 0) == 0) {
			//	userPiece = new Dog(x, y);
			//	userPiece->setUserPiece(true);
			//}
			//else if (character == "D") {
			//	aiPieces.push_back(new Dog(x, y));
			//}
			//            } else if (character == "B") {
			//                blocks.Add(new PushBlock(x, y));
			//            }
			tiles.push_back(NewBlock);
		}
	}
}


void AEscapist3BlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
