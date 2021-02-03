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

	struct special_board {
		bool special;
		int evaluation;
	};

	/*
	 * Checks if the position is special in a way that it does not need to be evaluated. For example
	 * mates and insufficient material. Enemy moves are only pseudo legal
	 */
	special_board isSpecialBoard(unsigned int enemySide, chessboard& board, const game_information& info, std::list<move>& enemyMoves) {
		if(board.isInsufficientMaterial()) { //draw without evaluation
			return {true, 0};
		}
		auto legalCheck = [&](const move& m) { return !(isLegalMove(m, board, info, true).first); };
		enemyMoves.remove_if(legalCheck); //filter out illegal moves
		if(enemyMoves.size() == 0) {
			bool enemyInCheck = isAttacked(board, 1-enemySide, board.getKingSquare(enemySide));
			if(enemyInCheck) {
				return {true, INT32_MAX};
			} else {
				return {true, 0};
			}
		}
		return {false, 0};
	}

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
				std::list<move> enemyMoves = createEnemyMoves(1-side, board, infoAfterMove);
				special_board sb = isSpecialBoard(1-side, board, infoAfterMove, enemyMoves);
				int evaluation = 0;
				if(sb.special) { //no need for static evaluation
					 evaluation = sb.evaluation;
				} else { //static evaluation
					evaluation = side == white ? evaluateBoard(side, board, infoAfterMove) : -evaluateBoard(side, board, infoAfterMove);
				}
				//std::cout << "evaluation " << evaluation << " for move " << (*it).to_string(1) << std::endl;
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



