#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "game_board.h"

#define GAME_BOARD_SIZE_X 10
#define GAME_BOARD_SIZE_Y 20
#define NUM_BLOCKS 4

class GameBoard;

// A piece of the active piece
struct Square {
	int x;
	int y;
};
typedef Square Square;

// The current piece which is falling
// Square 0 will be the axis of rotation
struct Active_Piece {
	Square squares[NUM_BLOCKS];
};
typedef Active_Piece Active_Piece;

class Game_Board {
	friend class Game_Board_Tester;

private:
	// Tracks the number of filled squares in a row
	int row_descriptor[GAME_BOARD_SIZE_Y];

	// creates a new piece which becomes the active piece
	void new_piece(int piece_type);

	// removes squares in rows which have become full
	void clear_rows();

	// rotates the coordinates left
	void rotate_left();

	// checks if the square is intersecting another square
	bool check_drop_intersect();

	// checks if the active piece is able to more horizontally
	// @param delta the direction the piece will move
	bool check_horz_intersect(int delta);

	// Checks it the coordinates passed in are out of bounds or intersect anything
	bool check_intersect(int x, int y);

	// Checks if the piece is able to be rotated anticlockwise
	bool check_anticlock_rot_intersect();

	// Checks if the piece is able to be rotated clockwise
	bool check_clock_rot_intersect();

	//drops the piece as far as it is able to go
	void quick_drop();

	// Adds the active piece to the game board
	void add_piece();

	// Assigns the specified coordinates to corresdoning square in ap
	void assign_coords(int one_x, int one_y, int two_x, int two_y,
		int three_x, int three_y);

	// produces a random number for piece creation
	int random_num(int max);

public:
	// a struct containing 4 squares which represents the actively falling piece
	Active_Piece ap;

	// a 2d bool array to represent spots taken up by a square
	bool board[GAME_BOARD_SIZE_X][GAME_BOARD_SIZE_Y];

	static void begin_game();

	void rotate_right();

	void drop();

	bool isPiece(int i, int j);

	void printBoard();

	Game_Board();

	void on_left_arrow();

	void on_right_arrow();

	void on_up_arrow();

	void on_space_bar();

	void on_down_arrow();
};

#endif