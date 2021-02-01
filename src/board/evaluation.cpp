/*
 * evaluation.cpp
 *
 *	Many of the evaluation values/tables are from the 'simplified fevaluation function'.
 *
 *  Created on: 2021. febr. 1.
 *      Author: Gáspár Tamás
 */
#include <algorithm>

#include "evaluation.h"
#include "board/board.h"

namespace tchess
{
	const int pieceValues[7] = {0, 100, 320, 330, 500, 20000, 900};

	//piece square tables (for both sides)
	extern const int pawnTable[2][64] = {
	   {
			0,  0,  0,  0,  0,  0,  0,  0,
			50, 50, 50, 50, 50, 50, 50, 50,
			10, 10, 20, 30, 30, 20, 10, 10,
			5,  5, 10, 25, 25, 10,  5,  5,
			0,  0,  0, 20, 20,  0,  0,  0,
			5, -5,-10,  0,  0,-10, -5,  5,
			5, 10, 10,-20,-20, 10, 10,  5,
			0,  0,  0,  0,  0,  0,  0,  0
	   },
	   {
		    0,  0,  0,  0,  0,  0,  0,  0,
		    5, 10, 10,-20,-20, 10, 10,  5,
		    5, -5,-10,  0,  0,-10, -5,  5,
		    0,  0,  0, 20, 20,  0,  0,  0,
		    5,  5, 10, 25, 25, 10,  5,  5,
			10, 10, 20, 30, 30, 20, 10, 10,
			50, 50, 50, 50, 50, 50, 50, 50,
			0,  0,  0,  0,  0,  0,  0,  0
	   }
	};

	extern const int knightTable[2][64] = {
	   {
		   -50,-40,-30,-30,-30,-30,-40,-50,
		   -40,-20,  0,  0,  0,  0,-20,-40,
		   -30,  0, 10, 15, 15, 10,  0,-30,
		   -30,  5, 15, 20, 20, 15,  5,-30,
		   -30,  0, 15, 20, 20, 15,  0,-30,
		   -30,  5, 10, 15, 15, 10,  5,-30,
		   -40,-20,  0,  5,  5,  0,-20,-40,
		   -50,-40,-30,-30,-30,-30,-40,-50
	   },
	   {
		   -50,-40,-30,-30,-30,-30,-40,-50,
		   -40,-20,  0,  5,  5,  0,-20,-40,
		   -30,  5, 10, 15, 15, 10,  5,-30,
		   -30,  0, 15, 20, 20, 15,  0,-30,
		   -30,  5, 15, 20, 20, 15,  5,-30,
		   -30,  0, 10, 15, 15, 10,  0,-30,
		   -40,-20,  0,  0,  0,  0,-20,-40,
		   -50,-40,-30,-30,-30,-30,-40,-50
	   }
	};

	extern const int bishopTable[2][64] = {
	   {
		   -20,-10,-10,-10,-10,-10,-10,-20,
		   -10,  0,  0,  0,  0,  0,  0,-10,
		   -10,  0,  5, 10, 10,  5,  0,-10,
		   -10,  5,  5, 10, 10,  5,  5,-10,
		   -10,  0, 10, 10, 10, 10,  0,-10,
		   -10, 10, 10, 10, 10, 10, 10,-10,
		   -10,  5,  0,  0,  0,  0,  5,-10,
		   -20,-10,-10,-10,-10,-10,-10,-20
	   },
	   {
		   -20,-10,-10,-10,-10,-10,-10,-20,
		   -10,  5,  0,  0,  0,  0,  5,-10,
		   -10, 10, 10, 10, 10, 10, 10,-10,
		   -10,  0, 10, 10, 10, 10,  0,-10,
		   -10,  5,  5, 10, 10,  5,  5,-10,
		   -10,  0,  5, 10, 10,  5,  0,-10,
		   -10,  0,  0,  0,  0,  0,  0,-10,
		   -20,-10,-10,-10,-10,-10,-10,-20
	   }
	};

	extern const int rookTable[2][64] = {
	   {
		   0,  0,  0,  0,  0,  0,  0,  0,
			5, 10, 10, 10, 10, 10, 10,  5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
			0,  0,  0,  5,  5,  0,  0,  0
	   },
	   {
		   0,  0,  0,  5,  5,  0,  0,  0,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   -5,  0,  0,  0,  0,  0,  0, -5,
		   5, 10, 10, 10, 10, 10, 10,  5,
		   0,  0,  0,  0,  0,  0,  0,  0
	   }
	};

	extern const int kingTable[2][64] = {
	   {
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -20,-30,-30,-40,-40,-30,-30,-20,
		   -10,-20,-20,-20,-20,-20,-20,-10,
			20, 20,  0,  0,  0,  0, 20, 20,
			20, 30, 10,  0,  0, 10, 30, 20
	   },
	   {
		   20, 30, 10,  0,  0, 10, 30, 20,
		   20, 20,  0,  0,  0,  0, 20, 20,
		   -10,-20,-20,-20,-20,-20,-20,-10,
		   -20,-30,-30,-40,-40,-30,-30,-20,
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -30,-40,-40,-50,-50,-40,-40,-30,
		   -30,-40,-40,-50,-50,-40,-40,-30
	   }
	};

