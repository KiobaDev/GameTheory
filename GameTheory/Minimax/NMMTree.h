#pragma once

#include <vector>

#include "NMMBoard.h"

namespace NMM
{
	class Node
	{
	public:
		NMM::BoardState board;
		std::vector<Node*> children;

		Node()
		{

		}

		Node(const NMM::BoardState& other)
		{
			board = other;
		}

		~Node()
		{
			for (Node* child : children)
			{
				delete child;
				child = nullptr;
			}

			children.clear();
		}

		void generateChildren(int player)
		{
			for (NMM::BoardState& possibleMove : board.possibleMoves(player))
				children.push_back(new Node(possibleMove));
		}

		void generateChildrenToNthDepth(int player, int maxDepth, int depth = 1)
		{
			generateChildren(player);

			if (depth < maxDepth)
				for (Node* child : children)
					child->generateChildrenToNthDepth(board.getOpponent(player), maxDepth, depth + 1);
		}
	};
}
