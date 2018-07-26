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
	RootComponent->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	//DummyRoot->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
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

MoveResult AEscapist3BlockGrid::MovePiece(AEscapist3Block* block) {
	// Compare distance with users block
	// 
//	UserPiece.
	//userPiece->Move_XAxis();
	//userPiece->Move_YAxis();
	FVector pieceDestination = block->GetActorLocation() + FVector(-1500, 0, 200);
	UE_LOG(LogTemp, Warning, TEXT("Moving user to location: %s"), *pieceDestination.ToString());
	userPiece->SetActorLocation(pieceDestination);
	UE_LOG(LogTemp, Warning, TEXT("User rootcomp location: %s"), *(RootComponent->GetComponentLocation().ToString()));

	// TODO Need to calculate distance between block and user
	float xDistanceToBlock = userPiece->GetActorLocation().X -block->GetActorLocation().X;
	float yDistanceToBlock = userPiece->GetActorLocation().Y - block->GetActorLocation().Y;
	int xCoord = xDistanceToBlock / 250;
	int yCoord = yDistanceToBlock / 250;

	AMove* x;
	//AMove* y;

	if (xCoord > 0) {
		x = new AMove(Direction::Down, xCoord);
	}
	else if (xCoord < 0) {
		x = new AMove(Direction::Down, xCoord);
	}
	MoveResult moveResult = MoveResult::ValidMove;

	// if 
	// TODO Convert result into move list
	// 
	

	/*
	AMove filteredMove = move;
	
	vector<Point> moveCoordinates = piece.getAllMoveCoordinatesForMove(move);
	for (int i = 0; i < moveCoordinates.size(); i++) {
		Point point = moveCoordinates.at(i);
		// validate move here for obstacles and blocks
		// TODO SG Maybe this should be a valid move but filter it to stop the move early.
		if (!isValidPosition(point.getX(), point.getY())) {
			return MoveResult::InvalidMove;
		}

		Block* blockToBeActioned = checkBlockExists(point.getX(), point.getY());
		if (blockToBeActioned != nullptr) {
			//            if (blockToBeActioned->takeAction(*this, move)) {
			//                filteredMove = *new Move(move.getDirection(), i + 1);
			//                moveResult = MoveResult::BlockInteraction;
			//                break;
			//            } else {
			return MoveResult::InvalidMove;
			//            }
		}
		else {
			Piece* pieceToBeTaken = checkPieceExists(point.getX(), point.getY());
			if (pieceToBeTaken == nullptr || *pieceToBeTaken == piece) {
				continue;
			}

			if (pieceToBeTaken->isUserPiece()) {
				// Game over...
				_userPiece = new Dog(-20, -20);
				_userPiece->setUserPiece(true);

				filteredMove = *new Move(move.getDirection(), i + 1);
				moveResult = MoveResult::UserPieceTaken;
				break;
			}
			else {
				// TODO need to check that the piece not getting taken is actually the User piece
				_aiManager->removePiece(*pieceToBeTaken);

				filteredMove = *new Move(move.getDirection(), i + 1);
				moveResult = MoveResult::AiPieceTaken;
				break;
			}
		}
	}
	piece.takeMove(filteredMove);
	return moveResult;
	*/
	return moveResult;
};

/**
*'D' is for Dog
* A 'U' infront of a letter will indicate that this is a users piece
*/
void AEscapist3BlockGrid::SpawnActor(std::string character, int x, int y) {
	// Make position vector, offset from Grid location
	const FVector BlockLocation = FVector(x * BlockSpacing, y * BlockSpacing, 0.f);// + GetActorLocation();
	const FVector PawnLocation = FVector(y * BlockSpacing, x * BlockSpacing, 200.f);// + GetActorLocation();
	AEscapist3Block* NewBlock = NULL;
	APiece* Pawn = NULL;
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
		Pawn = GetWorld()->SpawnActor<APiece>(PawnLocation, FRotator(0, 0, 0));
		Pawn->Init(x, y);
		this->userPiece = Pawn;
	}
	// Tell the block about its owner
	if (NewBlock != nullptr) {
		NewBlock->OwningGrid = this;
		NewBlock->Init(x, y);
		UE_LOG(LogTemp, Warning, TEXT("Spawning tile at expected location %s, Tile is at real location: %s"), *BlockLocation.ToString(), *NewBlock->GetActorLocation().ToString());

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

void AEscapist3BlockGrid::CreateBoard(std::string path) {
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
		std::vector<std::string> characters = split(line, ',');

		yLength = characters.size();
		for (int y = 0; y < yLength; y++) {
			std::string character = characters[y];
			
			SpawnActor(character, x, y);
			
		}
	}
}

void AEscapist3BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	std::string path = "C:\\Users\\Sean\\Documents\\Chessmate v2\\Assets\\Levels\\TestLevel.txt";

	CreateBoard(path);
}


void AEscapist3BlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
