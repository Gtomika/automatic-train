/*
 * evaluation.h
 *
 *  Created on: 2021. febr. 1.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_BOARD_EVALUATION_H_
#define SRC_BOARD_EVALUATION_H_

#include "board.h"

namespace tchess
{
	//piece values
	extern const int pieceValues[7];

	//piece square tables (for both sides)
	extern const int pawnTable[2][64];

	extern const int knightTable[2][64];

	extern const int bishopTable[2][64];

	extern const int rookTable[2][64];

	extern const int kingTable[2][64];

	extern const int kingEndgameTable[2][64];

	extern const int queenTable[2][64];

	/*
	 * Returns true if the board is in the endgame. This method uses Speelman's
	 * definition: it is endgame if both sides have at most 13 worth of material (king not counted).
	 */
	bool isEndgame(const chessboard& board);

	/*
	 * Main static evaluator function. Used in negamax, so it evaluates relative to the side to move.
	 *
	 * This method must be called after the move has been made on the board,
	 * then after evaluation the move can be unmade.
	 *
	 * Parameters:
	 *  - side: the side who made the last move on the board and for whom the evaluation is happening.
	 *  - board: the board object
	 *  - info: game info object needed to generate the enemy moves (to check for mates)
	 */
	int evaluateBoard(unsigned int side, chessboard& board, const game_information& info);
}

#endif /* SRC_BOARD_EVALUATION_H_ */
