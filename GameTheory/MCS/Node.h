#pragma once

#include <vector>
#include <algorithm>

#include "Board.h"

namespace NMM
{
	namespace MCS
	{
		class Node
		{
		public:
			Board board;
			std::vector<Node*> children;

			bool isFinalState = false;

			Node(Board _board)
			{
				board = _board;
			}

			~Node()
			{
				for (size_t i = 0; i < children.size(); ++i)
					if (children[i])
						delete children[i];

				children.clear();
			}

			void generateChildren()
			{
				for (Board move : board.possibleMoves())
				{
					Node* child = new Node(move);

					children.push_back(child);
				}
			}
		};
	}
}