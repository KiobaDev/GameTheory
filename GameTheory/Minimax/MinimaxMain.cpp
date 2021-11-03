#include <iostream>

#include "NMMBoard.h"
#include "NMMTree.h"
#include "NMMMinimax.h"

int main()
{
	srand(time(NULL));

	std::cout << "Input minimax depth: ";
	int depth;
	std::cin >> depth;

	int player1WonGames = 0, player2WonGames = 0, draws = 0;
	for (int j = 0; j < 1000; ++j)
	{
		NMM::BoardState b1;
		int playerWon = 0;
		for (int i = 0; i < 100; ++i)
		{
			NMM::Node* tree = new NMM::Node();
			tree->board = b1;

			tree->generateChildrenToNthDepth(1, depth + 1);
			if (tree->children.size() == 0)
			{
				playerWon = 2;
				delete tree;
				break;
			}
			b1 = NMM::minimaxBestMove(tree, 1, depth + 1);

			if (NMM::isPlayerWinning(b1, 1))
			{
				playerWon = 1;
				delete tree;
				break;
			}

			std::vector<NMM::BoardState> possibleMovesForPlayer2 = b1.possibleMoves(2);
			if (possibleMovesForPlayer2.size() == 0)
			{
				playerWon = 1;
				delete tree;
				break;
			}
			b1 = possibleMovesForPlayer2[rand() % possibleMovesForPlayer2.size()];

			if (NMM::isPlayerWinning(b1, 2))
			{
				playerWon = 2;
				delete tree;
				break;
			}

			delete tree;
		}

		if (playerWon == 1)
			++player1WonGames;
		else if (playerWon == 2)
			++player2WonGames;
		else
			++draws;

		std::cout << "Player 1 won games: " << player1WonGames << "\tPlayer 2 won games: " << player2WonGames << "\tDraws count: " << draws << std::endl;
	}

	system("pause");
}
