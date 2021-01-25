#include <iostream>
#include <string>

#include "move.h"
#include "game.h";

/*
 * human_player.h
 *
 * Methods and classes related to the player controlled manually
 * by the user are here.
 *
 *  Created on: 2021. jan. 24.
 *      Author: Gáspár Tamás
 */

#ifndef SRC_HUMAN_HUMAN_PLAYER_H_
#define SRC_HUMAN_HUMAN_PLAYER_H_

namespace tchess
{
	//This message is printed when the user asks for help.
	extern const std::string helpMessage;

	/*
	 * This class represents the manually controlled player agent. It has a 'makeMove' method, and so
	 * it is eligible to be passed to a 'game' object.
	 * This class uses the console to interact with the user.
	 */
	class human_player_console {

		//Side of the player.
		unsigned int side;

		//Reference to the game object. Needed to call it
		template<typename WhitePlayer, typename BlackPlayer>
		game<WhitePlayer,BlackPlayer>& gameObject;

	public:
		template<typename WhitePlayer, typename BlackPlayer>
		human_player_console(unsigned int side, game<WhitePlayer,BlackPlayer>& gameObj) : side(side), gameObject(gameObj) {}

		/*
		 * Will prompt the user on the console to enter a move.
		 */
		void makeMove() const;

		/**
		 * Return a string describing this player as human controlled (includes played side).
		 */
		std::string description() const;
	};
}

#endif /* SRC_HUMAN_HUMAN_PLAYER_H_ */
