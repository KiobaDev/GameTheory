#pragma once

#include <vector>

#include "..\Minimax\NMMBoard.h"
#include "Enums.h"

namespace NMM 
{
	namespace PNS
	{
		class Node
		{
		public:
			NMM::BoardState board;
			Node* parent;
			std::vector<Node*> children;

			int proof = 0, disproof = 0;
			bool isExpanded = false;
			GameState value = GameState::Unknown;
			NodeType type;

			Node(NMM::BoardState _board, NodeType _type, Node* _parent)
			{
				board = _board;
				type = _type;
				parent = _parent;
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
				for (NMM::BoardState move : board.possibleMoves(board.playerTurn))
				{
					Node* child = new Node(move, (type == NodeType::And ? NodeType::Or : NodeType::And), this);

					children.push_back(child);
				}
			}
		};
	}
}