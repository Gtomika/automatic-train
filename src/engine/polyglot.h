/*
 * polyglot.h
 *
 *  Created on: 2021. febr. 6.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_ENGINE_POLYGLOT_H_
#define SRC_ENGINE_POLYGLOT_H_

#include "board/board.h"

#ifdef _MSC_VER
#  define U64(u) (u##ui64)
#else
#  define U64(u) (u##ULL)
#endif

//Unsigned 64 bit integer type definition.
typedef unsigned long long uint64;

namespace tchess
{
	//Random array used by polyglot books.
	extern const uint64 Random64[781];

	/*
	 * Creates the polygot hash key of a board. Can be used to
	 * look up positions in the opening book.
	 */
	uint64 polyKeyFromBoard(const chessboard& board, const game_information& info);

	/*
	 * Converts a piece code used in Tchess to a polyglot
	 * piece code.
	 */
	unsigned int polyPieceCode(int pieceCode);
}

#endif /* SRC_ENGINE_POLYGLOT_H_ */
