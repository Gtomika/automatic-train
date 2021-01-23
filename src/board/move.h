#include <string>
#include <bitset>

/*
 * move.h
 *
 * This header contains the move class that describes chess moves.
 *
 *  Created on: 2021. jan. 23.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_BOARD_MOVE_H_
#define SRC_BOARD_MOVE_H_

namespace tchess {

//Move code for a "non-special" move that results in no capture.
const unsigned int quietMove = 0;

//Move code for double pawn pushes.
const unsigned int doublePawnPush = 1;

//Move code for kingside castle.
const unsigned int kingsideCastle = 2;

//Move code for queenside castle.
const unsigned int queensideCastle = 3;

//Move code for captures.
const unsigned int capture = 4;

//Move code for en-passant captures.
const unsigned int enPassantCapture = 5;

//Move code for a pawn push that results in a knight promotion.
const unsigned int knightPromotion = 8;

//Move code for a pawn push that results in a bishop promotion.
const unsigned int bishopPromotion = 9;

//Move code for a pawn push that results in a rook promotion.
const unsigned int rookPromotion = 10;

//Move code for a pawn push that results in a queen promotion.
const unsigned int queenPromotion = 11;

//Move code for a pawn push that captures and results in a knight promotion.
const unsigned int knightPromotionCap = 12;

//Move code for a pawn push that captures and results in a bishop promotion.
const unsigned int bishopPromotionCap = 13;

//Move code for a pawn push that captures and results in a rook promotion.
const unsigned int rookPromotionCap = 14;

//Move code for a pawn push that captures and results in a queen promotion.
const unsigned int queenPromotionCap = 15;


/**
 * Represents a chess move, using the departure square (from) and the destination
 * square (to).
 *
 * Also includes 4 flags in a bitfield. These flags are:
 *  - Bit 1: capture flag
 *  - Bit 2: promotion flag
 *  - Bit 3: Special flag 1
 *  - Bit 4: Special flag 2
 *
 * The possible bitfields are declared as constants in this file. For example, bishop promotion capture
 * is 13 (binary 1101), so both capture and promotion flags are 1, but only the second special flag is 1.
 *
 * The special flags are used to for example distinguish between a promotion to a rook or a queen.
 */
class move {

	//Departure square of the move.
	unsigned int fromSquare;

	//Destination square of the move.
	unsigned int toSquare;

	//The 4 bit wide bitfield containing the flags.
	std::bitset<4> flags;

public:
	//Create a move object
	move(unsigned int fromSquare, unsigned int toSquare, unsigned int flags) :
		fromSquare(fromSquare), toSquare(toSquare), flags(flags) {}

	//Checks if this move is a capture.
	inline bool isCapture() const;

	//Checks if this move is a promotion.
	inline bool isPromotion() const;

	//Checks if this move is a double pawn push.
	inline bool isDoublePawnPush() const;

	//Checks if this move is a kingside castle.
	inline bool isKingsideCastle() const;

	//Checks if this move is a queen castle.
	inline bool isQueensideCastle() const;

	//Checks if this move is an en-passant capture.
	inline bool isEnPassant() const;

	/*
	 * Returns the piece code of the piece the paawn was promoted into.
	 * This should only be called if the move is a promotion. Piece codes are
	 * found in board.h
	 */
	unsigned int promotedTo() const;

	inline unsigned int getFromSquare() const { return fromSquare; }

	inline unsigned int getToSquare() const { return toSquare; }

	//Creates a string format of the move.
	std::string to_string() const;
};

}

#endif /* SRC_BOARD_MOVE_H_ */
