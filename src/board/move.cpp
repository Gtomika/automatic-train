#include <vector>

#include <boost/tokenizer.hpp>

#include "move.h"
#include "board.h"
#include "../game/game.h"

/*
 * move.cpp
 *
 * Implementations for move.h
 *
 *  Created on: 2021. jan. 23.
 *      Author: Gáspár Tamás
 */

namespace tchess
{
	//Move code for a "non-special" move that results in no capture.
	extern const unsigned int quietMove = 0;

	//Move code for double pawn pushes.
	extern const unsigned int doublePawnPush = 1;

	//Move code for kingside castle.
	extern const unsigned int kingsideCastle = 2;

	//Move code for queenside castle.
	extern const unsigned int queensideCastle = 3;

	//Move code for captures.
	extern const unsigned int capture = 4;

	//Move code for en-passant captures.
	extern const unsigned int enPassantCapture = 5;

	//Move code for a pawn push that results in a knight promotion.
	extern const unsigned int knightPromotion = 8;

	//Move code for a pawn push that results in a bishop promotion.
	extern const unsigned int bishopPromotion = 9;

	//Move code for a pawn push that results in a rook promotion.
	extern const unsigned int rookPromotion = 10;

	//Move code for a pawn push that results in a queen promotion.
	extern const unsigned int queenPromotion = 11;

	//Move code for a pawn push that captures and results in a knight promotion.
	extern const unsigned int knightPromotionCap = 12;

	//Move code for a pawn push that captures and results in a bishop promotion.
	extern const unsigned int bishopPromotionCap = 13;

	//Move code for a pawn push that captures and results in a rook promotion.
	extern const unsigned int rookPromotionCap = 14;

	//Move code for a pawn push that captures and results in a queen promotion.
	extern const unsigned int queenPromotionCap = 15;

	unsigned int move::promotedTo() const {
		//assumes this a promotion, so only checking special bits
		int s1 = flags[2], s2 = flags[3];
		if(s1==1 && s2==1) return queen;
		if(s1==1 && s2==0) return rook;
		if(s1==0 && s2==1) return bishop;
		return knight;
	}

	bool move::operator==(const move& other) const {
		if(isPromotion()) {
			return fromSquare == other.fromSquare && toSquare == other.toSquare &&
					promotedTo() == other.promotedTo();
		} else { //the kings dep. and dest. squares are enough to check castle equality as well
			return fromSquare == other.fromSquare && toSquare == other.toSquare;
		}
	}

	move& move::operator=(const move& other) {
		fromSquare = other.fromSquare;
		toSquare = other.toSquare;
		flags = other.flags;
		return *this;
	}

	//Helper method to convert piece codes into letters
	char pieceNameFromCode(int pieceCode) {
		switch(pieceCode) {
		case 1:
			return 'P';
			break;
		case 2:
			return 'N';
			break;
		case 3:
			return 'B';
			break;
		case 4:
			return 'R';
			break;
		case 5:
			return 'K';
			break;
		case 6:
			return 'Q';
			break;
		default:
			return '?';
		}
	}

	std::string move::to_string(int pieceThatMoved) const {
		if(isKingsideCastle()) { //special moves
			return "o-o";
		} else if(isQueensideCastle()) {
			return "o-o-o";
		}
		std::string moveString;
		moveString += pieceNameFromCode(pieceThatMoved);
		moveString += " ";
		moveString += createSquareName(fromSquare);
		if(isCapture()) { //write x if capture
			moveString += " x ";
		} else {
			moveString += " ";
		}
		moveString += createSquareName(toSquare);
		if(isPromotion()) { //write what it was promoted to if it was promotion
			moveString += " = ";
			int prom = promotedTo();
			moveString += pieceNameFromCode(prom);
		}
		if(isEnPassant()) { //indicate en passant
			moveString += " (en passant)";
		}
		return moveString;
	}

	char const * move_parse_exception::what() const throw() {
		return message.c_str();
	}

	//Helper method that detects if the move is a promotion (assumes pawn move, correct square names)
	bool checkForPromotion(const std::string& fromSquareName, const std::string& toSquareName, unsigned int side) {
		if(side == white) {
			//white promoted if the move is from the 7. rank to the 8. rank
			return fromSquareName[1] == 7 && toSquareName[1] == 8;
		} else { //black
			//black promoted if the move is from the 2. rank to the 1. rank
			return fromSquareName[1] == 2 && toSquareName[1] == 1;
		}
	}

	move parse_move(const std::string& moveString, unsigned int side) {
		int fromSquare, toSquare;
		if(moveString == "o-o") {
			//get kingside castle squares for KING, depending on side
			fromSquare = side == white ? 60 : 4 ;
			toSquare = side == white ? 62 : 6 ;
			return move(fromSquare, toSquare, kingsideCastle);
		} else if(moveString == "o-o-o") {
			//get queenside castle squares for KING, depending on side
			fromSquare = side == white ? 60 : 4 ;
			toSquare = side == white ? 58 : 2 ;
			return move(fromSquare, toSquare, queensideCastle);
		}
		//move is not castle, so there must be piece code, departure and destination squares specified
		std::vector<std::string> splitMove;
		boost::tokenizer<> tok(moveString);
		for(boost::tokenizer<>::iterator it=tok.begin(); it != tok.end(); ++it) {
			splitMove.push_back(*it);
		}
		int splitSize = splitMove.size();
		if(splitSize < 3) {
			throw move_parse_exception("Not enough move arguments!");
		}
		//the first token is the moved piece code
		std::string p = splitMove[0];
		if(p!="K" && p!="Q" && p!="P" && p!="R" && p!="N" && p!="B") {
			throw move_parse_exception("Unrecognized piece code!");
		}
		//the second + third argument should be the departure/destination squares
		try {
			fromSquare = createSquareNumber(splitMove[1]);
			toSquare = createSquareNumber(splitMove[2]);
			//if the move is a promotion
			if(p=="P" && checkForPromotion(splitMove[1], splitMove[2], side)) {
				//must have a promotion piece code as well in this case
				if(splitSize < 4) {
					throw move_parse_exception("This move appears to be a promotion, but no promotion piece code was found!");
				}
				std::string pProm = splitMove[3];
				unsigned int promMoveType;
				if(pProm == "Q") {
					promMoveType = queenPromotion;
				} else if(pProm == "R") {
					promMoveType = rookPromotion;
				} else if(pProm == "N") {
					promMoveType = knightPromotion;
				} else if(pProm == "B") {
					promMoveType = bishopPromotion;
				} else {
					throw move_parse_exception("Unrecognized promotion piece code!");
				}
				//move is a promotion, with a valid promotion piece code
				return move(fromSquare, toSquare, promMoveType);
			} else { //this move does not appear to be a promotion
				/*
				 * This move may be a capture or some other non promotion/non castle type,
				 * but we have no way of knowing that here, so just using quiet move type.
				 */
				return move(fromSquare, toSquare, quietMove);
			}
		} catch (std::runtime_error& e) { //failed to parse square names
			throw move_parse_exception("Unrecognized square name/names!");
		}
	}
}
