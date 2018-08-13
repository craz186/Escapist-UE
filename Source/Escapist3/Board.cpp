// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Board.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ABoard::ABoard()
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

AMove* ABoard::CalculateMoveToBlock(ATile* block) {

	UE_LOG(LogTemp, Warning, TEXT("User location is: %s"), *userPiece->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Block location is: %s"), *block->GetActorLocation().ToString());

	// TODO Need to calculate distance between block and piece
	float xDistanceToBlock = userPiece->GetActorLocation().X - (block->GetActorLocation().X);
	float yDistanceToBlock = userPiece->GetActorLocation().Y - (block->GetActorLocation().Y);
	int xCoord = xDistanceToBlock / BlockSpacing;
	int yCoord = yDistanceToBlock / BlockSpacing;

	AMove* calculatedMove = new AMove(Direction::Down, 0);

	if (xCoord > 0) {
		calculatedMove = new AMove(Direction::Down, xCoord);
	}
	else if (xCoord < 0) {
		calculatedMove = new AMove(Direction::Up, xCoord);
	}
	else if (yCoord < 0) {
		calculatedMove = new AMove(Direction::Right, yCoord);
	}
	else if (yCoord > 0) {
		calculatedMove = new AMove(Direction::Left, yCoord);
	}

	MoveResult moveResult = MoveResult::ValidMove;
	FString moveString(calculatedMove->ToString().c_str());

	UE_LOG(LogTemp, Warning, TEXT("Calculated Move was : %s"), *moveString);

	return calculatedMove;
}

bool ABoard::IsValidPosition(int x, int y) {
	// need to check for obstacles

	return tiles.at((x * (_xLength)) + y)->DoesAllowPiece(); //&& _tiles[(x * _xLength) + y].DoesAllowPiece();
};

MoveResult ABoard::MovePiece(ATile* block) {
	MoveResult moveResult = MoveResult::ValidMove;

	FVector pieceDestination = block->GetActorLocation() + FVector(0, 0, 200);
	UE_LOG(LogTemp, Warning, TEXT("Moving user to location: %s"), *pieceDestination.ToString());
	AMove* filteredMove = CalculateMoveToBlock(block); 

	std::vector<Point> moveCoordinates = userPiece->GetAllMoveCoordinatesForMove(filteredMove);
	for (int i = 0; i < moveCoordinates.size(); i++) {
		Point point = moveCoordinates.at(i);
		// validate move here for obstacles and blocks
		// TODO SG Maybe this should be a valid move but filter it to stop the move early.
		if (!IsValidPosition(point.getX(), point.getY())) {
			UE_LOG(LogTemp, Warning, TEXT("Invalid move"));

			return MoveResult::InvalidMove;
		}
		//todo AIManager
		
		//Block* blockToBeActioned = checkBlockExists(point.getX(), point.getY());
		//if (blockToBeActioned != nullptr) {
			//            if (blockToBeActioned->takeAction(*this, move)) {
			//                filteredMove = *new Move(move.getDirection(), i + 1);
			//                moveResult = MoveResult::BlockInteraction;
			//                break;
			//            } else {
		//	return MoveResult::InvalidMove;
			//            }
		//}
		//else {

		/*
		Piece* pieceToBeTaken = checkPieceExists(point.getX(), point.getY());
		if (pieceToBeTaken == nullptr || *pieceToBeTaken == piece) {
			continue;
		}

		if (pieceToBeTaken->isUserPiece()) {
			// Game over...
			_userPiece = new Dog(-20, -20);
			_userPiece->setUserPiece(true);

			filteredMove = new Move(move.getDirection(), i + 1);
			moveResult = MoveResult::UserPieceTaken;
			break;
		}
		else {
			// TODO need to check that the piece not getting taken is actually the User piece
			_aiManager->removePiece(*pieceToBeTaken);

			filteredMove = new AMove(move.getDirection(), i + 1);
			moveResult = MoveResult::AiPieceTaken;
			break;
		}
		*/
		//}
	}
	UE_LOG(LogTemp, Warning, TEXT("Filtered location is: %s"), *filteredMove->ToVector().ToString());
	userPiece->SetActorLocation(userPiece->GetActorLocation() + filteredMove->ToVector());
	UE_LOG(LogTemp, Warning, TEXT("User location is: %s"), *userPiece->GetActorLocation().ToString());

	return moveResult;
};

/**
*'D' is for Dog
* A 'U' infront of a letter will indicate that this is a users piece
*/
void ABoard::SpawnActor(std::string character, int x, int y) {
	// Make position vector, offset from Grid location
	const FVector BlockLocation = FVector(x * BlockSpacing, y * BlockSpacing, 0.f);// + GetActorLocation();
	const FVector PawnLocation = FVector(x * BlockSpacing, y * BlockSpacing, 200.f);// + GetActorLocation();
	APiece* Pawn = NULL;

	if (character == "0") {
		ATile* obstacle = GetWorld()->SpawnActor<AObstacleBlock>(BlockLocation, FRotator(0, 0, 0));
		obstacle->OwningGrid = this;
		obstacle->Init(x, y, false);
		tiles.push_back(obstacle);
	}
	// Spawn a block
	else {
		ATile* NewBlock = GetWorld()->SpawnActor<ATile>(BlockLocation, FRotator(0, 0, 0));
		NewBlock->OwningGrid = this;
		NewBlock->Init(x, y);
		tiles.push_back(NewBlock);
		UE_LOG(LogTemp, Warning, TEXT("Spawning tile at expected location %s, Tile is at real location: %s"), *BlockLocation.ToString(), *NewBlock->GetActorLocation().ToString());
	}

	if (character == "UD") {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Pawn = GetWorld()->SpawnActor<APiece>(PawnLocation, FRotator(0, 0, 0));
		Pawn->Init(x, y);
		this->userPiece = Pawn;
		UE_LOG(LogTemp, Warning, TEXT("Spawning piece at expected location %s, Tile is at real location: %s"), *BlockLocation.ToString(), *Pawn->GetActorLocation().ToString());

	}
	else if (character == "D") {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Pawn = GetWorld()->SpawnActor<APiece>(PawnLocation, FRotator(0, 0, 0));
		Pawn->Init(x, y);
		aiPieces.push_back(Pawn);
		UE_LOG(LogTemp, Warning, TEXT("Spawning piece at expected location %s, Tile is at real location: %s"), *BlockLocation.ToString(), *Pawn->GetActorLocation().ToString());

	}
	
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

	//_tiles[(x * _xLength) + y] = *NewBlock;
}

void ABoard::CreateBoard(std::string path) {
	_xLength = getNumberOfLinesInFile(path);

	// read in each lines and parse into Tiles and Pieces
	std::ifstream boardFile;
	boardFile.open(path);

	int numPieces = 0; // TODO remove this line
	for (int x = 0; !boardFile.eof(); x++) {
		std::cout << "Eyyyy ";
		std::string line;
		std::getline(boardFile, line);
		std::vector<std::string> characters = split(line, ',');

		_yLength = characters.size();
		for (int y = 0; y < _yLength; y++) {
			std::string character = characters[y];
			SpawnActor(character, x, y);
		}
	}

}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	// TODO Need to change how we read this in
	FString path = FPaths::GameSourceDir().Append("Escapist3/").Append("TestLevel.txt");	
	UE_LOG(LogTemp, Warning, TEXT("TestLevel path is: %s"), *path);

	CreateBoard(std::string(TCHAR_TO_UTF8(*path)));
}


void ABoard::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
