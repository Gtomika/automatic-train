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

namespace tchess
{
	char selectPlayerForSide(const std::string& sideName);

	void selectPlayersAndStart();

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

		/*
		 * The moves of the game are stored in this list.
		 */
		std::list<move> moves;

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
			std::cout << board.to_string();
			std::string playerWhoMoves = info.getSideToMove()==white ? whitePlayer.description() : blackPlayer.description();
			std::cout << playerWhoMoves << "'s turn to move..." << std::endl;
			whitePlayer.makeMove(*this); //white player will call 'submitMove'.
		}

		/**
		 * This method is used by the players to communicate with tha game object.
		 * The received moves are validated.
		 */
		void submitMove(const move& m) {
			unsigned int side = info.getSideToMove();
			std::string playerWhoMoved = side==white ? whitePlayer.description() : blackPlayer.description();

			std::list<move> pseudoLegalMoves; //generate pseudo legal moves, will be needed at least for move validation
			move_generator generator(board, info);
			generator.generatePseudoLegalMoves(side, pseudoLegalMoves);

			int pieceThatMoved = board[m.getFromSquare()]; //will be needed later, if the move is legal (only the type)
			pieceThatMoved = pieceThatMoved >= 0 ? pieceThatMoved : -pieceThatMoved;
			move_legality_result result = isValidMove(m, pseudoLegalMoves); //make the move on the board while checking

			if(result.isLegal()) { //move is legal
				moves.push_back(m); //save this move
				//update game information, such as castling rights and side to move
				updateGameInformation(board, m, info);

				/*
				 * Check if the game has ended: checkmate, stalemate, repetition, etc.
				 * For this, all legal moves of the side to move is needed.
				 * Also need to know is the side to move is in check.
				 */
				bool check = isAttacked(board, side, board.getKingSquare(1-side)), checkmate = false, stalemate = false;
				generator.generatePseudoLegalMoves(1-side, pseudoLegalMoves); //all pseudo legal moves of the side to move
				unsigned int legalMoveCount = 0;
				for(const move& plMove: pseudoLegalMoves) {
					move_legality_result legRes = isValidMove(plMove, pseudoLegalMoves); //this made the move on the board!!!
					if(legRes.isLegal()) { //found at least 1 legal move, cant be stalemate, checkmate
						std::cout << "Legal move for the opponent: " << plMove.to_string(1) << std::endl;
						++legalMoveCount;
						board.unmakeMove(plMove, 1-side, legRes.getCapturedPiece()); //unmake tested move
						break;
					}
					board.unmakeMove(plMove, 1-side, legRes.getCapturedPiece()); //unmake tested move
				}
				if(legalMoveCount == 0) { //no legal moves, must be checkmate or stalemate
					if(check) {
						checkmate = true;
					} else {
						stalemate = true;
					}
				}
				std::cout << playerWhoMoved << " has made the move: " << m.to_string(pieceThatMoved);
				if(checkmate) {
					std::cout << " (checkmate)" << std::endl;
					endGame(false, playerWhoMoved, "Checkmate");
					return;
				} else if(stalemate) {
					std::cout << " (stalemate)" << std::endl;
					endGame(true, "", "Stalemate");
				} else if(check) {
					std::cout << " (check)";
				}
				std::cout << std::endl;
				std::cout << board.to_string();

				side = info.getSideToMove();
				std::string playerWhoMoves= side==white ? whitePlayer.description() : blackPlayer.description();
				std::cout << playerWhoMoves << "'s turn to move..." << std::endl;
				//ask for the next move
				side == white ? whitePlayer.makeMove(*this) : blackPlayer.makeMove(*this);
			} else { //move is invalid
				std::cout << playerWhoMoved << " has made an illegal move: " << result.getInformation() << std::endl;
				board.unmakeMove(m, side, result.getCapturedPiece()); //unmakde the illegal move on the board
				side == white ? whitePlayer.makeMove(*this) : blackPlayer.makeMove(*this); //ask for a new move
			}
		}

	private:

		/**
		 * This method checks if a move is valid. It will take into consideration the board and
		 * the game_information object.
		 *
		 * THIS WILL MAKE THE MOVE ON THE BOARD!!! That is needed to check for legality. If the move turns out to
		 * be illegal, the move must be unmaked.
		 */
		move_legality_result isValidMove(const move& playerMove, std::list<move>& pseudoLegalMoves) {
			bool legal = false;
			std::string information;
			int capturedPiece;
			unsigned int side = info.getSideToMove();
			unsigned int enemySide = 1 - side;
			for(const move& plMove: pseudoLegalMoves) { //check if move is at least pseudo legal
				if(playerMove == plMove) {
					//move is pseudo legal
					capturedPiece = board.makeMove(playerMove, side); //make this move on the board
					if(playerMove.isKingsideCastle()) {
						if(info.getKingsideCastleRights(side)) {
							//for castle, not only king safety needs to be checked, but also the king cant move through attacked squares
							if(!isAttacked(board, enemySide, playerMove.getFromSquare()) && // <-- check that king did not castle out of check
							   !isAttacked(board, enemySide, playerMove.getFromSquare()+1) && // <-- check that king did not castle through check
							   !isAttacked(board, enemySide, playerMove.getFromSquare()+2)) { // <-- check that king did not castle into check
								//the king is safe and did not catle out/through attacked squares
								legal = true;
							} else {
								information = "Can't castle out of/through check!";
							}
						} else { //castle rights lost
							information = "Don't have kingside castle rights anymore!";
						}
						break;
					} else if(playerMove.isQueensideCastle()) {
						//for castle, not only king safety needs to be checked, but also the king cant move through attacked squares
						if(info.getQueensideCastleRights(side)) {
							if(!isAttacked(board, enemySide, playerMove.getFromSquare()) && // <-- check that king did not castle out of check
								   !isAttacked(board, enemySide, playerMove.getFromSquare()-1) && // <-- check that king did not castle through check
								   !isAttacked(board, enemySide, playerMove.getFromSquare()-2)) { // <-- check that king did not castle into check
									//the king is safe and did not catle out/through attacked squares
									legal = true;
								} else {
									information = "Can't castle out of/through check!";
								}
						} else { //castle rights lost
							information = "Don't have queenside castle rights anymore!";
						}
						break;
					} else {
						//not a castle, only king safety is important
						if(!isAttacked(board, enemySide, board.getKingSquare(side))) {
							//the king is safe after this move
							legal = true;
						} else {
							information = "This move leave the king in check!";
						}
						break;
					}
				}
			}
			return move_legality_result(legal, information, capturedPiece);
		}

		/*
		 * Called when the game has ended. Prints information about the ending.
		 */
		void endGame(bool draw, const std::string& winninSide, const std::string& message) {
			if(draw) {
				std::cout << "The game has ended in a draw!" << std::endl;
			} else {
				std::cout << winninSide << " has won the game!" << std::endl;
			}
			std::cout << "Reason: " << message << std::endl;
			std::cout << "-------------------------------------" << std::endl;
			std::cout << "Do you want to start a new game? (y = yes/anything else = no)" << std::endl;
			std::string input;
			std::getline(std::cin, input);
			if(input == "y") {
				tchess::selectPlayersAndStart();
			}
		}
	};
}

#endif /* SRC_GAME_GAME_H_ */
