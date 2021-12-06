#pragma once

#include <algorithm>

#include "Node.h"
#include "..\Minimax\NMMBoard.h"
#include "Config.h"

namespace NMM
{
	namespace PNS
	{
		int numberOfPlayerPawns(Node* node, int player)
		{
			return std::count(node->board.board, node->board.board + 24, player);
		}

		int numberOfBlockedPawns(Node* node, int player)
		{
			int blockedOpponentsCount = 0;

			for (int i = 0; i < 24; ++i)
			{
				if (node->board.board[i] == player)
				{
					std::vector<int> possibleMoves = possibleBoardMoves[i];

					bool isBlocked = true;
					for (int point : possibleMoves)
					{
						if (node->board.board[point] == 0)
							isBlocked = false;
					}

					if (isBlocked)
						++blockedOpponentsCount;
				}
			}

			return blockedOpponentsCount;
		}

		int isPlayerWinning(Node* node)
		{
			int opponentCount = numberOfPlayerPawns(node, node->board.playerTurn);
			int numberOfBlockedOpponentPawns = numberOfBlockedPawns(node, node->board.playerTurn);

			if ((node->board.ply > 18 && opponentCount < 3) || numberOfBlockedOpponentPawns == opponentCount)
				return 1;
			return 0;
		}

		int isOpponentWinning(Node* node)
		{
			int playerCount = numberOfPlayerPawns(node, node->board.getOpponent(node->board.playerTurn));
			int numberOfBlockedPlayerPawns = numberOfBlockedPawns(node, node->board.getOpponent(node->board.playerTurn));

			if ((node->board.ply > 18 && playerCount < 3) || numberOfBlockedPlayerPawns == playerCount)
				return 1;
			return 0;
		}

		void evaluate(Node* node)
		{
			if (isPlayerWinning(node))
				node->value = GameState::Win;
			else if (isOpponentWinning(node))
				node->value = GameState::Lose;
			else
				node->value = GameState::Unknown;
		}
	}
}