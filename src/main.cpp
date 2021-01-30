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

namespace tchess {

	/*
	 * Can test all kind of functions here.
	 */
	void test() {
		std::bitset<4> set(1);
		std::cout << set << std::endl;
		std::cout << set[3] << std::endl;

	}
}

int main() {
	//tchess::test();
	std::cout << "TChess program" << std::endl;
	std::cout << "Currently only console mode is supported!" << std::endl;
	tchess::selectPlayersAndStart();
	return 0;
}



