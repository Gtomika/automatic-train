#include <cstring>
#include <stdexcept>

#include "board.h"

/*
 * board.cpp
 *
 *  Created on: 2021. jan. 24.
 *      Author: Gáspár Tamás
 */

namespace tchess
{
	//Constant for the empty squares.
	extern const unsigned int empty = 0;

	//Constant for pawns (negated for black).
	extern const unsigned int pawn = 1;

	//Constant for knights (negated for black).
	extern const unsigned int knight = 2;

	//Constant for bishops (negated for black).
	extern const unsigned int bishop = 3;

	//Constant for rooks (negated for black).
	extern const unsigned int rook = 4;

	//Constant for kings (negated for black).
	extern const unsigned int king = 5;

	//Constant for queens (negated for black).
	extern const unsigned int queen = 6;

	//Constant for the sentinel squares (outside of the central 8x8).
	extern const unsigned int sentinel = 7;

	extern const int def_squares[144] = {
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

	extern const int mailbox[144] = {
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

	extern const int mailbox64[64] = {
			21, 22, 23, 24, 25, 26, 27, 28,
			31, 32, 33, 34, 35, 36, 37, 38,
			41, 42, 43, 44, 45, 46, 47, 48,
			51, 52, 53, 54, 55, 56, 57, 58,
			61, 62, 63, 64, 65, 66, 67, 68,
			71, 72, 73, 74, 75, 76, 77, 78,
			81, 82, 83, 84, 85, 86, 87, 88,
			91, 92, 93, 94, 95, 96, 97, 98
		};

	extern const bool canSlide[6] = {false, false, true, true, true, false};

	extern const unsigned int offsetAmount[6] = {0, 8, 4, 4, 8, 8};

	extern const int offsets[6][8] = {
			{   0,   0,  0,  0, 0,  0,  0,  0 }, // <-- unused
			{ -21, -19,-12, -8, 8, 12, 19, 21 }, // <-- KNIGHT
			{ -11,  -9,  9, 11, 0,  0,  0,  0 }, // <-- BISHOP
			{ -10,  -1,  1, 10, 0,  0,  0,  0 }, // <-- ROOK
			{ -11, -10, -9, -1, 1,  9, 10, 11 }, // <-- QUEEN
			{ -11, -10, -9, -1, 1,  9, 10, 11 }  // <-- KING
		};

	static const char firstFile = 'a';

	std::string createSquareName(int square) {
		std::string squareName;
		int rank = 8 - square / 8;
		char file = firstFile + square % 8;
		//32 -> rank: 8 - 32/8 = 4, file: 'a' + 32 % 8 = 'a'
		//33 -> rank 8 - 33/8 = 4, file: 'a' + 33 % 8 = 'b'
		squareName += file;
		squareName += std::to_string(rank);
		return squareName;
	}

	unsigned int createSquareNumber(const std::string& squareName) {
		if(squareName.size() != 2) throw std::runtime_error("Invalid name!");
		char file = squareName[0];
		unsigned int rank = squareName[1];
		unsigned int number = (rank * 8) + ((file - 'a')%8);
		if(number < 0 || number > 63) {
			throw std::runtime_error("Invalid name!");
		}
		return number;
	}

	//chessboard class implementations

	chessboard::chessboard() {
		//set board square as default
		std::memcpy(squares, tchess::def_squares, sizeof(squares));
	}

	inline int chessboard::operator [](unsigned int i) {
		return squares[i];
	}

	//move generation implementation

	void move_generator::generatePseudoLegalPawnMoves(unsigned int side, unsigned int square, std::list<move>& moves) const {
		if(side == white) { //pawn moves for white

		} else { //pawn moves for black

		}
	}

	//Credits to chessprogramming.org for this useful code
	void move_generator::generatePseudoLegalNonPawnMoves(unsigned int side, unsigned int square, std::list<move>& moves) const {
		/*
		 * Depending on the side, this may be negative, but now we wonly care about
		 * the type of the piece.
		 */
		const int piece = board[square] < 0 ? -1*board[square] : board[square];
		for(int i=0; i<offsetAmount[piece]; ++i) { //check all the directions (offsets) this piece can move to
			for (int n = square;;) {
				n = mailbox[mailbox64[n] + offsets[piece][i]]; //number of the next square in this direction
			    if (n == -1) break; //square is off the board
			    if (board[n] != 0) { //there is a piece on this square
			    	if ( (side==white && board[n] < 0) || (side == black && board[n] > 0) ) { //this piece is an enemy piece
			    		moves.push_back(move(square, n, capture));
			        }
			        break; //after capture, or if it was a friendly piece we can not move any more in this direction
			    }
			    //there was no piece here, can make a quiet move in this direction
			    moves.push_back(move(square, n, quietMove));
			    if (!canSlide[piece]) break; //for non-sliding pieces, we must stop after 1 move in a direction
			}
		}
	}

	void move_generator::generatePseudoLegalNormalMoves(unsigned int side, std::list<move>& moves) const {
		if(side == white) { //generate moves for white
			for(unsigned int square = 0; square < 64; ++square) { //check all squares
				int piece = board[square];
				if(piece > 0 && piece < 7) { //found a white piece
					if(piece == pawn) { //look for white pawn moves
						generatePseudoLegalPawnMoves(side, square, moves); //find all moves for this white pawn
					} else { //look for white's non pawn moves
						generatePseudoLegalNonPawnMoves(side, square, moves); //find all moves for this white piece
					}
				}
			}
		} else { //generate moves for black
			for(unsigned int square = 0; square < 64; ++square) { //check all squares
				int piece = board[square];
				if(piece < 0) { //found a black piece
					if(piece == pawn) { //look for black pawn moves
						generatePseudoLegalPawnMoves(side, square, moves); //find all moves for black pawn
					} else { //look for black's non pawn moves
						generatePseudoLegalNonPawnMoves(side, square, moves); //find all moves for this black piece
					}
				}
			}
		}
	}

	void move_generator::generatePseudoLegalCastleMoves(unsigned int side, std::list<move>&) const {

	}

	void move_generator::generatePseudoLegalMoves(unsigned int side, std::list<move>& moves) const {
		moves.clear(); //empty list
		generatePseudoLegalNormalMoves(side, moves); //add normal moves
		generatePseudoLegalCastleMoves(side, moves); //add castling moves
	}
}


