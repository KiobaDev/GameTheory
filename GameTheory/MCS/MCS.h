#pragma once

#include <vector>

#include "Node.h"
#include "Evaluation.h"
#include "Board.h"

namespace NMM
{
	namespace MCS
	{
		Board monteCarloEvaluation(Node* node, int numOfSimulations)
		{
			Node* bestChild = nullptr;
			float bestProbability = -1;

			for (Node* child : node->children)
			{
				int r = 0;
				for (int i = 0; i < numOfSimulations; ++i)
				{
					Board currentState = node->board;
					while (!isPlayerLosing(currentState, 1) && !isPlayerLosing(currentState, 2))
						currentState = currentState.randomMove();

					if (isPlayerLosing(currentState, 2))
						r += 1;
				}

				float probability = (float)r / (float)numOfSimulations;
				if (probability > bestProbability)
				{
					bestProbability = probability;
					bestChild = child;
				}
			}

			return bestChild->board;
		}
	}
}