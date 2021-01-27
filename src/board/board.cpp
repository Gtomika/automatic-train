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

	//This is how the board looks at the start of a game
	extern const int def_squares[144] = {
		-4, -2, -3, -5, -6, -3, -2, -4,
		-1, -1, -1, -1, -1, -1, -1, -1,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 1,  1,  1,  1,  1,  1,  1,  1,
		 4,  2,  3,  5,  6,  3,  2,  4
	};

	//Mailbox array with sentinel squares (-1).
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

	//64 sized mailbox array.
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

	//determines if pieces can slide
	extern const bool canSlide[6] = {false, false, true, true, true, false};

	//determines the amount of directions pieces can move to
	extern const unsigned int offsetAmount[6] = {0, 8, 4, 4, 8, 8};

	//the exact offsets to get the directions of each piece
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
		squareName += file;
		squareName += std::to_string(rank);
		return squareName;
	}

	unsigned int createSquareNumber(const std::string& squareName) {
		if(squareName.size() != 2) throw std::runtime_error("Invalid name!");
		char file = squareName[0];
		unsigned int rank = squareName[1] - '0';
		unsigned int number = ((8-rank) * 8) + (file - 'a');
		if(number < 0 || number > 63) {
			throw std::runtime_error("Invalid name!");
		}
		return number;
	}

	//move generation implementation

	void move_generator::generatePseudoLegalPawnMoves(unsigned int side, unsigned int square, std::list<move>& moves) const {
		int inFront, captureLeft, captureRight;
		bool hasCaptureLeftSquare = false, hasCaptureRightSquare = false;
		if(side == white) { //pawn moves for white
			inFront = board[square-8]; //this square is "in front" of the black pawn
			if(square % 8 != 0) { //the pawn is not on the leftmost file, and can possibly capture left
				hasCaptureLeftSquare = true;
				captureLeft = board[square-9]; //find what is on the left capture square
			}
			if(square % 8 != 7) { //the pawn is not on the rightmost file and can possibly capture right
				hasCaptureRightSquare = true;
				captureRight = board[square-7]; //find what is on the right capture square
			}
			if(square / 8 == 6) { //the pawn is on its original rank, and may be able to make a double move
				int inFrontFar = board[square-16]; //this square is in front of the square in front of the white pawn
				//double pawn push is only possible of 2 squares in front of the pawn are empty
				if(inFront == empty && inFrontFar == empty) {
					moves.push_back(move(square, square-16, doublePawnPush)); //register possible double pawn push
					//adding the en passant capture square is not done here
				}
				if(inFront == empty) { //the pawn can still choose to only move one square
					moves.push_back(move(square, square-8, quietMove));
				}
				//despite the double push, the pawn can still choose to capture
				if(hasCaptureLeftSquare && captureLeft < 0) { //enemy piece on the left capture square
					moves.push_back(move(square, square-9, capture));
				}
				if(hasCaptureRightSquare && captureRight < 0) { //enemy piece on the right capture square
					moves.push_back(move(square, square-7, capture));
				}
			} else if(square / 8 == 1) { //the pawn is ready to promote if it can move/capture forward
				if(inFront == empty) { //the pawn can quietly move into the promotion rank if there is nothing in front
					moves.push_back(move(square, square-8, knightPromotion));
					moves.push_back(move(square, square-8, bishopPromotion));
					moves.push_back(move(square, square-8, rookPromotion));
					moves.push_back(move(square, square-8, queenPromotion));
				}
				if(hasCaptureLeftSquare && captureLeft < 0) { //enemy piece on the left (promotion) capture square
					moves.push_back(move(square, square-9, knightPromotionCap));
					moves.push_back(move(square, square-9, bishopPromotionCap));
					moves.push_back(move(square, square-9, rookPromotionCap));
					moves.push_back(move(square, square-9, queenPromotionCap));
				}
				if(hasCaptureRightSquare && captureRight < 0) { //enemy piece on the right (promotion) capture square
					moves.push_back(move(square, square-7, knightPromotionCap));
					moves.push_back(move(square, square-7, bishopPromotionCap));
					moves.push_back(move(square, square-7, rookPromotionCap));
					moves.push_back(move(square, square-7, queenPromotionCap));
				}
			} else { //the pawn can neither double push nor promote
				if(inFront == empty) { //the pawn can advance if the way is empty
					moves.push_back(move(square, square-8, quietMove));
				}
				//the pawn can also capture
				if(hasCaptureLeftSquare && captureLeft < 0) { //enemy piece on the left capture square
					moves.push_back(move(square, square-9, capture));
				} else if(hasCaptureLeftSquare && (square-9) == gameInfo.getEnPassantSquare(white)) { //en passant capture possible to the left
					moves.push_back(move(square, square-9, enPassantCapture));
				}
				if(hasCaptureRightSquare && captureRight < 0) { //enemy piece on the right capture square
					moves.push_back(move(square, square-7, capture));
				} else if(hasCaptureRightSquare && (square-7) == gameInfo.getEnPassantSquare(white)) { //en passant capture possible to the right
					moves.push_back(move(square, square-7, enPassantCapture));
				}
			}
		} else { //pawn moves for black
			inFront = board[square+8]; //this square is "in front" of the white pawn
			if(square % 8 != 0) { //the pawn is not on the leftmost file, and can possibly capture left
				hasCaptureLeftSquare = true;
				captureLeft = board[square+7]; //find what is on the left capture square
			}
			if(square % 8 != 7) { //the pawn is not on the rightmost file and can possibly capture right
				hasCaptureRightSquare = true;
				captureRight = board[square+9]; //find what is on the right capture square
			}
			if(square / 8 == 1) { //the pawn is on its original rank, and may be able to make a double move
				int inFrontFar = board[square+16]; //this square is in front of the square in front of the black pawn
				//double pawn push is only possible of 2 squares in front of the pawn are empty
				if(inFront == empty && inFrontFar == empty) {
					moves.push_back(move(square, square+16, doublePawnPush)); //register possible double pawn push
					//adding the en passant capture square is not done here
				}
				if(inFront == empty) { //the pawn can still choose to only move one square
					moves.push_back(move(square, square+8, quietMove));
				}
				//despite the double push, the pawn can still choose to capture
				if(hasCaptureLeftSquare && captureLeft > 0) { //enemy piece on the left capture square
					moves.push_back(move(square, square+7, capture));
				}
				if(hasCaptureRightSquare && captureRight > 0) { //enemy piece on the right capture square
					moves.push_back(move(square, square+9, capture));
				}
			} else if(square / 8 == 6) { //the pawn is ready to promote if it can move/capture forward
				if(inFront == empty) { //the pawn can quietly move into the promotion rank if there is nothing in front
					moves.push_back(move(square, square+8, knightPromotion));
					moves.push_back(move(square, square+8, bishopPromotion));
					moves.push_back(move(square, square+8, rookPromotion));
					moves.push_back(move(square, square+8, queenPromotion));
				}
				if(hasCaptureLeftSquare && captureLeft > 0) { //enemy piece on the left (promotion) capture square
					moves.push_back(move(square, square+7, knightPromotionCap));
					moves.push_back(move(square, square+7, bishopPromotionCap));
					moves.push_back(move(square, square+7, rookPromotionCap));
					moves.push_back(move(square, square+7, queenPromotionCap));
				}
				if(hasCaptureRightSquare && captureRight > 0) { //enemy piece on the right (promotion) capture square
					moves.push_back(move(square, square+9, knightPromotionCap));
					moves.push_back(move(square, square+9, bishopPromotionCap));
					moves.push_back(move(square, square+9, rookPromotionCap));
					moves.push_back(move(square, square+9, queenPromotionCap));
				}
			} else { //the pawn can neither double push nor promote
				if(inFront == empty) { //the pawn can advance if the way is empty
					moves.push_back(move(square, square+8, quietMove));
				}
				//the pawn can also capture
				if(hasCaptureLeftSquare && captureLeft > 0) { //enemy piece on the left capture square
					moves.push_back(move(square, square+7, capture));
				}  else if(hasCaptureLeftSquare && (square+7) == gameInfo.getEnPassantSquare(black)) { //en passant capture possible to the left
					moves.push_back(move(square, square+7, enPassantCapture));
				}
				if(hasCaptureRightSquare && captureRight > 0) { //enemy piece on the right capture square
					moves.push_back(move(square, square+9, capture));
				} else if(hasCaptureRightSquare && (square+9) == gameInfo.getEnPassantSquare(black)) { //en passant capture possible to the right
					moves.push_back(move(square, square+9, enPassantCapture));
				}
			}
		}
	}

	//Credits to chessprogramming.org for this useful code
	void move_generator::generatePseudoLegalNonPawnMoves(unsigned int side, unsigned int square, std::list<move>& moves) const {
		/*
		 * Depending on the side, this may be negative, but now we wonly care about
		 * the type of the piece.
		 */
		const int piece = board[square] < 0 ? -1*board[square] : board[square];
		for(unsigned int i=0; i<offsetAmount[piece]; ++i) { //check all the directions (offsets) this piece can move to
			for (int n = square;;) {
				n = mailbox[mailbox64[n] + offsets[piece][i]]; //number of the next square in this direction
			    if (n == -1) break; //square is off the board
			    if (board[n] != empty) { //there is a piece on this square
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

	void move_generator::generatePseudoLegalCastleMoves(unsigned int side, std::list<move>& moves) const {
		if(side == white) { //make castling moves for white
			if(gameInfo.getKingsideCastleRights(white)) { //look for kingside castle, if white still has the right
				/*
				 * Since this is pseudo legal generation, only need to check if the space between king and rook
				 * is empty. Can assume that the rook and the king are on their original square, otherwise the
				 * right would be lost.
				 */
				if(board[61] == empty && board[62] == empty) moves.push_back(move(60,62,kingsideCastle));
			}
			if(gameInfo.getQueensideCastleRights(white)) { //look for kingside castle, if white still has the right
				if(board[57] == empty && board[58] == empty && board[59] == empty) moves.push_back(move(60,58,queensideCastle));
			}
		} else { //make castling moves for black
			if(gameInfo.getKingsideCastleRights(black)) { //look for kingside castle, if black still has the right
				/*
				 * Since this is pseudo legal generation, only need to check if the space between king and rook
				 * is empty. Can assume that the rook and the king are on their original square, otherwise the
				 * right would be lost.
				 */
				if(board[5] == empty && board[6] == empty) moves.push_back(move(4,6,kingsideCastle));
			}
			if(gameInfo.getQueensideCastleRights(black)) { //look for kingside castle, if black still has the right
				if(board[1] == empty && board[2] == empty && board[3] == empty) moves.push_back(move(4,2,queensideCastle));
			}
		}
	}

	void move_generator::generatePseudoLegalMoves(unsigned int side, std::list<move>& moves) const {
		moves.clear(); //empty list
		generatePseudoLegalNormalMoves(side, moves); //add normal moves
		generatePseudoLegalCastleMoves(side, moves); //add castling moves
	}
}


