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

#include <iostream>
#include <string>

#include "board/move.h"
#include "game/game.h"

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

	public:
		human_player_console(unsigned int side) : side(side) {}

		human_player_console(const human_player_console& other) : side(other.side) {}

		/*
		 * Will prompt the user on the console to enter a move. The resulting move has to
		 * be sent to the controller object.
		 */
		template<typename WhitePlayer, typename BlackPlayer>
		void makeMove(game<WhitePlayer, BlackPlayer>& controller) const {
			bool moveParsed = false;
			while(!moveParsed) { //if use types help or something unparsable this will be printed again
				std::cout << "--------------------------------" << std::endl;
				std::cout << "Enter your move, or type 'help'!" << std::endl;
				std::string input;
				std::getline(std::cin, input);

				if(input == "help") { //sked for help
					std::cout << helpMessage;
				} else { //try to parse a move
					try {
						move m = parse_move(input, side);
						moveParsed = true;
						controller.submitMove(m); //send move back to game controller
					} catch(move_parse_exception& exc) {
						std::cout << "Could not parse this move: " << exc.what() << std::endl;
					}
				}
			}
		}

		/**
		 * Return a string describing this player as human controlled (includes played side).
		 */
		std::string description() const;
	};
}

#endif /* SRC_HUMAN_HUMAN_PLAYER_H_ */
