#include <iostream>
#include <ctime>

#include "Board.h"
#include "MCS.h"
#include "Node.h"
#include "Evaluation.h"

#include "../Minimax/ABNegamax/NMMABNegamax.h"
#include "../Minimax/NMMTree.h"

int main()
{
	srand(time(NULL));

	float times[10];
	int player1WonGames = 0, player2WonGames = 0;
	for (int i = 0; i < 10; ++i)
	{
		NMM::MCS::Board board;

		clock_t time = clock();

		while (true)
		{
			if (NMM::MCS::isPlayerLosing(board, 1))
			{
				player2WonGames += 1;
				break;
			}

			NMM::MCS::Node* tree = new NMM::MCS::Node(board);
			tree->generateChildren();

			board = NMM::MCS::monteCarloEvaluation(tree, 20);

			if (NMM::MCS::isPlayerLosing(board, 2))
			{
				player1WonGames += 1;
				delete tree;
				break;
			}

			NMM::Node* treeNegamax = new NMM::Node(board);

			board = NMM::ABnegamaxBestMove(treeNegamax, 2, 4);
			//board = board.randomMove();

			delete treeNegamax;
			delete tree;
		}

		time = clock() - time;
		times[i] = ((float)time) / CLOCKS_PER_SEC;

		std::cout << "Player 1 won games: " << player1WonGames << "\tPlayer 2 won games: " << player2WonGames << "\tTime: " << times[i] << std::endl;
	}

	float sum = 0.0f;
	for (int i = 0; i < 10; ++i)
		sum += times[i];
	std::cout << "Average simulation time: " << sum / 10.0f << std::endl;

	system("pause");
}