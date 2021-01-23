#include "move.h"
#include "board.h"

/*
 * move.cpp
 *
 * Implementations for move.h
 *
 *  Created on: 2021. jan. 23.
 *      Author: Gáspár Tamás
 */

inline bool tchess::move::isCapture() const {
	//need to check the second bit
	return flags[1];
}

inline bool tchess::move::isPromotion() const {
	//need to check first bit
	return flags[0];
}

inline bool tchess::move::isDoublePawnPush() const {
	//not capture, promotion and only second special bit is on
	return !flags[0] && !flags[1] && !flags[2] && flags[3];
}

inline bool tchess::move::isKingsideCastle() const {
	//not capture, promotion and only the first special bit is true
	return !flags[0] && !flags[1] && flags[2] && !flags[3];
}

inline bool tchess::move::isQueensideCastle() const {
	//not capture, promotion and both special bits are true
	return !flags[0] && !flags[1] && flags[2] && flags[3];
}

inline bool tchess::move::isEnPassant() const {
	//is a capture but not a promotion and only second special bit is true
	return !flags[0] && flags[1] && !flags[2] && flags[3];
}

unsigned int tchess::move::promotedTo() const {
	//assumes this a promotion, so only checking special bits
	int s1 = flags[2], s2 = flags[3];
	if(s1==1 && s2==1) return tchess::queen;
	if(s1==1 && s2==0) return tchess::rook;
	if(s1==0 && s2==1) return tchess::bishop;
	return tchess::knight;
}

std::string tchess::move::to_string() const {
	std::string moveString;

	return moveString;
}
