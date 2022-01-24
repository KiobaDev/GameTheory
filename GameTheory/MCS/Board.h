#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "../Minimax/NMMBoard.h"

namespace NMM
{
	namespace MCS
	{
		std::map<int, std::vector<int>> possibleBoardMoves =
		{
			{0, {1, 9}}, {1, {0, 2, 4}}, {2, {1, 14}}, {3, {4, 10}}, {4, {1, 3, 5, 7}}, {5, {4, 13}},
			{6, {7, 11}}, {7, {4, 6, 8}}, {8, {7 ,12}}, {9, {0, 10, 21}}, {10, {3, 9, 11, 18}}, {11, {6, 10, 15}},
			{12, {8, 13, 17}}, {13, {5, 12, 14, 20}}, {14, {2, 13, 23}}, {15, {11, 16}}, {16, {15, 17, 19}}, {17, {12, 16}},
			{18, {10, 19}}, {19, {16, 18, 20, 22}}, {20, {13, 19}}, {21, {9, 22}}, {22, {19, 21, 23}}, {23, {14, 22}}
		};	//Moves saved in format {3, {2, 1, 5, 4}} contain moves 3->2, 3->1, 3->5, 3->4

		std::map<int, std::vector<int>> possibleBoardLines =
		{
			{0, {1, 2, 9, 21}}, {1, {0, 2, 4, 7}}, {2, {0, 1, 14, 23}}, {3, {4, 5, 10, 18}}, {4, {3, 5, 1, 7}}, {5, {3, 4, 13, 20}},
			{6, {7, 8, 11, 15}}, {7, {6, 8, 1, 4}}, {8, {6, 7, 12, 17}}, {9, {0, 21, 10, 11}}, {10, {3, 18, 9, 11}}, {11, {6, 15, 9, 10}},
			{12, {8, 17, 13, 14}}, {13, {5, 20, 12, 14}}, {14, {2, 23, 12, 13}}, {15, {6, 11, 16, 17}}, {16, {15, 17, 19, 22}}, {17, {8, 12, 15, 16}},
			{18, {3, 10, 19, 20}}, {19, {18, 20, 16, 22}}, {20, {5, 13, 18, 19}}, {21, {0, 9, 22, 23}}, {22, {21, 23, 16, 19}}, {23, {2, 14, 21, 22}}
		};	//Lines saved in format {3, {2, 1, 5, 4}} contain lines 2-3-1, 5-3-4

		class Board
		{
		public:
			int* board;
			int ply;
			int playerTurn;

			Board()
			{
				board = new int[24]{};
				ply = 0;
				playerTurn = 1;
			}

			Board(const Board& other)
				: Board()
			{
				std::copy(other.board, other.board + 24, board);
				ply = other.ply;
				playerTurn = other.playerTurn;
			}

			Board(const NMM::BoardState& other)
				: Board()
			{
				std::copy(other.board, other.board + 24, board);
				ply = other.ply;
				playerTurn = other.playerTurn;
			}

			~Board()
			{
				delete[] board;
				board = nullptr;
			}

			Board randomMove()
			{
				int playerPawnCount = std::count(board, board + 24, playerTurn);

				if (ply < 18)
					return randomMovePhaseOne();
				else if (playerPawnCount <= 3)
					return randomMovePhaseThree();
				else
					return randomMovePhaseTwo();
			}

			Board randomMovePhaseOne()
			{
				int insertIndex = getRandomPawn(0);

				Board newMCSBoard = *this;
				newMCSBoard.insertPawn(insertIndex, playerTurn);
				++newMCSBoard.ply;
				newMCSBoard.playerTurn = getOpponent();

				if (newMCSBoard.isPawnInLine(insertIndex))
					newMCSBoard.removeRandomPawn(getOpponent());

				return newMCSBoard;
			}

			Board randomMovePhaseTwo()
			{
				std::vector<int> notBlockedPawns = getNotBlockedPawns(playerTurn);
				int randPawn = notBlockedPawns[rand() % notBlockedPawns.size()];

				const std::vector<int>* moves = &possibleBoardMoves[randPawn];

				int emptyCells = 0;
				for (int i = 0; i < moves->size(); ++i)
					if (board[(*moves)[i]] == 0)
						emptyCells += 1;
				int randomCell = rand() % emptyCells;

				int cellIndex = 0;
				for (int i = 0; i < moves->size(); ++i)
				{
					if (randomCell != 0 && board[(*moves)[i]] == 0)
						randomCell -= 1;
					else if (randomCell == 0 && board[(*moves)[i]] == 0)
					{
						cellIndex = (*moves)[i];
						break;
					}
				}

				Board newMCSBoard = *this;
				newMCSBoard.movePawnFromTo(randPawn, cellIndex);
				++newMCSBoard.ply;
				newMCSBoard.playerTurn = getOpponent();

				if (newMCSBoard.isPawnInLine(cellIndex))
					newMCSBoard.removeRandomPawn(getOpponent());

				return newMCSBoard;
			}

			Board randomMovePhaseThree()
			{
				int insertIndex = getRandomPawn(playerTurn);
				int emptyCell = getRandomPawn(0);

				Board newMCSBoard = *this;
				newMCSBoard.movePawnFromTo(insertIndex, emptyCell);
				++newMCSBoard.ply;
				newMCSBoard.playerTurn = getOpponent();

				if (newMCSBoard.isPawnInLine(emptyCell))
					newMCSBoard.removeRandomPawn(getOpponent());

				return newMCSBoard;
			}

