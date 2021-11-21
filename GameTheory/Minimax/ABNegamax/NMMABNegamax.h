#pragma once

#include <algorithm>

#include "..\NMMTree.h"
#include "..\NMMEvaluation.h"

namespace NMM
{
	int ABnegamax(NMM::Node* parent, NMM::Node* tree, int depth, int player, int alpha, int beta)
	{
		if (depth == 1 || isPlayerWinning(tree->board, player))
			return NMM::NMMEvaluation(parent->board, tree->board, player);

		tree->generateChildren(player);
		for (NMM::Node* child : tree->children)
		{
			int eval = -ABnegamax(tree, child, depth - 1, player % 2 + 1, -beta, -alpha);
			alpha = std::max(alpha, eval);

			if (alpha >= beta)
				break;
		}

		for (NMM::Node* child : tree->children)
			delete child;

		tree->children.clear();

		return alpha;
	}

	NMM::BoardState ABnegamaxBestMove(NMM::Node* tree, int player, int depth)
	{
		NMM::BoardState bestState;

		int alpha = INT_MIN;
		int beta = INT_MAX;

		tree->generateChildren(player);
		for (NMM::Node* child : tree->children)
		{
			int eval = ABnegamax(tree, child, depth - 1, player, alpha, beta);

			if (eval > alpha)
			{
				bestState = child->board;
				alpha = eval;
			}

			if (alpha >= beta)
				break;
		}

		for (NMM::Node* child : tree->children)
			delete child;

		tree->children.clear();

		return bestState;
	}
}
