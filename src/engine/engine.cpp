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

	void makeMove(game& gameController) {

	}

	std::string engine::description() const {
		std::string sideName = side == white ? "White" : "Black";
		return "Tchess engine (" + sideName + ")";
	}
}



