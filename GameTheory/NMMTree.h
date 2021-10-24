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

		void generateChildren(int player)
		{
			for (NMM::BoardState& possibleMove : board.possibleMoves(player))
				children.push_back(new Node(possibleMove));
		}
	};
}