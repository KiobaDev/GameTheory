#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Config.h"

std::map<int, std::vector<int>> possibleMoves =
{
	{0, {1, 3, 9}}, {1, {0, 2, 4}}, {2, {1, 5, 14}}, {3, {0, 4, 10, 6}}, {4, {1, 3, 5, 7}}, {5, {2, 4, 13, 8}},
	{6, {3, 7, 11}}, {7, {6, 8, 4}}, {8, {5, 7 ,12}}, {9, {0, 10, 21}}, {10, {3, 9, 11, 18}}, {11, {6, 10 ,15}},
	{12, {8, 13, 17}}, {13, {5, 12, 14, 20}}, {14, {2, 13, 23}}, {15, {11, 16, 18}}, {16, {15, 17, 19}}, {17, {12, 16, 20}},
	{18, {10, 15, 19, 21}}, {19, {16, 18, 20, 22}}, {20, {13, 17, 19, 23}}, {21, {9, 18, 22}}, {22, {19, 21, 23}}, {23, {22, 20, 14}}
};

std::vector<std::vector<int>> possibleLines =
{
	{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, //up-horizontal
	{0, 9, 21}, {3, 10, 18}, {6, 11, 15}, //left-vertical
	{2, 14, 23}, {5, 13, 20}, {8, 12, 17}, //right-vertical
	{15, 16, 17}, {18, 19, 20}, {21, 22, 23}, //bottom-horizontal
	{1, 4, 7}, {9, 10, 11}, {12, 13, 14}, {16, 19, 22}, //middle-intersections
	{0, 3, 6}, {2, 5, 8}, {17, 20, 23}, {15, 18, 21} //diagonall-intersections
};

std::vector<int> findFreeSpaces(Pawn board[BOARD_SIZE])
{
	std::vector<int> freeSpaces;
	for (int i = 0; i < BOARD_SIZE; ++i)
		if (board[i] == Pawn::none)
			freeSpaces.push_back(i);

	return freeSpaces;
}

std::vector<int> findPlayerPawns(Pawn board[BOARD_SIZE], Pawn player)
{
	std::vector<int> playerPawns;
	for (int i = 0; i < BOARD_SIZE; ++i)
		if (board[i] == player)
			playerPawns.push_back(i);

	return playerPawns;
}

std::vector<std::pair<int, int>> findPossibleMovesInSecondPhase(Pawn board[BOARD_SIZE], Pawn player)
{
	std::vector<std::pair<int, int>> moves;
	for (int i = 0; i < BOARD_SIZE; ++i)
		if (board[i] == player)
		{
			std::vector<int> pawnMoves = possibleMoves[i];
			pawnMoves.erase(std::remove_if(pawnMoves.begin(), pawnMoves.end(), [&](int pawn) { return board[pawn] != Pawn::none; }), pawnMoves.end());

			for (int move : pawnMoves)
				moves.push_back(std::pair<int, int>(i, move));
		}
	
	return moves;
}

std::vector<std::pair<int, int>> findPossibleMovesInThirdPhase(Pawn board[BOARD_SIZE], Pawn player)
{
	std::vector<std::pair<int, int>> moves;
	std::vector<int> freeSpaces = findFreeSpaces(board);

	for(int pawn : findPlayerPawns(board, player))
		for (int space : freeSpaces)
			moves.push_back(std::pair<int, int>(pawn, space));

	return moves;
}

bool isPawnInLine(Pawn board[BOARD_SIZE], int pawnPosition)
{
	for (int i = 0; i < possibleLines.size(); ++i)
		if (std::find(possibleLines[i].begin(), possibleLines[i].end(), pawnPosition) != possibleLines[i].end())
		{
			bool isLineFormed = true;
			for (int j = 0; j < possibleLines[i].size(); ++j)
				if (board[possibleLines[i][j]] != board[pawnPosition])
					isLineFormed = false;

			if (isLineFormed)
				return true;
		}

	return false;
}

bool willPawnBeInLineAfterInsert(Pawn board[BOARD_SIZE], Pawn player, int pawnPosition)
{
	Pawn oldPawn = board[pawnPosition];
	board[pawnPosition] = player;

	bool isPlayerPawnInLine = isPawnInLine(board, pawnPosition);
	board[pawnPosition] = oldPawn;

	return isPlayerPawnInLine;
}

bool willPawnBeInLineAfterMove(Pawn board[BOARD_SIZE], Pawn player, std::pair<int, int> pawnMove)
{
	Pawn oldPawn = board[pawnMove.first];
	board[pawnMove.first] = Pawn::none;
	board[pawnMove.second] = player;

	bool isPlayerPawnInLine = isPawnInLine(board, pawnMove.second);
	board[pawnMove.first] = oldPawn;
	board[pawnMove.second] = Pawn::none;

	return isPlayerPawnInLine;
}

Pawn getOpponent(Pawn playerTurn)
{
	if (playerTurn == Pawn::black)
		return Pawn::white;
	else
		return Pawn::black;
}

void calculateGameComplexityM1()
{
	srand(time(0));

	std::vector<int> turnCounts;
	std::vector<double> averageMovesPerTurn;

	for (int g = 0; g < GAME_COUNT; ++g)
	{
		Pawn board[BOARD_SIZE] = {};

		Pawn playerTurn = Pawn::black;
		bool isGameWon = false;

		std::vector<int> possibleMovesInTurns;

		int turn = 0;

		while (!isGameWon)
		{
			++turn;

			if (turn <= 18)
			{
				//Phase 1
				std::vector<int> freeSpaces = findFreeSpaces(board);

				if (freeSpaces.size() == 0)
					break;

				int space = freeSpaces[rand() % freeSpaces.size()];

				int possibleRemoves = 0;
				for (int i = 0; i < freeSpaces.size(); ++i)
					if (willPawnBeInLineAfterInsert(board, playerTurn, freeSpaces[i]))
						possibleRemoves += findPlayerPawns(board, getOpponent(playerTurn)).size();

				board[space] = playerTurn;

				if (isPawnInLine(board, space))
				{
					std::vector<int> oponentPawns = findPlayerPawns(board, getOpponent(playerTurn));
					space = oponentPawns[rand() % oponentPawns.size()];

					board[space] = Pawn::none;
				}

				possibleMovesInTurns.push_back(freeSpaces.size() + possibleRemoves);
			}
			else
			{
				std::vector<std::pair<int, int>> possibleCurrentMoves;

				if (findPlayerPawns(board, playerTurn).size() > 3)
					possibleCurrentMoves = findPossibleMovesInSecondPhase(board, playerTurn);
				else
					possibleCurrentMoves = findPossibleMovesInThirdPhase(board, playerTurn);

				if (possibleCurrentMoves.size() == 0)
					break;

				std::pair<int, int> move = possibleCurrentMoves[rand() % possibleCurrentMoves.size()];

				int possibleRemoves = 0;
				for (int i = 0; i < possibleCurrentMoves.size(); ++i)
					if (willPawnBeInLineAfterMove(board, playerTurn, possibleCurrentMoves[i]))
						possibleRemoves += findPlayerPawns(board, getOpponent(playerTurn)).size();

				board[move.first] = Pawn::none;
				board[move.second] = playerTurn;

				if (isPawnInLine(board, move.second))
				{
					std::vector<int> oponentPawns = findPlayerPawns(board, getOpponent(playerTurn));
					int space = oponentPawns[rand() % oponentPawns.size()];

					board[space] = Pawn::none;
				}

				possibleMovesInTurns.push_back(possibleCurrentMoves.size() + possibleRemoves);

				if (findPlayerPawns(board, getOpponent(playerTurn)).size() == 2)
					isGameWon = true;
			}

			playerTurn = getOpponent(playerTurn);

			if (turn > 1000)
				break;
		}

		int sum = 0;
		for (int moves : possibleMovesInTurns)
			sum += moves;

		double average = double(sum) / possibleMovesInTurns.size();

		turnCounts.push_back(turn);
		averageMovesPerTurn.push_back(average);
	}

	int averageTurns = 0;
	for (int turns : turnCounts)
		averageTurns += turns;

	double averageMoves = 0;
	for (double moves : averageMovesPerTurn)
		averageMoves += moves;

	std::wcout << "Srednia ilosc tur: " << averageTurns / turnCounts.size() << " Srednia ilosc rochow: " << averageMoves / averageMovesPerTurn.size() << std::endl;
}