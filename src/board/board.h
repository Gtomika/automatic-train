/*
 * board.h
 *
 * Chessboard representation.
 *  - Square centric.
 *  - Mailbox: white pices are positive, black pieces are negative, empty square is 0, the sentinel
 *  files and ranks are 7.
 *
 * The board is a 8x8 array looks like this:
 *  -4 -2 -3 -5 -6 -3 -2 -4  <-- black's backrank
 *  -1 -1 -1 -1 -1 -1 -1 -1  <-- black's pawn rank
 *   0  0  0  0  0  0  0  0
 *   0  0  0  0  0  0  0  0
 *   0  0  0  0  0  0  0  0
 *   0  0  0  0  0  0  0  0
 *   1  1  1  1  1  1  1  1  <-- white's pawn rank
 *   4  2  3  5  6  3  2  4  <-- white's backrank
 *
 *  Created on: 2021. jan. 23.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_BOARD_BOARD_H_
#define SRC_BOARD_BOARD_H_

#include <string>
#include <list>

#include "move.h"
#include "game/game.h"

namespace tchess
{
	//Constant for the empty squares.
	extern const unsigned int empty;

	//Constant for pawns (negated for black).
	extern const unsigned int pawn;

	//Constant for knights (negated for black).
	extern const unsigned int knight;

	//Constant for bishops (negated for black).
	extern const unsigned int bishop;

	//Constant for rooks (negated for black).
	extern const unsigned int rook;

	//Constant for kings (negated for black).
	extern const unsigned int king;

	//Constant for queens (negated for black).
	extern const unsigned int queen;

	//Default squares of the board, when starting a game (using numbers here instead of the constant names for compactness).
	extern const int def_squares[144];

	/*
	 * 12x12 mailbox array, used to check if a move is out of bounds together with
	 * mailbox64 array.
	 */
	extern const int mailbox[144];

	/**
	 * 8x8 mailbox array, used to check if a move is out of bounds together with
	 * the 12x12 mailbox array.
	 */
	extern const int mailbox64[64];

	/**
	 * This array is used to determing if a piece can slide (using the piece codes). For example the
	 * rook (code 4) can slide, and so the fourth element in this array (index 3) is true.
	 */
	extern const bool canSlide[6];

	/**
	 * This array determines to how many directions (not literally in case of knights) a piece can move. For example the
	 * bishop (piece code 3) can move on the 4 diagonals, so the third element of this array is 4.
	 */
	extern const unsigned int offsetAmount[6];

	/**
	 * This array stores the move offsets for each piece. The offsets of piece p will be in offsets[p] sub array. For
	 * example the knights (code 2) offsets will be in offsets[2].
	 *
	 * In case of pieces where only 4 offset directions are possible the last 4 offsets will be 0 (and are unused).
	 *
	 * The meaning of this is that for example if the rook moves "down" by one square then we can get the destination
	 * by adding 10 to its current square, and so 10 is one offset of the rook.
	 */
	extern const int offsets[6][8];


	class game_information;
	class chessboard;

	/*
	 * This class creates moves from a chessboard and the side to move.
	 */
	class move_generator {

		/*
		 * Stores the current chessboard.
		 */
		const chessboard& board;

		/**
		 * Additional information about the game, that is needed to
		 * generate moves.
		 */
		const game_information& gameInfo;

	public:

		move_generator(const chessboard& b, const game_information& gi) : board(b), gameInfo(gi) {}

		/*
		 * Generates the pseudo legal moves for a chessboard and the side to move. The list
		 * received as parameter will be cleared before, then filled with the moves. This method first
		 * generates moves using one piece, then castling moves.
		 *
		 * Pseudo legal moves are where it isn't checked if they leave their king in check.
		 */
		void generatePseudoLegalMoves(unsigned int side, std::list<move>&) const;

	private:

		/*
		 * This method will generate all pseudo legal moves for 1 pawn, on the given square.
		 * It is checked by the called of this method that there is a pawn on this square.
		 */
		void generatePseudoLegalPawnMoves(unsigned int side, unsigned int square, std::list<move>&) const;

		/**
		 * Generates all pseudo legal moves for a non pawn piece. IT is checked by the called of
		 * this method that there is a non pawn piece on this square
		 */
		void generatePseudoLegalNonPawnMoves(unsigned int side, unsigned int square, std::list<move>&) const;

		/**
		 * Generates all pawn moves, including en passant captures and promotions.
		 */
		void generatePseudoLegalNormalMoves(unsigned int side, std::list<move>&) const;

		/**
		 * Generates kingside and queenside castling moves. Pseudo legal means here that it it
		 * not checked whether the king is in check when castling or if it passes through attacked fields.
		 */
		void generatePseudoLegalCastleMoves(unsigned int side, std::list<move>&) const;
	};

	/*
	 * Will create a readable representation of a field, such as b5. For example,
	 * from field 32 it will create a4.
	 */
	std::string createSquareName(int square);

	/*
	 * Inverse of the method above, it will determine the mailbox position of
	 * a square from its name. For example for "h1" it will return 63.
	 */
	unsigned int createSquareNumber(const std::string& squareName);
}

#endif /* SRC_BOARD_BOARD_H_ */
