#include <iostream>
#include <string>

#include "board/board.h"
#include "human/human_player.h"

/*
 * main.cpp
 *
 * Entry point for TChess.
 *
 *  Created on: 2021. jan. 23.
 *      Author: Gáspár Tamás
 */

#include "game/game.h"
#include "board/evaluation.h"

namespace tchess {

	const int test_squares[64] = {
		 0,  0,  -5,  0,  0,  0,  0,  0,
		 0,  0,  0,  -4,  0,  0,  0,  0,
		 0,  6,  0,  -1,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  1,  0,
		 0,  0,  2,  0,  1,  0,  0,  0,
		 0,  0,  0,  0,  1,  0,  0,  0,
		 0,  0,  0,  3,  0,  0,  0,  0,
		 0,  0,  5,  0,  0,  0,  0,  0
	};

	/*
	 * Can test all kind of functions here.
	 */
	void test() {
		chessboard board(test_squares);
		std::cout << board.to_string() << std::endl;

		std::cout << kingSafetyEvaluation(white, createSquareNumber("c1"), board) << std::endl;
		std::cout << kingSafetyEvaluation(black, createSquareNumber("c8"), board) << std::endl;
	}
}

int main() {
	//tchess::test();
	std::cout << "TChess program" << std::endl;
	std::cout << "Currently only console mode is supported!" << std::endl;
	tchess::selectPlayersAndStart();
	return 0;
}



