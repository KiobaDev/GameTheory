#pragma once

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define GAME_COUNT 1000
#define NUM_OF_SIMULATIONS 100
#define BOARD_SIZE 24
#define NUM_OF_STATES 24 * 24 * 24

enum class Pawn { none, black, white };

bool isBoardCorrect(Pawn board[18])
{
    int whitePawns = std::count(board, board + BOARD_SIZE, Pawn::white);
    int blackPawns = std::count(board, board + BOARD_SIZE, Pawn::black);

    return !(whitePawns < 2 || whitePawns > 8 || blackPawns < 2 || blackPawns > 8 || (whitePawns == 2 && blackPawns == 2));
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