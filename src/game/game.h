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
#include <list>

#include "board/board.h"
#include "board/move.h"
#include "player.h"

namespace tchess
{
	/**
	 * Allows the player to select who will play White and Black sides.
	 */
	char selectPlayerForSide(const std::string& sideName);

	/*
	 * Allows the user to select who will play on both sides. For example, player vs engine or
	 * player vs player.
	 */
	void selectPlayersAndStart();

	class player;

	/*
	 * This template connects the two player's sides. It maintaints the game board and checks if
	 * moves submitted are legal. IT also signals the players when it is their turn to move.
	 *
	 * The template parameters are the references for the two players. These objects must have a method
	 * called 'makeMove', which will tell the player/engine/whatever that its their time to make a move.
	 * They also must have a 'description' method which will return text info about the them.
	 *
	 * These player objects will then create their move and send it to the game object using 'submitMove'.
	 */
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
		player* whitePlayer;

		/**
		 * Reference to the black player.
		 */
		player* blackPlayer;

		/*
		 * The moves of the game are stored in this list.
		 */
		std::list<move> moves;

	public:
		game() = delete;

		/**
		 * Create the game, from references to both sides.
		 */
		game(player* wp, player* bp) : whitePlayer(wp), blackPlayer(bp) {}

		/**
		 * This method starts the game by requesting the first move from the white player.
		 */
		void beginGame();

		/**
		 * This method is used by the players to communicate with tha game object.
		 * The received moves are validated.
		 */
		void submitMove(const move& m);

		/*
		 * This will return the last move made in the game. Useful for player agents
		 * who maintain a board representation, so they can make the moves of the opponent.
		 */
		move getLastMove() const;

	private:

		/**
		 * This method checks if a move is valid. It will take into consideration the board and
		 * the game_information object.
		 *
		 * THIS WILL MAKE THE MOVE ON THE BOARD!!! That is needed to check for legality. If the move turns out to
		 * be illegal, the move must be unmaked.
		 */
		move_legality_result isValidMove(const move& playerMove, std::list<move>& pseudoLegalMoves);

		/*
		 * Called when the game has ended. Prints information about the ending.
		 */
		void endGame(bool draw, const std::string& winninSide, const std::string& message);
	};
}

#endif /* SRC_GAME_GAME_H_ */
