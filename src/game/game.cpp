/*
 * game.cpp
 *
 *  Created on: 2021. jan. 30.
 *      Author: Gáspár Tamás
 */

#include "game.h"
#include "human/human_player.h"
#include "random/random_player.h"
#include "greedy/greedy_player.h"

namespace tchess
{
	char selectPlayerForSide(const std::string& sideName) {
		std::cout << "---------------------------" << std::endl
				  << "Select who will play " << sideName << ":" << std::endl
				  << " - Type p for player!" << std::endl
				  << " - Type e for engine!" << std::endl
				  << " - Type r for the random move maker!" << std::endl
				  << " - Type g for the greedy move maker!" << std::endl;
		std::string selection;
		std::getline(std::cin, selection);
		if(selection == "p") {
			std::cout << sideName << " will be controlled by a player." << std::endl;
		} else if(selection == "e") {
			std::cout << sideName << " will be controlled by the TChess engine." << std::endl;
		} else if(selection == "r") {
			std::cout << sideName << " will be controlled by the Random move maker." << std::endl;
		} else if(selection == "g") {
			std::cout << sideName << " will be controlled by the Greedy move maker." << std::endl;
		}
		else {
			std::cout << "Invalid selection, please try again!" << std::endl;
			selectPlayerForSide(sideName);
		}
		return selection[0];
	}

	void selectPlayersAndStart() {
		char whiteSelect = selectPlayerForSide("White");
		char blackSelect = selectPlayerForSide("Black");
		player* whitePlayer;
		player* blackPlayer;
		if(whiteSelect == 'p') {
			whitePlayer = new human_player_console(white);
		} else if(whiteSelect == 'e') {
			//TODO
		} else if(whiteSelect == 'r') {
			whitePlayer = new random_player(white);
		} else if(whiteSelect == 'g') {
			whitePlayer = new greedy_player(white);
		}
		if(blackSelect == 'p') {
			blackPlayer = new human_player_console(black);
		} else if(blackSelect == 'e') {
			//TODO
		} else if(blackSelect == 'r') {
			blackPlayer = new random_player(black);
		} else if(blackSelect == 'g') {
			blackPlayer = new greedy_player(black);
		}
		game gameController(whitePlayer, blackPlayer);
		gameController.beginGame();
		delete whitePlayer;
		delete blackPlayer;
	}

	//game class implementation

	void game::beginGame() {
		std::cout << "The game begins: " << whitePlayer->description() << " vs " << blackPlayer->description() << std::endl;
		std::cout << board.to_string();
		std::string playerWhoMoves = info.getSideToMove()==white ? whitePlayer->description() : blackPlayer->description();
		std::cout << playerWhoMoves << "'s turn to move..." << std::endl;
		whitePlayer->makeMove(*this); //white player will call 'submitMove'.
	}

	void game::submitMove(const move& m) {
		unsigned int side = info.getSideToMove();
		std::string playerWhoMoved = side==white ? whitePlayer->description() : blackPlayer->description();

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
			std::cout << "Move " << moves.size() << ": " << playerWhoMoved <<
					" has made the move: " << m.to_string(pieceThatMoved);
			if(checkmate) {
				std::cout << " (checkmate)" << std::endl;
				endGame(false, playerWhoMoved, "Checkmate");
				return;
			} else if(stalemate) {
				std::cout << " (stalemate)" << std::endl;
				endGame(true, "", "Stalemate");
				return;
			} else if(board.isInsufficientMaterial()) {
				std::cout << std::endl;
				endGame(true, "", "Insufficient mating material");
				return;
			} else if(check) {
				std::cout << " (check)";
			}
			std::cout << std::endl;
			std::cout << board.to_string();

			side = info.getSideToMove();
			std::string playerWhoMoves= side==white ? whitePlayer->description() : blackPlayer->description();
			std::cout << playerWhoMoves << "'s turn to move..." << std::endl;
			//ask for the next move
			side == white ? whitePlayer->makeMove(*this) : blackPlayer->makeMove(*this);
		} else { //move is invalid
			std::cout << playerWhoMoved << " has made an illegal move: " << m.to_string(pieceThatMoved) << ", " << result.getInformation() << std::endl;
			board.unmakeMove(m, side, result.getCapturedPiece()); //unmakde the illegal move on the board
			side == white ? whitePlayer->makeMove(*this) : blackPlayer->makeMove(*this); //ask for a new move
		}
	}

	const std::list<move>& game::getMoves() const {
		return moves;
	}

	move_legality_result game::isValidMove(const move& playerMove, std::list<move>& pseudoLegalMoves) {
		bool legal = false;
		std::string information;
		int capturedPiece = 0;
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

	void game::endGame(bool draw, const std::string& winninSide, const std::string& message) {
		if(draw) {
			std::cout << "The game has ended in a draw!" << std::endl;
		} else {
			std::cout << winninSide << " has won the game!" << std::endl;
		}
		std::cout << "Reason: " << message << std::endl
				<< "The board at the end of the game:\n" << board.to_string();
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "Do you want to start a new game? (y = yes/anything else = no)" << std::endl;
		std::string input;
		std::getline(std::cin, input);
		if(input == "y") {
			tchess::selectPlayersAndStart();
		}
	}
}