			std::vector<Board> possibleMoves()
			{
				int playerPawnCount = std::count(board, board + 24, playerTurn);

				if (ply < 18)
					return possibleMovesPhaseOne();
				else if (playerPawnCount <= 3)
					return possibleMovesPhaseThree();
				else
					return possibleMovesPhaseTwo();
			}

			std::vector<Board> possibleMovesPhaseOne()
			{
				std::vector<Board> possibleMoves;

				for (int i = 0; i < 24; ++i)
				{
					if (board[i] == 0)
					{
						Board newBoard = *this;
						newBoard.insertPawn(i, playerTurn);
						++newBoard.ply;
						newBoard.playerTurn = getOpponent();

						std::vector<Board> BoardsAfterRemoves = possibleRemoves(newBoard, i);

						possibleMoves.insert(possibleMoves.end(), BoardsAfterRemoves.begin(), BoardsAfterRemoves.end());
					}
				}

				return possibleMoves;
			}

			std::vector<Board> possibleMovesPhaseTwo()
			{
				std::vector<Board> possibleMoves;

				for (int i = 0; i < 24; ++i)
				{
					if (board[i] == playerTurn)
					{
						for (int k = 0; k < possibleBoardMoves[i].size(); ++k)
						{
							if (board[possibleBoardMoves[i][k]] == 0)
							{
								Board newBoard = *this;
								newBoard.movePawnFromTo(i, possibleBoardMoves[i][k]);
								++newBoard.ply;
								newBoard.playerTurn = getOpponent();

								std::vector<Board> BoardsAfterRemoves = possibleRemoves(newBoard, possibleBoardMoves[i][k]);

								possibleMoves.insert(possibleMoves.end(), BoardsAfterRemoves.begin(), BoardsAfterRemoves.end());
							}
						}
					}
				}

				return possibleMoves;
			}

			std::vector<Board> possibleMovesPhaseThree()
			{
				std::vector<Board> possibleMoves;

				for (int i = 0; i < 24; ++i)
				{
					if (board[i] == playerTurn)
					{
						for (int k = 0; k < 24; ++k)
						{
							if (board[k] == 0)
							{
								Board newBoard = *this;
								newBoard.movePawnFromTo(i, k);
								++newBoard.ply;
								newBoard.playerTurn = getOpponent();

								std::vector<Board> BoardsAfterRemoves = possibleRemoves(newBoard, k);

								possibleMoves.insert(possibleMoves.end(), BoardsAfterRemoves.begin(), BoardsAfterRemoves.end());
							}
						}
					}
				}

				return possibleMoves;
			}

			std::vector<Board> possibleRemoves(Board newBoard, int point)
			{
				std::vector<Board> boardeStatesForRemove;

				if (newBoard.isPawnInLine(point))
				{
					for (int j = 0; j < 24; ++j)
					{
						if (newBoard.board[j] == getOpponent())
						{
							if (newBoard.isPawnInLine(j) && (std::count(newBoard.board, newBoard.board + 24, getOpponent()) > 3))
								continue;

							Board newBoardForRemove = newBoard;
							newBoardForRemove.removePawn(j);

							boardeStatesForRemove.push_back(newBoardForRemove);
						}
					}
				}
				else
					boardeStatesForRemove.push_back(newBoard);

				return boardeStatesForRemove;
			}

			std::vector<int> getNotBlockedPawns(int player)
			{
				std::vector<int> pawns;

				for (int i = 0; i < 24; ++i)
				{
					if (board[i] != player)
						continue;

					const std::vector<int>* moves = &possibleBoardMoves[i];
					for (int j = 0; j < moves->size(); ++j)
						if (board[(*moves)[j]] == 0)
						{
							pawns.push_back(i);
							break;
						}
				}

				return pawns;
			}

			void removeRandomPawn(int player)
			{
				int removeIndex = getRandomPawn(player);

				removePawn(removeIndex);
			}

			int getRandomPawn(int player)
			{
				int emptyCellsCount = std::count(board, board + 24, player);
				int randIndex = rand() % emptyCellsCount;

				int pawnIndex = 0;
				for (int i = 0; i < 24; ++i)
				{
					if (randIndex != 0 && board[i] == player)
						randIndex -= 1;
					else if (randIndex == 0 && board[i] == player)
					{
						pawnIndex = i;
						break;
					}
				}

				return pawnIndex;
			}

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

			bool isPawnInLine(int point) const
			{
				int pawn = board[point];
				std::vector<int>* ptrToPossibleLines = &possibleBoardLines[point];

				bool isPointInLine = false;
				for (int i = 0; i < ptrToPossibleLines->size(); i += 2)
					if (board[(*ptrToPossibleLines)[i]] == pawn && board[(*ptrToPossibleLines)[i + 1]] == pawn)
						isPointInLine = true;

				return isPointInLine;
			}

			int getOpponent() const
			{
				return playerTurn == 1 ? 2 : 1;
			}

			Board& operator=(const Board& other)
			{
				std::copy(other.board, other.board + 24, board);
				ply = other.ply;
				playerTurn = other.playerTurn;

				return *this;
			}
		};
	}
}
