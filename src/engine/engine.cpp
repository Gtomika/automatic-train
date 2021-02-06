/*
 * engine.cpp
 *
 *  Created on: 2021. febr. 3.
 *      Author: Gáspár Tamás
 */

#include <list>
#include <limits>

#include "engine.h"
#include "board/evaluation.h"

namespace tchess
{
	const unsigned int default_depth = 4;

	static move testMove(22,54, capture);

	move engine::alphaBetaNegamaxRoot() {
		unsigned int side = info.getSideToMove();
		//create legal moves for this board and side
		std::list<move> moves;
		move_generator generator(board, info);
		generator.generatePseudoLegalMoves(side, moves);
		auto legalCheck = [&](const move& m) { return !(isLegalMove(m, board, info)); };
		moves.remove_if(legalCheck);
		//we cant be at maximum depth, since this is the root call
		move bestMove;
		int bestEvaluation = INT32_MIN+1;

		int count = 0;
		for(const move& _move: moves) { //iterate legal moves
			int p = board[_move.getFromSquare()];
			p = p > 0 ? p : -p;
			std::cout << "\rAnalyzing " << ++count << ". move out of " << moves.size()
								<< ", move: " << _move.to_string(p);
			int capturedPiece = board.makeMove(_move, side);
			game_information infoAfterMove = info; //create a game info object
			updateGameInformation(board, _move, infoAfterMove); //update new info object with move
			int evaluation = -alphaBetaNegamax(INT32_MIN+1, INT32_MAX, depth, infoAfterMove); //move down in the tree
			board.unmakeMove(_move, side, capturedPiece); //unmake the move before moving on
			if(evaluation >= bestEvaluation) {
				bestEvaluation = evaluation;
				bestMove = _move;
			}
		}
		std::cout << std::endl;
		return bestMove;
	}

	int engine::alphaBetaNegamax(int alpha, int beta, int depthLeft, game_information& gameInfo) {
		unsigned int side = gameInfo.getSideToMove();
		//create legal moves for this board and side
		std::list<move> moves;
		move_generator generator(board, gameInfo);
		generator.generatePseudoLegalMoves(side, moves);
		auto legalCheck = [&](const move& m) { return !(isLegalMove(m, board, gameInfo)); };
		moves.remove_if(legalCheck); //filter out illegal moves

		if(depthLeft == 0) { //we are at maximum search depth, evaluate
			special_board sb = isSpecialBoard(side, board, gameInfo, moves); //detect mates and drawn games
			if(sb.special) {
				return sb.evaluation; //return special evaluation
			} else {
				return evaluateBoard(side, board, gameInfo); //evaulate non special board
			}
		}

		int bestEvaluation = INT32_MIN+1;

		for(const move& move: moves) { //iterate legal moves
			int capturedPiece = board.makeMove(move, side);
			game_information infoAfterMove = gameInfo; //create a game info object
			updateGameInformation(board, move, infoAfterMove); //update new info object with move
			int evaluation = -alphaBetaNegamax(-beta, -alpha, depthLeft - 1, infoAfterMove); //move down in the tree
			board.unmakeMove(move, side, capturedPiece); //unmake the move before moving on
			if(evaluation > bestEvaluation) {
				bestEvaluation = evaluation;
			}
			if(bestEvaluation > alpha) {
				alpha = bestEvaluation;
			}
			if(bestEvaluation >= beta) {
				break;
			}
		}
		return alpha;
	}

	move engine::makeMove(const game& gameController) {
		const std::list<move>& gameMoves = gameController.getMoves();
		if(gameMoves.size() > 0) {
			move enemyMove = gameMoves.back(); //update our board with enemy move
			board.makeMove(enemyMove, 1-side);
			updateGameInformation(board, enemyMove, info); //update game information
		}
		//std::cout << "My board before my move:\n" << board.to_string();
		move bestMove = alphaBetaNegamaxRoot();
		//update out board with the selected move
		board.makeMove(bestMove, side); //keep board updated
		updateGameInformation(board, bestMove, info);
		//std::cout << "My board after my move:\n" << board.to_string();
		return bestMove;
	}

	std::string engine::description() const {
		std::string sideName = side == white ? "White" : "Black";
		return "Tchess engine (" + sideName + ")";
	}
}



