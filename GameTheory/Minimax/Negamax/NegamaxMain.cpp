#include <iostream>

#include "..\NMMBoard.h"
#include "..\NMMTree.h"
#include "NMMNegamax.h"

#include <ctime>

int main()
{
	srand(time(NULL));

	std::cout << "Input negamax depth: ";
	int depth;
	std::cin >> depth;

	float times[10];

	int player1WonGames = 0, player2WonGames = 0, draws = 0;
	for (int j = 0; j < 10; ++j)
	{
		NMM::BoardState b1;
		int playerWon = 0;

		clock_t time = clock();
		for (int i = 0; i < 100; ++i)
		{
			NMM::Node* testTree = new NMM::Node(b1);
			NMM::Node* tree = new NMM::Node(b1);

			testTree->generateChildren(1);
			if (testTree->children.size() == 0)
			{
				playerWon = 2;
				delete testTree;
				delete tree;
				break;
			}
			b1 = NMM::negamaxBestMove(tree, 1, depth + 1);

			if (NMM::isPlayerWinning(b1, 1))
			{
				playerWon = 1;
				delete testTree;
				delete tree;
				break;
			}

			std::vector<NMM::BoardState> possibleMovesForPlayer2 = b1.possibleMoves(2);
			if (possibleMovesForPlayer2.size() == 0)
			{
				playerWon = 1;
				delete testTree;
				delete tree;
				break;
			}
			b1 = possibleMovesForPlayer2[rand() % possibleMovesForPlayer2.size()];

			if (NMM::isPlayerWinning(b1, 2))
			{
				playerWon = 2;
				delete testTree;
				delete tree;
				break;
			}

			delete testTree;
			delete tree;
		}

		if (playerWon == 1)
			++player1WonGames;
		else if (playerWon == 2)
			++player2WonGames;
		else
			++draws;

		time = clock() - time;
		times[j] = ((float)time) / CLOCKS_PER_SEC;

		std::cout << "Player 1 won games: " << player1WonGames << "\tPlayer 2 won games: " << player2WonGames << "\tDraws count: " << draws << std::endl;
	}

	float sum = 0.0f;
	for (int i = 0; i < 10; ++i)
		sum += times[i];
	std::cout << "Average simulation time: " << sum / 10.0f << std::endl;

	system("pause");
}
