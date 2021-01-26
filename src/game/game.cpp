#include "game.h"

/*
 * game.cpp
 *
 *  Created on: 2021. jan. 24.
 *      Author: Gáspár Tamás
 */

namespace tchess
{
	//Identifier for the white player.
	extern const unsigned int white = 0;

	//Identifier for the black player.
	extern const unsigned int black = 1;

	//Constant that indicated no en passant capture is possible.
	extern const int noEnPassant = - 10;

	//implement game_informatin class

	game_information::game_information() {
		sideToMove = white; //white begins
		kingsideCastleRight = {true, true}; //both player are eligible to castle at start
		queensideCastleRight = {true, true};
		enPassantCaptureSquares = {noEnPassant, noEnPassant}; //no en passant is possible at start
	}

	void game_information::disableKingsideCastleRight(unsigned int side) {
		kingsideCastleRight[side] = false;
	}

	void game_information::disableQueensideCastleRight(unsigned int side) {
		queensideCastleRight[side] = false;
	}

	void game_information::setEnPassantSquare(unsigned int side, unsigned int square) {
		enPassantCaptureSquares[side] = square;
	}

	void game_information::setSideToMove(unsigned int side) {
		sideToMove = side;
	}
}




