/*
 * engine.h
 *
 *	Tchess engine header file.
 *
 *  Created on: 2021. febr. 3.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_ENGINE_ENGINE_H_
#define SRC_ENGINE_ENGINE_H_

#include "game/player.h"

namespace tchess
{
	extern const unsigned int default_depth;

	/*
	 * Player agent for the Tchess engine.
	 */
	class engine: player {

		//The side on which this agent plays.
		unsigned int side;

		//Search depth.
		unsigned int depth;

		//Board object.
		chessboard board;

		//game info object.
		game_information info;

	public:
		engine() = delete;

		engine(unsigned int side, unsigned int depth = default_depth)
			: side(side), depth(depth) {}

		/*
		 * Will submit the best move it can find after evaluating the
		 * game tree down to a certain depth.
		 */
		void makeMove(game& gameController);

		std::string description() const;
	};

}

#endif /* SRC_ENGINE_ENGINE_H_ */
