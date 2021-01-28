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
		chessboard board;
		board.makeMove(move(createSquareNumber("e2"), createSquareNumber("e4"), quietMove), white);
		std::cout << board.to_string();
		//bool d5attacked = isAttacked(board, white, createSquareNumber("d5"));
		//std::cout << "d5 attacked: " << d5attacked << std::endl;
		//bool e5attacked = isAttacked(board, white, createSquareNumber("e5"));
		//std::cout << "e5 attacked: " << e5attacked << std::endl;

		board.makeMove(move(createSquareNumber("f7"), createSquareNumber("f6"), quietMove), black);
		std::cout << board.to_string();
		board.makeMove(move(createSquareNumber("b1"), createSquareNumber("c3"), quietMove), white);
		std::cout << board.to_string();

		//bool e5attackedBlack = isAttacked(board, black, createSquareNumber("e5"));
		//std::cout << "e5 attacked by black: " << e5attackedBlack << std::endl;
		bool b5attackedWhite = isAttacked(board, white, createSquareNumber("b5"));
		std::cout << "b5 attacked by white: " << b5attackedWhite << std::endl;
	}

}

int main() {
	tchess::test();
	std::cout << "TChess program" << std::endl;
	std::cout << "Currently only console mode is supported!" << std::endl;
	//tchess::selectPlayersAndStart();
	return 0;
}



