#pragma once

#include <algorithm>

#include "NMMBoard.h"

namespace NMM
{
	int numberOfPlayerPawns(const NMM::BoardState& board, int player) 
	{ 
		return std::count(board.board, board.board + 24, player); 
	}


	int numberOfMorrises(const NMM::BoardState& board, int player)
	{
		std::vector<std::vector<int>> morrises;

		for (int i = 0; i < 24; ++i)
		{
			if (board.board[i] == player)
			{
				std::vector<int>* ptrToPossibleLines = &possibleBoardLines[i];

				for (int j = 0; j < ptrToPossibleLines->size(); j += 2)
					if (board.board[(*ptrToPossibleLines)[j]] == player && board.board[(*ptrToPossibleLines)[j + 1]] == player)
					{
						std::vector<int> morris = {i, (*ptrToPossibleLines)[j], (*ptrToPossibleLines)[j + 1]};
						std::sort(morris.begin(), morris.end());

						bool skip = false;
						for (std::vector<int> temp : morrises)
							if (temp == morris)
								skip = true;
						if (skip)
							continue;

						morrises.push_back(morris);
					}
			}
		}

		return morrises.size();
	}

	int closedMorris(const NMM::BoardState& previousBoard, const NMM::BoardState& board, int player)
	{
		if (numberOfMorrises(previousBoard, player) < numberOfMorrises(board, player))
			return 1;
		return 0;
	}

	int numberOfBlockedOpponents(const NMM::BoardState& board, int player)
	{
		int blockedOpponentsCount = 0;

		for (int i = 0; i < 24; ++i)
		{
			if (board.board[i] == board.getOpponent(player))
			{
				std::vector<int> possibleMoves = possibleBoardMoves[i];

				bool isBlocked = true;
				for (int point : possibleMoves)
				{
					if (board.board[point] == 0)
						isBlocked = false;
				}

				if (isBlocked)
					++blockedOpponentsCount;
			}
		}

		return blockedOpponentsCount;
	}

	int isPlayerWinning(const NMM::BoardState& board, int player)
	{
		int opponentCount = numberOfPlayerPawns(board, board.getOpponent(player));

		if (board.ply > 18 && opponentCount < 3)
			return 1;
		return 0;
	}

	int NMMEvaluation(const NMM::BoardState& previousBoard, const NMM::BoardState& board, int player)
	{
		if (board.ply <= 18)
		{
			return 18 * (closedMorris(previousBoard, board, player) - closedMorris(previousBoard, board, board.getOpponent(player))) +
				26 * (numberOfMorrises(board, player) - numberOfMorrises(board, board.getOpponent(player))) +
				1 * (numberOfBlockedOpponents(board, player) - numberOfBlockedOpponents(board, board.getOpponent(player))) +
				9 * (numberOfPlayerPawns(board, player) - numberOfPlayerPawns(board, board.getOpponent(player)));
		}
		else if (numberOfPlayerPawns(board, player) == 3)
		{
			return 16 * (closedMorris(previousBoard, board, player) - closedMorris(previousBoard, board, board.getOpponent(player))) +
				 1190 * (isPlayerWinning(board, player) - isPlayerWinning(board, board.getOpponent(player)));
		}
		else
		{
			return 14 * (closedMorris(previousBoard, board, player) - closedMorris(previousBoard, board, board.getOpponent(player))) +
				43 * (numberOfMorrises(board, player) - numberOfMorrises(board, board.getOpponent(player))) +
				10 * (numberOfBlockedOpponents(board, player) - numberOfBlockedOpponents(board, board.getOpponent(player))) +
				11 * (numberOfPlayerPawns(board, player) - numberOfPlayerPawns(board, board.getOpponent(player))) +
				1086 * (isPlayerWinning(board, player) - isPlayerWinning(board, board.getOpponent(player)));
		}
	}
}

//Evaluation function for Phase 1 = 18 * (1) + 26 * (2) + 1 * (3) + 9 * (4) + 10 * (5) + 7 * (6)

//Evaluation function for Phase 2 =  + 8 * (7) + 1086 * (8)

//Evaluation function for Phase 3 = 16 * (1) + 10 * (5) + 1 * (6) + 1190 * (8)