#pragma once

#include <map>
#include <vector>
#include <algorithm>

#define BOARD_SIZE 24

enum class Pawn { none, black, white };

std::map<int, std::vector<int>> possibleMoves =
{{0, {1, 3, 9}},{1, {0, 2, 4}},{2, {1, 5, 14} }, { 3, {0, 4, 10, 6} }, { 4, {1, 3, 5, 7} },
{ 5, {2, 4, 13, 8} }, { 6, {3, 7, 11} }, { 7, {6, 8, 4} }, { 8, {5, 7 ,12 }},{9, {0, 10, 21} },
{ 10, {3, 9, 11, 18}},{11, {6, 10 ,15}},{12, {8, 13, 17}},{13, {5, 12, 14, 20}}, {14, {2, 13, 23}},
{ 15, {11, 16, 18}},{16, {15, 17, 19}},{17, {12, 16, 20}},{18, {10, 15, 19, 21}},{19, {16, 18, 20, 22}},
{ 20, {13, 17, 19, 23}},{21, {9, 18, 22}},{22, {19, 21, 23}},{23, {22, 20, 14}} };

std::vector<std::vector<int>> possibleLines =
{
	{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, //up-horizontal
	{0, 9, 21}, {3, 10, 18}, {6, 11, 15}, //left-vertical
	{2, 14, 23}, {5, 13, 20}, {8, 12, 17}, //right-vertical
	{15, 16, 17}, {18, 19, 20}, {21, 22, 23}, //bottom-horizontal
	{1, 4, 7}, {9, 10, 11}, {12, 13, 14}, {16, 19, 22}, //middle-intersections
	{0, 3, 6}, {2, 5, 8}, {17, 20, 23}, {15, 18, 21} //diagonall-intersections
};

std::vector<int> findPossibleMoves(Pawn board[BOARD_SIZE], int pawn) //TODO: test
{
	std::vector<int> moves = possibleMoves[pawn];
	std::remove_if(moves.begin(), moves.end(), [&](int pawn) { return board[pawn] != Pawn::none; });
}

bool isPawnInLine(Pawn board[BOARD_SIZE], int pawn) //TODO: test2
{
	for (int i = 0; i < possibleLines.size(); ++i)
		if (std::find(possibleLines[i].begin(), possibleLines[i].end(), pawn) != possibleLines[i].end())
		{
			bool isLineFormed = true;
			for (int j = 0; j < possibleLines[i].size(); ++j)
				if (board[possibleLines[i][j]] != board[pawn])
					isLineFormed = false;

			if (isLineFormed)
				return true;
		}

	return false;
}

void calculateGameComplexityM1()
{

}