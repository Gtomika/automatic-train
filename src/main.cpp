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

#include "engine/polyglot.h"

namespace tchess {

	const int test_squares[64] = {
			-4, -2, -3, -6, -5, -3, -2, -4,
			-1, -1, -1, -1, -1, -1, -1, -1,
			 0,  0,  0,  0,  0,  0,  0,  0,
			 0,  0,  0,  0,  0,  0,  0,  0,
			 0,  0,  0,  0,  0,  0,  0,  0,
			 0,  0,  0,  0,  0,  0,  0,  0,
			 1,  1,  1,  1,  1,  1,  1,  1,
			 4,  2,  3,  6,  5,  3,  2,  4
	};

	/*
	 * Can test all kind of functions here.
	 */
	void test() {
		chessboard board(test_squares);
		game_information info;
		move opening(createSquareNumber("e2"), createSquareNumber("e4"), doublePawnPush);
		board.makeMove(opening, white);
		updateGameInformation(board, opening, info);
		std::cout << std::hex << polyKeyFromBoard(board, info) << std::dec << std::endl;
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



