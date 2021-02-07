/*
 * engine.h
 *
 *	Tchess engine header file.
 *
 *  Created on: 2021. febr. 3.
 *      Author: G�sp�r Tam�s
 */

#ifndef SRC_ENGINE_ENGINE_H_
#define SRC_ENGINE_ENGINE_H_

#include "game/player.h"
#include "polyglot.h"

namespace tchess
{
	/*
	 * Default tree search depth.
	 */
	extern const unsigned int default_depth;

	/*
	 * Player agent for the Tchess engine.
	 */
	class engine: public player {

		//The side on which this agent plays.
		unsigned int side;

		//Search depth.
		unsigned int depth;

		//Board object.
		chessboard board;

		//game info object.
		game_information info;

		//Stores if the engine is playing opening mode (from book) or not.
		bool opening;

		//Polyglot opening book.
		opening_book openingBook;

	public:
		engine() = delete;

		engine(unsigned int side, unsigned int depth = default_depth)
			: side(side), depth(depth), opening(true) {}

		/*
		 * Will submit the best move it can find after evaluating the
		 * game tree down to a certain depth.
		 */
		move makeMove(const game& gameController);

		std::string description() const;

	private:
		/*
		 * The root negamax function. Unlike the normal negamax, this
		 * will return the best move instead of the best evaluation.
		 */
		move alphaBetaNegamaxRoot();

		/*
		 * Negamax tree searching method with alpha-beta cutoff.
		 */
		int alphaBetaNegamax(int alpha, int beta, int depthLeft, game_information& gameInfo);
	};

}

#endif /* SRC_ENGINE_ENGINE_H_ */
