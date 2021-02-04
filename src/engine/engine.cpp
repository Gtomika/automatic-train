/*
 * engine.cpp
 *
 *  Created on: 2021. febr. 3.
 *      Author: Gáspár Tamás
 */

#include "engine.h"

namespace tchess
{
	const unsigned int default_depth = 5;

	move engine::makeMove(const game& gameController) {
		const std::list<move>& gameMoves = gameController.getMoves();
		if(gameMoves.size() > 0) {
			move enemyMove = gameMoves.back(); //update our board with enemy move
			board.makeMove(enemyMove, 1-side);
			updateGameInformation(board, enemyMove, info); //update game information
		}
		move bestMove; //TODO
		return bestMove;
	}

	std::string engine::description() const {
		std::string sideName = side == white ? "White" : "Black";
		return "Tchess engine (" + sideName + ")";
	}
}



