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
			-4, -2, -3,  0, -5, -3, -2, -4,
			-1, -1, -1, -1,  0, -1, -1, -1,
			 0,  0,  0,  0,  0,  0,  0,  0,
			 0,  0,  0,  0,  -1,  0,  0,  0,
			 0,  0,  3,  0,  1,  0,  0,  0,
			 0,  0,  0,  1,  0,  2,  0,  0,
			 1,  1,  1,  1,  0,  1,  -6,  1,
			 4,  2,  3,  6,  5,  0,  0,  4
	};

	/*
	 * Can test all kind of functions here.
	 */
	void test() {
		chessboard board(test_squares);
		game_information info;
		std::cout << board.to_string() << std::endl;

		std::cout << evaluateBoard(white, board, info) << std::endl;

		move m = move(createSquareNumber("g2"), createSquareNumber("h1"), capture);
		int capped = board.makeMove(m, black);
		board.unmakeMove(m, black, capped);

		std::cout << evaluateBoard(white, board, info) << std::endl;
		std::cout << board.to_string() << std::endl;
	}
}

int main() {
	//tchess::test();
	std::cout << "TChess program" << std::endl;
	std::cout << "Currently only console mode is supported!" << std::endl;
	bool startGame = true;
	while(startGame) {
		startGame = tchess::selectPlayersAndStart();
	}
	return 0;
}



