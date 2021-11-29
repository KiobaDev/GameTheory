#pragma once

#include <algorithm>

#include "..\NMMTree.h"
#include "..\NMMEvaluation.h"

namespace NMM
{
	int SortABnegamax(NMM::Node* parent, NMM::Node* tree, int depth, int player, int alpha, int beta)
	{
		if (depth == 1 || isPlayerWinning(tree->board, player))
			return NMM::NMMEvaluation(parent->board, tree->board, player);

		tree->generateChildren(player);
		std::sort(tree->children.begin(), tree->children.end(), [&](NMM::Node* first, NMM::Node* second) { return NMM::NMMEvaluation(parent->board, first->board, player) >= NMM::NMMEvaluation(parent->board, second->board, player); });
		for (NMM::Node* child : tree->children)
		{
			int eval = -SortABnegamax(tree, child, depth - 1, player % 2 + 1, -beta, -alpha);
			alpha = std::max(alpha, eval);

			if (alpha >= beta)
				break;
		}

		for (NMM::Node* child : tree->children)
			delete child;

		tree->children.clear();

		return alpha;
	}

	NMM::BoardState SortABnegamaxBestMove(NMM::Node* tree, int player, int depth)
	{
		NMM::BoardState bestState;

		int alpha = INT_MIN;
		int beta = INT_MAX;

		tree->generateChildren(player);
		std::sort(tree->children.begin(), tree->children.end(), [&](NMM::Node* first, NMM::Node* second) { return NMM::NMMEvaluation(tree->board, first->board, player) >= NMM::NMMEvaluation(tree->board, second->board, player); });
		for (NMM::Node* child : tree->children)
		{
			int eval = SortABnegamax(tree, child, depth - 1, player, -beta, -alpha);

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
