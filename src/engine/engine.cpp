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
		int bestEvaluation = WORST_VALUE;

		int count = 0;
		for(const move& _move: moves) { //iterate legal moves
			int p = board[_move.getFromSquare()];
			p = p > 0 ? p : -p;
			//this is not working in eclipse console but does in normal console!
			std::cout << "\rAnalyzing " << ++count << ". move out of " << moves.size()
								<< ", move: " << _move.to_string(p) << "           "; //<- to delete whole line
			int capturedPiece = board.makeMove(_move, side);
			game_information infoAfterMove = info; //create a game info object
			updateGameInformation(board, _move, infoAfterMove); //update new info object with move
			int evaluation = -alphaBetaNegamax(WORST_VALUE, INT32_MAX, depth-1, infoAfterMove); //move down in the tree
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
			special_board sb = isSpecialBoard(side, board, gameInfo, moves, depth - depthLeft); //detect mates and drawn games
			if(sb.special) {
				return sb.evaluation; //return special evaluation
			} else {
				return evaluateBoard(side, board, gameInfo); //evaulate non special board
			}
		}

		int bestEvaluation = WORST_VALUE;

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
		move bestMove;
		move bookMove(0,0,quietMove);
		if(opening) bookMove = openingBook.getBookMove(board, info);
		if(!(bookMove == move(0,0,quietMove))) { //found a book opening
			std::cout << "I am playing from my opening book!" << std::endl;
			bestMove = bookMove;
		} else {
			std::cout << "I am looking for my move..." << std::endl;
			opening = false; //not in the opening anymore
			bestMove = alphaBetaNegamaxRoot();
		}
		//std::cout << "My board before my move:\n" << board.to_string();
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



