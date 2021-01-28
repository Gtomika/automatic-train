/*
 * game.h
 *
 * Header file of the game class which is the central class connecting the two players.
 *
 *  Created on: 2021. jan. 24.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_GAME_GAME_H_
#define SRC_GAME_GAME_H_

#include <iostream>

#include "board/board.h"
#include "board/move.h"

namespace tchess
{
	//Identifier for the white player.
	extern const unsigned int white;

	//Identifier for the black player.
	extern const unsigned int black;

	//Constant that indicated no en passant capture is possible.
	extern const int noEnPassant;

	/*
	 * The chessboard class which stores the current board, and allows for making and
	 * unmaking moves. This class won't check for the validity of the moves, instead it assumes the
	 * moves received are at least pseudo legal.
	 */
	class chessboard {

		//Squares of the board
		int squares[64];

	public:
		//Creates a chessboard as it is at the start of the game.
		chessboard();

		/*
		 * This operator is used to access the squares of the board.
		 */
		inline int operator[](unsigned int i) const {
			return squares[i];
		}

		/*
		 * Modifies the chessboard according to the given move. Returns the code of the
		 * captured piece (or 'empty' if there was no capture). This return value can later
		 * be used by the 'unmakeMove' method to restore the board's previous state.
		 */
		int makeMove(const move&, unsigned int side);

		/*
		 * Unmakes the modifications made by the given move. This requires what the captured piece
		 * was, becase the move object does not store this information.
		 */
		void unmakeMove(const move&, unsigned int side, int capturedPiece);

		/*
		 * Mostly a debug method that will create a string representation of the board that can
		 * be printed to see if the move making/unmaking was correct. It uses the letters of each
		 * piece, and 0 for empty squares. Black pieces have a - sign in front of them.
		 *
		 * For example:
		 * ---------------------------
		 * | -R -N -B -Q -K -B -N -R |
		 * | -P -P -P -P -P -P -P -P |
		 * |  0  0  0  0  0  0  0  0 |
		 * |  0  0  0  0  0  0  0  0 |
		 * |  0  0  0  0  0  0  0  0 |
		 * |  0  0  0  0  0  0  0  0 |
		 * |  P  P  P  P  P  P  P  P |
		 * |  R  N  B  Q  K  B  N  R |
		 * ---------------------------
		 */
		std::string to_string() const;
	};

	/*
	 * This class stores information about a chess game. Every kind of information
	 * is stored in pairs where the first part of the pair is for white, the second
	 * is for black.
	 */
	class game_information {

		/**
		 * Stores which side must make the next move. IS either the white or the black constant.
		 */
		unsigned int sideToMove;

		/*
		 * Stores if the players have right to castle kingside.
		 * NOTE: this does not mean that the player can castle in the next move, since they might have pieces between the
		 * king and the rook, or the king may be in check. This only means that a side has or doesn't have the right to castle,
		 * if possible.
		 */
		bool kingsideCastleRight[2];

		/**
		 * Stores if the players have right to castle queenside.
		 * NOTE: this does not mean that the player can castle in the next move, since they might have pieces between the
		 * king and the rook, or the king may be in check. This only means that a side has or doesn't have the right to castle,
		 * if possible.
		 */
		bool queensideCastleRight[2];

		/**
		 * Stores if the players can perform en passant captures in the next move, and at which squares.
		 * If no such capture is possible, then the value is the 'noEnPassant' constant.
		 */
		int enPassantCaptureSquares[2];

	public:

		/**
		 * Creates an information object where all properties are set as they are
		 * in the beginning of a game.
		 */
		game_information();

		/**
		 * This method stores that a side is no longer eligible for kingside castling. For example, if a
		 * side has moved their king then his will be called.
		 * Since castling right can't be gained back once lost, this method can only disable them.
		 * Side must be either the white or the black constant.
		 */
		void disableKingsideCastleRight(unsigned int side);

		/**
		* This method stores that a side is no longer eligible for queenside castling. For example, if a
		* side has moved their king then his will be called.
		* Since castling right can't be gained back once lost, this method can only disable them.
		* Side must be either the white or the black constant.
		*/
		void disableQueensideCastleRight(unsigned int side);

		/**
		 * Marks a square as a possible en passant capture square for a side.
		 * Side must be either the white or the black constant.
		 * To indicate that an en passant is no longer possible, use the 'noEnPassant' constant.
		 */
		void setEnPassantSquare(unsigned int side, unsigned int square);

		/*
		 * Getter for en passant square property.
		 */
		inline unsigned int getEnPassantSquare(unsigned int side) const {
			return enPassantCaptureSquares[side];
		}

		/*
		 * Getter for kingside castle rights.
		 */
		inline bool getKingsideCastleRights(unsigned int side) const {
			return kingsideCastleRight[side];
		}

		/*
		 * Getter for queenside castle rights.
		 */
		inline bool getQueensideCastleRights(unsigned int side) const {
			return queensideCastleRight[side];
		}

		/**
		 * Update the side to move variable.
		 */
		inline void setSideToMove(unsigned int side) {
			sideToMove = side;
		}

		/*
		 * Getter for the side to move variable.
		 */
		inline unsigned int getSideToMove() {
			return sideToMove;
		}
	};

	bool isUnderAttack(unsigned int side, unsigned int square, const chessboard& board);

	/*
	 * This template connects the two players sides. It maintaints the game board and checks if
	 * moves submitted are legal. IT also signals the players when it is their turn to move.
	 *
	 * The template parameters are the refernces for the two player. These objects must have a method
	 * called 'makeMove', which will tell the player/engine/whatever that its their time to make a move.
	 * They also must have a 'description' method which will return text info about the player.
	 *
	 * These player objects will then create their move and send it to the game object using 'submitMove'.
	 */
	template<typename WhitePlayer, typename BlackPlayer>
	class game {

		/**
		 * The object that stores the current state of the board.
		 */
		chessboard board;

		/**
		 * Game information object, see above.
		 */
		game_information info;

		/**
		 * Reference to the white player.
		 */
		WhitePlayer& whitePlayer;

		/**
		 * Reference to the black player.
		 */
		BlackPlayer& blackPlayer;

	public:
		game() = delete;

		/**
		 * Create the game, from references to both sides.
		 */
		game(WhitePlayer& wp, BlackPlayer& bp) : whitePlayer(wp), blackPlayer(bp) {}

		/**
		 * This method starts the game by requesting the first move from the white player.
		 */
		void beginGame() {
			std::cout << "The game begins: " << whitePlayer.description() << " vs " << blackPlayer.description() << std::endl;
			whitePlayer.makeMove(*this); //white player will call 'submitMove'.
		}

		/**
		 * This method is used by the players to communicate with tha game object.
		 * The received moves are validated.
		 */
		void submitMove(const move& m) {
			std::string playerWhoMoved = info.getSideToMove()==white ? whitePlayer.description() : blackPlayer.description();
			if(isValidMove(m)) {
				std::cout << playerWhoMoved << " has made the move " << m.to_string() << std::endl;
				//TODO: update board
				//update side to move
				info.setSideToMove(info.getSideToMove() == white ? black : white);
				//ask for the next move
				info.getSideToMove() == white ? whitePlayer.makeMove(*this) : blackPlayer.makeMove(*this);
			} else { //move is invalid
				//TODO: end game
				std::cout << playerWhoMoved << " has made an illegal move, and lost the game!" << std::endl;
			}
		}

	private:

		/**
		 * This method checks if a move is valid. It will take into consideration the board and
		 * the game_information object.
		 */
		bool isValidMove(const move& m) const {
			return true; //TODO
		}
	};
}

#endif /* SRC_GAME_GAME_H_ */
