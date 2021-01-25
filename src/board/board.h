#include <string>

/*
 * board.h
 *
 * Chessboard representation.
 *  - Square centric.
 *  - Mailbox: white pices are positive, black pieces are negative, empty square is 0, the sentinel
 *  files and ranks are 7.
 *
 * The board is a 12x12 array (144) looks like this:
 *   7  7  7  7  7  7  7  7  7  7  7  7
 *   7  7  7  7  7  7  7  7  7  7  7  7
 *   7  7 -4 -2 -3 -5 -6 -3 -2 -4  7  7  <-- black's backrank
 *   7  7 -1 -1 -1 -1 -1 -1 -1 -1  7  7  <-- black's pawn rank
 *   7  7  0  0  0  0  0  0  0  0  7  7
 *   7  7  0  0  0  0  0  0  0  0  7  7
 *   7  7  0  0  0  0  0  0  0  0  7  7
 *   7  7  0  0  0  0  0  0  0  0  7  7
 *   7  7  1  1  1  1  1  1  1  1  7  7  <-- white's pawn rank
 *   7  7  4  2  3  5  6  3  2  4  7  7  <-- white's backrank
 *   7  7  7  7  7  7  7  7  7  7  7  7
 *   7  7  7  7  7  7  7  7  7  7  7  7
 *
 *
 *  Created on: 2021. jan. 23.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_BOARD_BOARD_H_
#define SRC_BOARD_BOARD_H_

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

	//Constant for the sentinel squares (outside of the central 8x8).
	extern const unsigned int sentinel;

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


	/*
	 * The chessboard class which stores the current board, and allows for making and
	 * unmaking moves. This class won't check for the validity of the moves, instead it assumes the
	 * moves received are valid.
	 */
	class chessboard {

		//Squares of the board (using numbers here instead of the constant names for compactness)..
		int squares[144];

	public:
		//Creates a chessboard as it is at the start of the game.
		chessboard();
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