	extern const int kingEndgameTable[2][64] = {
	   {
		   -50,-40,-30,-20,-20,-30,-40,-50,
		   -30,-20,-10,  0,  0,-10,-20,-30,
		   -30,-10, 20, 30, 30, 20,-10,-30,
		   -30,-10, 30, 40, 40, 30,-10,-30,
		   -30,-10, 30, 40, 40, 30,-10,-30,
		   -30,-10, 20, 30, 30, 20,-10,-30,
		   -30,-30,  0,  0,  0,  0,-30,-30,
		   -50,-30,-30,-30,-30,-30,-30,-50
	   },
	   {
		   -50,-30,-30,-30,-30,-30,-30,-50,
		   -30,-30,  0,  0,  0,  0,-30,-30,
		   -30,-10, 20, 30, 30, 20,-10,-30,
		   -30,-10, 30, 40, 40, 30,-10,-30,
		   -30,-10, 30, 40, 40, 30,-10,-30,
		   -30,-10, 20, 30, 30, 20,-10,-30,
		   -30,-20,-10,  0,  0,-10,-20,-30,
		   -50,-40,-30,-20,-20,-30,-40,-50
	   }
	};

	extern const int queenTable[2][64] = {
	   {
		   -20,-10,-10, -5, -5,-10,-10,-20,
		   -10,  0,  0,  0,  0,  0,  0,-10,
		   -10,  0,  5,  5,  5,  5,  0,-10,
			-5,  0,  5,  5,  5,  5,  0, -5,
			 0,  0,  5,  5,  5,  5,  0, -5,
		   -10,  5,  5,  5,  5,  5,  0,-10,
		   -10,  0,  5,  0,  0,  0,  0,-10,
		   -20,-10,-10, -5, -5,-10,-10,-20
	   },
	   {
		   -20,-10,-10, -5, -5,-10,-10,-20,
		   -10,  0,  5,  0,  0,  0,  0,-10,
		   -10,  5,  5,  5,  5,  5,  0,-10,
		   0,  0,  5,  5,  5,  5,  0, -5,
		   -5,  0,  5,  5,  5,  5,  0, -5,
		   -10,  0,  5,  5,  5,  5,  0,-10,
		   -10,  0,  0,  0,  0,  0,  0,-10,
		   -20,-10,-10, -5, -5,-10,-10,-20
	   }
	};

	static unsigned int pieceMaterial[7] = {0, 1, 3, 3, 5, 0, 9};

	bool isEndgame(const chessboard& board) {
		unsigned int materialWhite, materialBlack;
		for(unsigned int square = 0; square < 64; ++square) {
			int piece = board[square];
			if(piece != 0) {
				int pieceType = piece > 0 ? piece : -piece;
				if(piece > 0) { //white piece
					materialWhite += pieceMaterial[pieceType];
				} else { //black piece
					materialBlack += pieceMaterial[pieceType];
				}
			}
		}
		return materialWhite <= 13 && materialBlack <= 13;
	}

	int evaluateBoard(unsigned int side, chessboard& board, const game_information& info) {
		if(board.isInsufficientMaterial()) { //draw without evaluation
			return 0;
		}
		int sideMultiplier = side == white ? 1 : -1;
		unsigned int enemySide = 1-side;
		std::list<move> enemyMoves;
		move_generator generator(board, info);
		generator.generatePseudoLegalMoves(enemySide, enemyMoves); //generate all pseudo legal moves that the enemy can make here
		enemyMoves.remove_if([&](const move& m) { return !(isLegalMove(m, board, info, true).first); }); //filter out illegal moves
		bool enemyInCheck = isAttacked(board, side, board.getKingSquare(enemySide));
		if(enemyMoves.size() == 0) { //the enemy has no legal moves in this position
			if(enemyInCheck) { //checkmate
				return sideMultiplier * INT32_MAX;
			} else { //statelmate
				return 0;
			}
		}
		/*
		 * No mates, sufficient material, can begin material/mobility evaluation.
		 * The higher the score, the better this position is for the side to move.
		 */
		bool endgame = isEndgame(board);
		int evaluation = 0;
		for(int square = 0; square < 64; ++square) {
			int piece = board[square];
			if(piece != 0) { //piece here
				unsigned int pieceType = piece > 0 ? piece : -piece;
				unsigned int sideOfPiece = piece > 0 ? white : black;
				// Material evaluation
				if(sideOfPiece == white) { //white piece
					if(side == white) {
						evaluation += pieceValues[pieceType];
					} else {
						evaluation -= pieceValues[pieceType];
					}
				} else { //black piece
					if(side == white) {
						evaluation -= pieceValues[pieceType];
					} else {
						evaluation += pieceValues[pieceType];
					}
				}
				// piece square table evaluation
				if(pieceType == pawn) {
					evaluation += pawnTable[sideOfPiece][square];
				} else if(pieceType == knight) {
					evaluation += knightTable[sideOfPiece][square];
				} else if(pieceType == bishop) {
					evaluation += bishopTable[sideOfPiece][square];
				} else if(pieceType == rook) {
					evaluation += rookTable[sideOfPiece][square];
				} else if(pieceType == king) {
					if(endgame) {
						evaluation += kingEndgameTable[sideOfPiece][square];
					} else {
						evaluation += kingTable[sideOfPiece][square];
					}
				} else { //must be queen
					evaluation += queenTable[sideOfPiece][square];
				}
			}
		}

		return evaluation * sideMultiplier; //this multiplier makes it relative to the side to move
	}
}

