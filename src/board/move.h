/*
 * move.h
 *
 * This header contains the move class that describes chess moves.
 *
 *  Created on: 2021. jan. 23.
 *      Author: G�sp�r Tam�s
 */

#ifndef SRC_BOARD_MOVE_H_
#define SRC_BOARD_MOVE_H_

#include <string>
#include <bitset>
#include <stdexcept>

namespace tchess
{
	//Move code for a "non-special" move that results in no capture.
	extern const unsigned int quietMove;

	//Move code for double pawn pushes.
	extern const unsigned int doublePawnPush;

	//Move code for kingside castle.
	extern const unsigned int kingsideCastle;

	//Move code for queenside castle.
	extern const unsigned int queensideCastle;

	//Move code for captures.
	extern const unsigned int capture;

	//Move code for en-passant captures.
	extern const unsigned int enPassantCapture;

	//Move code for a pawn push that results in a knight promotion.
	extern const unsigned int knightPromotion;

	//Move code for a pawn push that results in a bishop promotion.
	extern const unsigned int bishopPromotion;

	//Move code for a pawn push that results in a rook promotion.
	extern const unsigned int rookPromotion;

	//Move code for a pawn push that results in a queen promotion.
	extern const unsigned int queenPromotion;

	//Move code for a pawn push that captures and results in a knight promotion.
	extern const unsigned int knightPromotionCap;

	//Move code for a pawn push that captures and results in a bishop promotion.
	extern const unsigned int bishopPromotionCap;

	//Move code for a pawn push that captures and results in a rook promotion.
	extern const unsigned int rookPromotionCap;

	//Move code for a pawn push that captures and results in a queen promotion.
	extern const unsigned int queenPromotionCap;

	/*
	 * This exception is thrown when a move string could not
	 * be parsed into a move.
	 */
	class move_parse_exception: public std::exception {
		std::string message;
	public:
		move_parse_exception(char const* const message) throw() : message(message) {}
		virtual char const* what() const throw();
	};

	/*
	 * Stores the result of a move legality test. Used to return not only the boolean value, but
	 * the reason for illegality as well.
	 */
	class move_legality_result {

		const bool legal;

		std::string information;

		//Needed to unmake move
		const int capturedPiece;

	public:
		move_legality_result(bool legal, const std::string& info, int cp) : legal(legal), information(info), capturedPiece(cp) {}

		bool isLegal() const {
			return legal;
		}

		std::string getInformation() const {
			return information;
		}

		int getCapturedPiece() const {
			return capturedPiece;
		}
	};

	/**
	 * Represents a chess move, using the departure square (from) and the destination
	 * square (to).
	 *
	 * Also includes 4 flags in a bitfield. These flags are:
	 *  - Bit 1: capture flag
	 *  - Bit 2: promotion flag
	 *  - Bit 3: Special flag 1
	 *  - Bit 4: Special flag 2
	 *
	 * The possible bitfields are declared as constants in this file. For example, bishop promotion capture
	 * is 13 (binary 1101), so both capture and promotion flags are 1, but only the second special flag is 1.
	 *
	 * For castling the departure and destination squares of the move will be the departure and destination
	 * of the king.
	 */
	class move {

		//Departure square of the move.
		unsigned int fromSquare;

		//Destination square of the move.
		unsigned int toSquare;

		//The 4 bit wide bitfield containing the flags.
		std::bitset<4> flags;

	public:
		//Create a move object
		move(unsigned int fromSquare, unsigned int toSquare, unsigned int flags) :
			fromSquare(fromSquare), toSquare(toSquare), flags(flags) {}

		//Checks if this move is a capture.
		inline bool isCapture() const {
			//need to check the second bit (bitset goes backwards...)
			return flags[2];
		}

		//Checks if this move is a promotion.
		inline bool isPromotion() const {
			//need to check first bit  (bitset goes backwards...)
			return flags[3];
		}

		//Checks if this move is a double pawn push.
		inline bool isDoublePawnPush() const {
			//not capture, promotion and only second special bit is on  (bitset goes backwards...)
			return !flags[3] && !flags[2] && !flags[1] && flags[0];
		}

		//Checks if this move is a kingside castle.
		inline bool isKingsideCastle() const {
			//not capture, promotion and only the first special bit is true
			return !flags[3] && !flags[2] && flags[1] && !flags[0];
		}

		//Checks if this move is a queenside castle.
		inline bool isQueensideCastle() const {
			//not capture, promotion and both special bits are true
			return !flags[3] && !flags[2] && flags[1] && flags[0];
		}

		//Checks if this move is an en-passant capture.
		inline bool isEnPassant() const {
			//is a capture but not a promotion and only second special bit is true
			return !flags[3] && flags[2] && !flags[1] && flags[0];
		}

		/*
		 * Returns the piece code of the piece the paawn was promoted into.
		 * This should only be called if the move is a promotion. Piece codes are
		 * found in board.h.
		 *
		 * This only returns the type of piece (only positive). For black, this should be negated.
		 */
		unsigned int promotedTo() const;

		inline unsigned int getFromSquare() const { return fromSquare; }

		inline unsigned int getToSquare() const { return toSquare; }

		/*
		 * Equality check between 2 moves. They are equal if both the departure
		 * and the destination squares are equal. In case of promotions, the equality
		 * of the promoted piece is also checked.
		 */
		bool operator==(const move& other) const;

		//Creates a string format of the move.
		std::string to_string() const;
	};

	/*
	 * Parses a move string into a move object. This method performs some
	 * checks in the input, but it does not guarantee that the move is legal.
	 * Basically this method will accept the move if it can parse it, but it
	 * is up to the game object to check if the player can actually make this move.
	 *
	 * The parsed move may also be "incomplete", because the parser cannot know
	 * whether the move is a capture or not.
	 */
	move parse_move(const std::string& moveString, unsigned int side);
}

#endif /* SRC_BOARD_MOVE_H_ */
