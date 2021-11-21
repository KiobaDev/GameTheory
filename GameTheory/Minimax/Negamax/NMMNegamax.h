#pragma once

#include <algorithm>

#include "..\NMMTree.h"
#include "..\NMMEvaluation.h"

namespace NMM
{
	int negamax(NMM::Node* parent, NMM::Node* tree, int depth, int player)
	{
		if (depth == 1 || isPlayerWinning(tree->board, player))
			return NMM::NMMEvaluation(parent->board, tree->board, player);

		int maxEval = INT_MIN;

		tree->generateChildren(player);
		for (NMM::Node* child : tree->children)
		{
			int eval = -negamax(tree, child, depth - 1, player % 2 + 1);
			maxEval = std::max(maxEval, eval);

			delete child;
		}

		tree->children.clear();

		return maxEval;
	}

	NMM::BoardState negamaxBestMove(NMM::Node* tree, int player, int depth)
	{
		int maxEval = INT_MIN;
		NMM::BoardState bestState;

		tree->generateChildren(player);
		for (NMM::Node* child : tree->children)
		{
			int eval = negamax(tree, child, depth - 1, player);

			if (eval > maxEval)
			{
				bestState = child->board;
				maxEval = eval;
			}

			delete child;
		}

		tree->children.clear();

		return bestState;
	}
}
