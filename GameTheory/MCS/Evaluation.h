#pragma once

#include <algorithm>

#include "Board.h"

namespace NMM
{
	namespace MCS
	{
		bool isPlayerLosing(Board board, int player)
		{
			return (std::count(board.board, board.board + 24, player) < 3 || board.getNotBlockedPawns(player).size() == 0) && board.ply > 18;
		}
	}
}