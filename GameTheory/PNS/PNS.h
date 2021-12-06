#pragma once

#include <cstdlib>
#include <algorithm>

#include "node.h"
#include "Evaluation.h"
#include "Enums.h"
#include "Config.h"

namespace NMM
{
	namespace PNS
	{
		void setProofAndDisproofNumbers(PNS::Node* node)
		{
			if (node->isExpanded)
			{
				if (node->type == NodeType::And)
				{
					node->proof = 0;
					node->disproof = INT_MAX;

					for (PNS::Node* child : node->children)
					{
						if (child->proof == INT_MAX)
							node->proof = INT_MAX;
						else if (node->proof != INT_MAX)
							node->proof += child->proof;
						node->disproof = std::min(node->disproof, child->disproof);
					}
				}
				else
				{
					node->proof = INT_MAX;
					node->disproof = 0;

					for (PNS::Node* child : node->children)
					{
						if (child->disproof == INT_MAX)
							node->disproof = INT_MAX;
						else if (node->disproof != INT_MAX)
							node->disproof += child->disproof;
						node->proof = std::min(node->proof, child->proof);
					}
				}
			}
			else
			{
				switch (node->value)
				{
				case GameState::Unknown:
					node->proof = 1;
					node->disproof = 1;
					break;
				case GameState::Win:
					node->proof = 0;
					node->disproof = INT_MAX;
					break;
				case GameState::Lose:
					node->proof = INT_MAX;
					node->disproof = 0;
					break;
				}
			}
		}

		Node* selectMostProvingNode(Node* node)
		{
			while (node->isExpanded)
			{
				int value = INT_MAX;
				Node* best = nullptr;

				if (node->type == NodeType::And)
				{
					for (Node* child : node->children)
						if (value > child->disproof)
						{
							best = child;
							value = child->disproof;
						}
				}
				else
				{
					for (Node* child : node->children)
						if (value > child->proof)
						{
							best = child;
							value = child->proof;
						}
				}

				node = best;
			}

			return node;
		}

		Node* selectBestMove(Node* root)
		{
			if (root->children.size() == 0)
				return nullptr;

			float value = 0;
			Node* best = nullptr;

			for (Node* child : root->children)
			{
				float childValue = (float)child->disproof / (float)child->proof;
				if (childValue > value)
				{
					value = childValue;
					best = child;
				}
			}

			return best;
		}

		void expandNode(Node* node)
		{
			node->generateChildren();

			for (Node* child : node->children)
			{
				evaluate(child);
				setProofAndDisproofNumbers(child);

				if (node->type == NodeType::And)
				{
					if (child->disproof == 0)
						break;
				}
				else
				{
					if (child->proof == 0)
						break;
				}
			}

			node->isExpanded = true;
		}

		Node* updateAncestors(Node* node, Node* root)
		{
			while (node != root)
			{
				int oldProof = node->proof;
				int oldDisproof = node->disproof;

				setProofAndDisproofNumbers(node);
				if (node->proof == oldProof && node->disproof == oldDisproof)
					return node;

				node = node->parent;
			}

			setProofAndDisproofNumbers(root);
			return root;
		}

		Node* PNS(Node* root)
		{
			evaluate(root);
			setProofAndDisproofNumbers(root);

			Node* current = root;
			while (root->proof != 0 && root->disproof != 0 && resourcesAvailable())
			{
				Node* mostProving = selectMostProvingNode(current);
				expandNode(mostProving);
				current = updateAncestors(mostProving, root);
			}

			Node* temp = selectBestMove(root);
			return temp;
		}
	}
}