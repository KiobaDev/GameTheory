#pragma once

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Config.h"

bool isBoardCorrect(Pawn board[18])
{
    int whitePawns = std::count(board, board + BOARD_SIZE, Pawn::white);
    int blackPawns = std::count(board, board + BOARD_SIZE, Pawn::black);

    return !(whitePawns > 9 || blackPawns > 9 ||
        (std::min(whitePawns, blackPawns) == 0 && std::max(whitePawns, blackPawns) > 1) ||
        (std::min(whitePawns, blackPawns) == 1 && std::max(whitePawns, blackPawns) > 3) ||
        (std::min(whitePawns, blackPawns) == 2 && std::max(whitePawns, blackPawns) > 5));
}

void calculateGameComplexityM2()
{
    srand(time(0));
    double simulationResults[NUM_OF_SIMULATIONS] = {};

    for (int i = 0; i < NUM_OF_SIMULATIONS; ++i)
    {
        int correctBoardCount = 0;
        for (int j = 0; j < GAME_COUNT; ++j)
        {
            Pawn board[BOARD_SIZE] = {};
            for (Pawn& pawn : board)
                pawn = static_cast<Pawn>(rand() % 3);

            if (isBoardCorrect(board))
                ++correctBoardCount;
        }

        simulationResults[i] = double(correctBoardCount) / GAME_COUNT * NUM_OF_STATES;
    }

    double results = 0;
    for (int i = 0; i < NUM_OF_SIMULATIONS; ++i)
        results += simulationResults[i];

    std::cout << "Usrednione wyniki otrzymane z wszystkich symulacji - zlozonosc gry wynosi: " << results / NUM_OF_SIMULATIONS << std::endl;
}
