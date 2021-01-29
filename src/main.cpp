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

namespace tchess {

	/**
	 * Allows the player to select who will play White and Black sides.
	 */
	char selectPlayerForSide(const std::string& sideName) {
		std::cout << "---------------------------" << std::endl;
		std::cout << "Select who will play " << sideName << ":" << std::endl;
		std::cout << " - Type p for player!" << std::endl;
		std::cout << " - Type e for engine!" << std::endl;
		std::string selection;
		std::getline(std::cin, selection);
		if(selection == "p") {
			std::cout << sideName << " will be controlled by a player." << std::endl;
		} else if(selection == "e") {
			std::cout << sideName << " will be controlled by the TChess engine." << std::endl;
		} else {
			std::cout << "Invalid selection, please try again!" << std::endl;
			selectPlayerForSide(sideName);
		}
		return selection[0];
	}

	/*
	 * Allows the user to select who will play on both sides. For example, player vs engine or
	 * player vs player.
	 */
	void selectPlayersAndStart() {
		char whiteSelect = selectPlayerForSide("White");
		char blackSelect = selectPlayerForSide("Black");
		if(whiteSelect == 'p' && blackSelect == 'p') { //both human controlled
			human_player_console whitePlayer(white), blackPlayer(black);
			game<human_player_console, human_player_console> gameController(whitePlayer, blackPlayer);
			gameController.beginGame();
		} //TODO other options
	}

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



