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

#include <string>
#include <bitset>
#include <stdexcept>

namespace tchess
{
	//Move code for a "non-special" move that results in no capture.
	extern const unsigned int quietMove;

	//Move code for double pawn pushes.
	extern const unsigned int doublePawnPush;

	//Move code for kingside castle.
	extern const unsigned int kingsideCastle;

	//Move code for queenside castle.
	extern const unsigned int queensideCastle;

	//Move code for captures.
	extern const unsigned int capture;

	//Move code for en-passant captures.
	extern const unsigned int enPassantCapture;

	//Move code for a pawn push that results in a knight promotion.
	extern const unsigned int knightPromotion;

	//Move code for a pawn push that results in a bishop promotion.
	extern const unsigned int bishopPromotion;

	//Move code for a pawn push that results in a rook promotion.
	extern const unsigned int rookPromotion;

	//Move code for a pawn push that results in a queen promotion.
	extern const unsigned int queenPromotion;

	//Move code for a pawn push that captures and results in a knight promotion.
	extern const unsigned int knightPromotionCap;

	//Move code for a pawn push that captures and results in a bishop promotion.
	extern const unsigned int bishopPromotionCap;

	//Move code for a pawn push that captures and results in a rook promotion.
	extern const unsigned int rookPromotionCap;

	//Move code for a pawn push that captures and results in a queen promotion.
	extern const unsigned int queenPromotionCap;

	/*
	 * This exception is thrown when a move string could not
	 * be parsed into a move.
	 */
	class move_parse_exception: public std::exception {
		std::string message;
	public:
		move_parse_exception(char const* const message) throw() : message(message) {}
		virtual char const* what() const throw();
	};

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
	 * For castling the departure and destination squares of the move will be the departure and destination
	 * of the king.
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
		 * found in board.h.
		 *
		 * This only returns the type of piece (only positive). For black, this should be negated.
		 */
		unsigned int promotedTo() const;

		inline unsigned int getFromSquare() const { return fromSquare; }

		inline unsigned int getToSquare() const { return toSquare; }

		//Creates a string format of the move.
		std::string to_string() const;
	};

	/*
	 * Parses a move string into a move object. This method performs some
	 * checks in the input, but it does not guarantee that the move is legal.
	 * Basically this method will accept the move if it can parse it, but it
	 * is up to the game object to check if the player can actually make this move.
	 *
	 * The parsed move may also be "incomplete", because the parser cannot know
	 * whether the move is a capture or not.
	 */
	move parse_move(const std::string& moveString, unsigned int side);
}

#endif /* SRC_BOARD_MOVE_H_ */
