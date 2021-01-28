#include <cstring>

#include "game.h"

/*
 * game.cpp
 *
 *  Created on: 2021. jan. 24.
 *      Author: Gáspár Tamás
 */

//Macro that negates a piece code if we are on black side
#define OWN_SIDE(side, piece) ((side)==0 ? (piece) : -(piece))

//Macro that gives the value of the piece on the enemy side
#define ENEMY_SIDE(side, piece) ((side)==0 ? -(piece) : (piece))

namespace tchess
{
	//Identifier for the white player.
	extern const unsigned int white = 0;

	//Identifier for the black player.
	extern const unsigned int black = 1;

	//Constant that indicated no en passant capture is possible.
	extern const int noEnPassant = - 10;

	//chessboard class implementations

	chessboard::chessboard() {
		//set board square as default
		std::memcpy(squares, tchess::def_squares, sizeof(squares));
	}

	int chessboard::makeMove(const move& m, unsigned int side) {
		const unsigned int from = m.getFromSquare(), to = m.getToSquare();
		int captured = empty;
		if(m.isKingsideCastle()) { //kingside castle moves 2 pieces
			//the move's from and to squares are the kings position before and after the castle
			squares[from] = empty; //move king
			squares[to] = OWN_SIDE(side, king);
			squares[to + 1] = empty; //move rook
			squares[from + 1] = OWN_SIDE(side, rook);
		} else if(m.isQueensideCastle()) { //queenside castle moves 2 pieces
			//the moves from and to squares are the kings position before and after the castle
			squares[from] = empty; //move king
			squares[to] = OWN_SIDE(side, king);
			squares[to - 2] = empty; //move rook
			squares[from - 1] = OWN_SIDE(side, rook);
		} else if(m.isEnPassant()) { //en passant is special, because the captures piece is not on the destination square
			squares[from] = empty; //move pawn
			squares[to] = OWN_SIDE(side, pawn);
			int capturedAt = side == white ? to+8 : to-8;
			squares[capturedAt] = empty; //remove pawn
			captured = ENEMY_SIDE(side, pawn); //save that the captured piece was an enemy pawn
		} else if(m.isPromotion()) { //in case of promotion we need to look at what the pawn was promoted to
			int promotedTo = OWN_SIDE(side, m.promotedTo());
			captured = squares[to]; //if there was a capture, this will save it, if not nothing changes
			squares[from] = empty;
			squares[to] = promotedTo; //make the promoted piece
		} else { //nothing special about this move
			captured = squares[to]; //if there was a capture, this will save it, if not nothing changes
			int pieceThatMoved = squares[from];
			squares[from] = empty;
			squares[to] = pieceThatMoved;
		}
		return captured;
	}

	void chessboard::unmakeMove(const move& m, unsigned int side, int capturedPiece) {
		const unsigned int from = m.getFromSquare(), to = m.getToSquare();
		if(m.isKingsideCastle()) { //kingside castle moved 2 pieces that need to be unmaked
			//the move's from and to squares are the kings position before and after the castle
			squares[to] = empty; //unmake king move
			squares[from] = OWN_SIDE(side, king);
			squares[to + 1] = OWN_SIDE(side, rook); //unmake rook move
			squares[from + 1] = empty;
		} else if(m.isQueensideCastle()) { //queenside castle moves 2 pieces that need to unmaked
			//the moves from and to squares are the kings position before and after the castle
			squares[from] = OWN_SIDE(side, king); //unmove king
			squares[to] = empty;
			squares[to - 2] = OWN_SIDE(side, rook); //unmove rook
			squares[from - 1] = empty;
		} else if(m.isEnPassant()) { //en passant is special, because the captures piece is not on the destination square
			squares[from] = OWN_SIDE(side, pawn); //unmove own pawn
			squares[to] = empty;
			int capturedAt = side == white ? to+8 : to-8;
			squares[capturedAt] = ENEMY_SIDE(side, pawn); //replace enemy pawn
		} else if(m.isPromotion()) { //in case of promotion a pawn must be placed to the departure square instead of the promotion piece
			squares[from] = OWN_SIDE(side, pawn); //replace pawn
			squares[to] = capturedPiece; // replace the captured piece (sets to empty in case of no capture)
		} else  { //normal move
			int pieceThatMoved = squares[to]; //we can find the piece that moved on the destination square
			squares[from] = pieceThatMoved; //place to its original square
			squares[to] = capturedPiece; // replace the captured piece (sets to empty in case of no capture)
		}
	}

	std::string chessboard::to_string() const {
		std::string bS = "---------------------------\n";
		for(int i=0; i<8; ++i) { //add each rank
			bS += "|";
			for(int j=0; j<8; ++j) {
				std::string pieceString = " ";
				int piece = squares[8*i + j]; //can also be empty
				int pieceType = piece >= 0 ? piece : -piece;
				char pieceChar;
				switch(pieceType) { //wont allow me to use predefined piece code variables in switch
				case 0: //empty
					pieceChar = '0';
					break;
				case 1: //pawn
					pieceChar = 'P';
					break;
				case 2: //knight
					pieceChar = 'N';
					break;
				case 3: //bishop
					pieceChar = 'B';
					break;
				case 4: //rook
					pieceChar = 'R';
					break;
				case 5: //king
					pieceChar = 'K';
					break;
				case 6: //queen
					pieceChar = 'Q';
					break;
				default:
					throw std::runtime_error("Corrupted board!");
				}
				pieceString += piece >= 0 ? " " : "-"; //add - for black pieces
				pieceString += pieceChar;
				bS += pieceString; //add piece string
			}
			bS += " |\n";
		}
		bS.append("---------------------------\n");
		return bS;
	}

	//implement game_informatin class

	game_information::game_information() {
		kingsideCastleRight[white] = true;
		kingsideCastleRight[black] = true;
		queensideCastleRight[white] = true;
		queensideCastleRight[black] = true;
		enPassantCaptureSquares[white] = noEnPassant;
		enPassantCaptureSquares[black] = noEnPassant;
		sideToMove = white; //white begins
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
}




