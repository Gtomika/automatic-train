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
	/*
	 * Will create a readable representation of a field, such as a4. For example,
	 * from field
	 */
	std::string createSquareName(int square);

	//Constant for the empty squares.
	const unsigned int empty = 0;

	//Constant for pawns (negated for black).
	const unsigned int pawn = 1;

	//Constant for knights (negated for black).
	const unsigned int knight = 2;

	//Constant for bishops (negated for black).
	const unsigned int bishop = 3;

	//Constant for rooks (negated for black).
	const unsigned int rook = 4;

	//Constant for kings (negated for black).
	const unsigned int king = 5;

	//Constant for queens (negated for black).
	const unsigned int queen = 6;

	//Constant for the sentinel squares (outside of the central 8x8).
	const unsigned int sentinel = 7;

	//Default squares of the board, when starting a game (using numbers here instead of the constant names for compactness).
	int def_squares[144] = {
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7, -4, -2, -3, -5, -6, -3, -2, -4,  7,  7,
		7,  7, -1, -1, -1, -1, -1, -1, -1, -1,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  1,  1,  1,  1,  1,  1,  1,  1,  7,  7,
		7,  7,  4,  2,  3,  5,  6,  3,  2,  4,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7
	};

	/*
	 * 12x12 mailbox array, used to check if a move is out of bounds together with
	 * mailbox64 array.
	 */
	int mailbox[144] = {
	     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		 -1, -1,  0,  1,  2,  3,  4,  5,  6,  7, -1, -1,
		 -1, -1,  8,  9, 10, 11, 12, 13, 14, 15, -1, -1,
		 -1, -1, 16, 17, 18, 19, 20, 21, 22, 23, -1, -1,
		 -1, -1, 24, 25, 26, 27, 28, 29, 30, 31, -1, -1,
		 -1, -1, 32, 33, 34, 35, 36, 37, 38, 39, -1, -1,
		 -1, -1, 40, 41, 42, 43, 44, 45, 46, 47, -1, -1,
		 -1, -1, 48, 49, 50, 51, 52, 53, 54, 55, -1, -1,
		 -1, -1, 56, 57, 58, 59, 60, 61, 62, 63, -1, -1,
		 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	/**
	 * 8x8 mailbox array, used to check if a move is out of bounds together with
	 * the 12x12 mailbox array.
	 */
	int mailbox64[64] = {
	    21, 22, 23, 24, 25, 26, 27, 28,
	    31, 32, 33, 34, 35, 36, 37, 38,
	    41, 42, 43, 44, 45, 46, 47, 48,
	    51, 52, 53, 54, 55, 56, 57, 58,
	    61, 62, 63, 64, 65, 66, 67, 68,
	    71, 72, 73, 74, 75, 76, 77, 78,
	    81, 82, 83, 84, 85, 86, 87, 88,
	    91, 92, 93, 94, 95, 96, 97, 98
	};

	/**
	 * This array is used to determing if a piece can slide (using the piece codes). For example the
	 * rook (code 4) can slide, and so the fourth element in this array (index 3) is true.
	 */
	bool canSlide[6] = {false, false, true, true, true, false};

	/**
	 * This array determines to how many directions (not literally in case of knights) a piece can move. For example the
	 * bishop (piece code 3) can move on the 4 diagonals, so the third element of this array is 4.
	 */
	unsigned int offsetAmount = {0, 8, 4, 4, 8, 8};

	/**
	 * This array stores the move offsets for each piece. The offsets of piece p will be in offsets[p] sub array. For
	 * example the knights (code 2) offsets will be in offsets[2].
	 *
	 * In case of pieces where only 4 offset directions are possible the last 4 offsets will be 0 (and are unused).
	 *
	 * The meaning of this is that for example if the rook moves "down" by one square then we can get the destination
	 * by adding 10 to its current square, and so 10 is one offset of the rook.
	 */
	int offsets[6][8] = {
		{   0,   0,  0,  0, 0,  0,  0,  0 }, // <-- unused
		{ -21, -19,-12, -8, 8, 12, 19, 21 }, // <-- KNIGHT
		{ -11,  -9,  9, 11, 0,  0,  0,  0 }, // <-- BISHOP
		{ -10,  -1,  1, 10, 0,  0,  0,  0 }, // <-- ROOK
		{ -11, -10, -9, -1, 1,  9, 10, 11 }, // <-- QUEEN
		{ -11, -10, -9, -1, 1,  9, 10, 11 }  // <-- KING
	};


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
		chessboard() : squares(def_squares) {}
	};
}

#endif /* SRC_BOARD_BOARD_H_ */
