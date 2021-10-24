#pragma once

#include <vector>
#include <map>
#include <algorithm>

namespace NMM
{
	std::map<int, std::vector<int>> possibleBoardMoves =
	{
		{0, {1, 3, 9}}, {1, {0, 2, 4}}, {2, {1, 5, 14}}, {3, {0, 4, 6, 10}}, {4, {1, 3, 5, 7}}, {5, {2, 4, 8, 13}},
		{6, {3, 7, 11}}, {7, {4, 6, 8}}, {8, {5, 7 ,12}}, {9, {0, 10, 21}}, {10, {3, 9, 11, 18}}, {11, {6, 10, 15}},
		{12, {8, 13, 17}}, {13, {5, 12, 14, 20}}, {14, {2, 13, 23}}, {15, {11, 16, 18}}, {16, {15, 17, 19}}, {17, {12, 16, 20}},
		{18, {10, 15, 19, 21}}, {19, {16, 18, 20, 22}}, {20, {13, 17, 19, 23}}, {21, {9, 18, 22}}, {22, {19, 21, 23}}, {23, {14, 20, 22}}
	};	//Moves saved in format {3, {2, 1, 5, 4}} contain moves 3->2, 3->1, 3->5, 3->4

	std::map<int, std::vector<int>> possibleBoardLines =
	{
		{0, {1, 2, 9, 21, 3, 6}}, {1, {0, 2, 4, 7}}, {2, {0, 1, 14, 23, 5, 8}}, {3, {4, 5, 10, 18, 0, 6}}, {4, {3, 5, 1, 7}}, {5, {3, 4, 13, 20, 2, 8}},
		{6, {7, 8, 11, 15, 0, 3}}, {7, {6, 8, 1, 4}}, {8, {6, 7, 12, 17, 2, 5}}, {9, {0, 21, 10, 11}}, {10, {3, 18, 9, 11}}, {11, {6, 15, 9, 10}},
		{12, {8, 17, 13, 14}}, {13, {5, 20, 12, 14}}, {14, {2, 23, 12, 13}}, {15, {6, 11, 16, 17, 18, 21}}, {16, {15, 17, 19, 22}}, {17, {8, 12, 15, 16, 20, 23}},
		{18, {3, 10, 19, 20, 15, 21}}, {19, {18, 20, 16, 22}}, {20, {5, 13, 18, 19, 17, 23}}, {21, {0, 9, 22, 23, 15, 18}}, {22, {21, 23, 16, 19}}, {23, {2, 14, 21, 22, 17, 20}}
	};	//Lines saved in format {3, {2, 1, 5, 4}} contain lines 2-3-1, 5-3-4

	class BoardState
	{
	public:
		int* board;
		int ply;

		BoardState()
		{
			board = new int[24]{};
			ply = 0;
		}

		BoardState(const BoardState& other)
			: BoardState()
		{
			std::copy(other.board, other.board + 24, board);
			ply = other.ply;
		}

#pragma region possibleMoves

		std::vector<BoardState> possibleMoves(int player)
		{
			int playerPawnCount = std::count(board, board + 24, player);

			if (ply <= 18)
				return possibleMovesPhaseOne(player);
			else if (playerPawnCount <= 3)
				return possibleMovesPhaseThree(player);
			else
				return possibleMovesPhaseTwo(player);
		}

		std::vector<BoardState> possibleMovesPhaseOne(int player)
		{
			std::vector<BoardState> possibleMoves;

			for (int i = 0; i < 24; ++i) 
			{
				if (board[i] == 0)
				{
					BoardState newBoardState = *this;
					newBoardState.insertPawn(i, player);
					++newBoardState.ply;

					if (newBoardState.isPawnInLine(i))
					{
						for(int j = 0; j < 24; ++j)
						{
							if (newBoardState.board[j] == getOpponent(player)) 
							{
								BoardState newBoardStateForRemove = newBoardState;
								newBoardStateForRemove.removePawn(j);

								possibleMoves.push_back(newBoardStateForRemove);
							}
						}
					}
					else
						possibleMoves.push_back(newBoardState);
				}
			}

			return possibleMoves;
		}

		std::vector<BoardState> possibleMovesPhaseTwo(int player)
		{
			std::vector<BoardState> possibleMoves;

			for (int i = 0; i < 24; ++i)
			{
				if (board[i] == player)
				{
					for (int k = 0; k < possibleBoardMoves[i].size(); ++k)
					{
						if (board[possibleBoardMoves[i][k]] == 0)
						{
							BoardState newBoardState = *this;
							newBoardState.movePawnFromTo(i, k);
							++newBoardState.ply;

							if (newBoardState.isPawnInLine(k))
							{
								for (int j = 0; j < 24; ++j)
								{
									if (newBoardState.board[j] == getOpponent(player))
									{
										BoardState newBoardStateForRemove = newBoardState;
										newBoardStateForRemove.removePawn(j);

										possibleMoves.push_back(newBoardStateForRemove);
									}
								}
							}
							else
								possibleMoves.push_back(newBoardState);
						}
					}
				}
			}

			return possibleMoves;
		}

		std::vector<BoardState> possibleMovesPhaseThree(int player)
		{
			std::vector<BoardState> possibleMoves;

			for (int i = 0; i < 24; ++i)
			{
				if (board[i] == player)
				{
					for (int k = 0; k < 24; ++k)
					{
						if (board[k] == 0)
						{
							BoardState newBoardState = *this;
							newBoardState.movePawnFromTo(i, k);
							++newBoardState.ply;

							if (newBoardState.isPawnInLine(k))
							{
								for (int j = 0; j < 24; ++j)
								{
									if (newBoardState.board[j] == getOpponent(player))
									{
										BoardState newBoardStateForRemove = newBoardState;
										newBoardStateForRemove.removePawn(j);

										possibleMoves.push_back(newBoardStateForRemove);
									}
								}
							}
							else
								possibleMoves.push_back(newBoardState);
						}
					}
				}
			}

			return possibleMoves;
		}

#pragma endregion

		void removePawn(int point)
		{
			board[point] = 0;
		}

		void insertPawn(int point, int player)
		{
			board[point] = player;
		}

	    void movePawnFromTo(int fromPoint, int toPoint)
		{
			board[toPoint] = board[fromPoint];
			board[fromPoint] = 0;
		}

		bool isPawnInLine(int point)
		{
			int pawn = board[point];
			std::vector<int>* ptrToPossibleLines = &possibleBoardLines[point];

			bool isPointInLine = false;
			for (int i = 0; i < ptrToPossibleLines->size(); i += 2)
				if (board[(*ptrToPossibleLines)[i]] == pawn && board[(*ptrToPossibleLines)[i + 1]] == pawn)
					isPointInLine = true;

			return isPointInLine;
		}

		int getOpponent(int player)
		{
			return (player % 2) + 1;
		}

		BoardState& operator=(const BoardState& other)
		{
			std::copy(other.board, other.board + 24, board);
			ply = other.ply;

			return *this;
		}
	};
}
