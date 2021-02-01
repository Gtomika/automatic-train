/*
 * greedy_player.cpp
 *
 *  Created on: 2021. febr. 1.
 *      Author: G�sp�r Tam�s
 */
#include <algorithm>
#include <limits>

#include "greedy_player.h"
#include "board/evaluation.h"

namespace tchess
{
	void greedy_player::makeMove(game& gameController) {
		const std::list<move>& gameMoves = gameController.getMoves();
		if(gameMoves.size() > 0) {
			move enemyMove = gameMoves.back(); //update our board with enemy move
			board.makeMove(enemyMove, 1-side);
			updateGameInformation(board, enemyMove, info); //update game information
		}
		std::list<move> moves;
		move_generator generator(board, info);
		generator.generatePseudoLegalMoves(side, moves); //generate all pseudo legal moves

		move bestMove; //stores current best move
		int bestEvaluation = INT32_MIN;
		//iterate pl moves
		for(auto it = moves.begin(); it != moves.end(); ++it) {
			std::pair<bool, int> legalityTest = isLegalMove(*it, board, info, false); //will MAKE the move, but not unmake
			if(legalityTest.first) {
				game_information infoAfterMove = info; //copy game info to not modify the original
				updateGameInformation(board, *it, infoAfterMove);
				//the move is legal, and it is made on the board, now evaluate
				int evaluation = -evaluateBoard(side, board, infoAfterMove);
				//The eval function will return higher score for better positions.
				if(evaluation >= bestEvaluation) { //new best move
					bestEvaluation = evaluation;
					bestMove = *it;
				}
				board.unmakeMove(*it, side, legalityTest.second); //unmake move after evaluation
			} else {
				//this move was illegal, dont evaluate it and unmake it
				board.unmakeMove(*it, side, legalityTest.second);
			}
		}
		board.makeMove(bestMove, side); //keep board updated
		updateGameInformation(board, bestMove, info);
		info.setSideToMove(1-side);
		gameController.submitMove(bestMove); //submit best greedy move
}

	std::string greedy_player::description() const {
		std::string sideName = side == white ? "White" : "Black";
		return "Greedy move maker (" + sideName + ")";
	}
}


