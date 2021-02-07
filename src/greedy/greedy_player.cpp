/*
 * greedy_player.cpp
 *
 *  Created on: 2021. febr. 1.
 *      Author: Gáspár Tamás
 */
#include <algorithm>
#include <limits>

#include "greedy_player.h"
#include "board/evaluation.h"

namespace tchess
{
	std::list<move> createEnemyMoves(unsigned int enemySide, const chessboard& board, const game_information& info) {
		std::list<move> enemyMoves;
		move_generator generator(board, info);
		generator.generatePseudoLegalMoves(enemySide, enemyMoves);
		return enemyMoves;
	}

	move greedy_player::makeMove(const game& gameController) {
		const std::list<move>& gameMoves = gameController.getMoves();
		if(gameMoves.size() > 0) {
			move enemyMove = gameMoves.back(); //update our board with enemy move
			board.makeMove(enemyMove, 1-side);
			updateGameInformation(board, enemyMove, info); //update game information
		}
		std::list<move> moves;
		move_generator generator(board, info);
		generator.generatePseudoLegalMoves(side, moves); //generate all pseudo legal moves
		auto legalCheck = [&](const move& m) { return !(isLegalMove(m, board, info)); }; //used later
		moves.remove_if(legalCheck); //filter out illegal moves

		move bestMove; //stores current best move
		int bestEvaluation = INT32_MIN;
		//iterate all legal moves to greedily find best move
		for(auto it = moves.begin(); it != moves.end(); ++it) {
			int capturedPiece = board.makeMove(*it, side);
			game_information infoAfterMove = info; //copy game info to not modify the original
			updateGameInformation(board, *it, infoAfterMove);
			//the move is legal, and it is made on the board, now evaluate
			std::list<move> enemyMoves = createEnemyMoves(1-side, board, infoAfterMove);
			enemyMoves.remove_if(legalCheck); //filter out illegal moves
			special_board sb = isSpecialBoard(1-side, board, infoAfterMove, enemyMoves, 1);
			int evaluation = 0;
			if(sb.special) { //no need for static evaluation
				 evaluation = sb.evaluation;
			} else { //static evaluation
				evaluation = evaluateBoard(side, board, infoAfterMove);
			}
			//std::cout << "evaluation " << evaluation << " for move " << (*it).to_string(1) << std::endl;
			//The eval function will return higher score for better positions.
			if(evaluation >= bestEvaluation) { //new best move
				bestEvaluation = evaluation;
				bestMove = *it;
			}
			board.unmakeMove(*it, side, capturedPiece); //unmake move after evaluation
		}
		board.makeMove(bestMove, side); //keep board updated
		updateGameInformation(board, bestMove, info);
		info.setSideToMove(1-side);
		return bestMove; //submit best greedy move
}

	std::string greedy_player::description() const {
		std::string sideName = side == white ? "White" : "Black";
		return "Greedy move maker (" + sideName + ")";
	}
}



