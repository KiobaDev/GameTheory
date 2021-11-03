#pragma once

#include <algorithm>

#include "NMMTree.h"
#include "NMMEvaluation.h"

namespace NMM
{
	int negamax(NMM::Node* parent, NMM::Node* tree, int depth, int player, bool maximalizedPlayer)
	{
		if (depth == 1 || isPlayerWinning(tree->board, player))
			return NMM::NMMEvaluation(parent->board, tree->board, player);

		int maxEval = INT_MIN;

		for (NMM::Node* child : tree->children)
		{
			int eval = -negamax(tree, child, depth - 1, player, !maximalizedPlayer);
			maxEval = std::max(maxEval, eval);
		}

		return maxEval;
	}

	NMM::BoardState negamaxBestMove(NMM::Node* tree, int player, int depth)
	{
		int maxEval = INT_MIN;
		NMM::BoardState bestState;

		for (NMM::Node* child : tree->children)
		{
			int eval = negamax(tree, child, depth - 1, player, false);

			if (eval > maxEval)
			{
				bestState = child->board;
				maxEval = eval;
			}
		}

		return bestState;
	}
}
