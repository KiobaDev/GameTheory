#include <iostream>
#include <ctime>

#include "..\Minimax\NMMBoard.h"
#include "..\Minimax\NMMTree.h"
#include "..\Minimax\ABNegamax\NMMABNegamax.h"
#include "Node.h"
#include "Enums.h"
#include "PNS.h"
#include "Config.h"

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

			NMM::PNS::Node* pnsNode = nullptr;
			if (std::count(b1.board, b1.board + 24, 2) <= 3 && b1.ply > 18)
			{
				pnsNode = new NMM::PNS::Node(b1, NMM::PNS::NodeType::Or, nullptr);
				resetResources();
			}

			testTree->generateChildren(1);
			if (testTree->children.size() == 0)
			{
				playerWon = 2;
				delete pnsNode;
				delete testTree;
				delete tree;
				break;
			}

			if (std::count(b1.board, b1.board + 24, 2) <= 3 && b1.ply > 18)
			{
				NMM::PNS::Node* temp = NMM::PNS::PNS(pnsNode);

				if (temp)
					b1 = temp->board;
			}
			else
				b1 = NMM::ABnegamaxBestMove(tree, 1, depth + 1);

			if (NMM::isPlayerWinning(b1, 1))
			{
				playerWon = 1;
				delete pnsNode;
				delete testTree;
				delete tree;
				break;
			}

			std::vector<NMM::BoardState> possibleMovesForPlayer2 = b1.possibleMoves(2);
			if (possibleMovesForPlayer2.size() == 0)
			{
				playerWon = 1;
				delete pnsNode;
				delete testTree;
				delete tree;
				break;
			}
			b1 = possibleMovesForPlayer2[rand() % possibleMovesForPlayer2.size()];

			if (NMM::isPlayerWinning(b1, 2))
			{
				playerWon = 2;
				delete pnsNode;
				delete testTree;
				delete tree;
				break;
			}

			delete pnsNode;
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