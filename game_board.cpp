#include "dialog.h"
#include "ui_dialog.h"
#include "game_board.h"

Game_Board mainBoard;

Game_Board::Game_Board() {
	for (int i = 0; i < GAME_BOARD_SIZE_X; ++i) {
		for (int j = 0; j < GAME_BOARD_SIZE_Y; ++j) {
			mainBoard.board[i][j] = false;
		}
	}
	srand(time(NULL));
}

void Game_Board::begin_game() {
	mainBoard.new_piece(mainBoard.random_num(6));
}

//Used to initialize a new peice based on the perameter piece type (0-5)
void Game_Board::new_piece(int piece_type) {
	ap.squares[0].x = GAME_BOARD_SIZE_X / 2;
	ap.squares[0].y = GAME_BOARD_SIZE_Y - 2;
	switch (piece_type) {
	case 0:
		// add left l
		assign_coords(ap.squares[0].x - 1, ap.squares[0].y, ap.squares[0].x + 1, ap.squares[0].y,
			ap.squares[0].x + 1, ap.squares[0].y + 1);
		break;
	case 1:
		// add right l
		assign_coords(ap.squares[0].x + 1, ap.squares[0].y, ap.squares[0].x - 1, ap.squares[0].y,
			ap.squares[0].x - 1, ap.squares[0].y + 1);
		break;
	case 2:
		// add left zig
		assign_coords(ap.squares[0].x + 1, ap.squares[0].y, ap.squares[0].x, ap.squares[0].y + 1,
			ap.squares[0].x - 1, ap.squares[0].y + 1);
		break;
	case 3:
		// add right zig
		assign_coords(ap.squares[0].x - 1, ap.squares[0].y, ap.squares[0].x, ap.squares[0].y + 1,
			ap.squares[0].x + 1, ap.squares[0].y + 1);
		break;
	case 4:
		// add square
		assign_coords(ap.squares[0].x + 1, ap.squares[0].y, ap.squares[0].x + 1, ap.squares[0].y + 1,
			ap.squares[0].x, ap.squares[0].y + 1);
		break;
	case 5:
		// add t
		assign_coords(ap.squares[0].x + 1, ap.squares[0].y, ap.squares[0].x - 1, ap.squares[0].y,
			ap.squares[0].x, ap.squares[0].y + 1);
		break;
	default:
		// add straight
		//qDebug() << "Straight";
		assign_coords(ap.squares[0].x - 1, ap.squares[0].y, ap.squares[0].x + 1, ap.squares[0].y,
			ap.squares[0].x - 2, ap.squares[0].y);
		break;
	}
}

//Sets up the quardinates for the active peice
void Game_Board::assign_coords(int one_x, int one_y, int two_x, int two_y,
	int three_x, int three_y) {
	ap.squares[1].x = one_x;
	ap.squares[1].y = one_y;
	ap.squares[2].x = two_x;
	ap.squares[2].y = two_y;
	ap.squares[3].x = three_x;
	ap.squares[3].y = three_y;
}

//checks for a full row using the values in row_descriptor
void Game_Board::clear_rows() {
	for (int j = 0; j < GAME_BOARD_SIZE_Y; ++j) {
		if (row_descriptor[j] >= GAME_BOARD_SIZE_X) {
			for (int i = j + 1; i < GAME_BOARD_SIZE_Y; ++i) {
				for (int x = 0; x < GAME_BOARD_SIZE_X; ++x) {
					board[x][i - 1] = board[x][i];
				}
				row_descriptor[i - 1] = row_descriptor[i];
			}
		}
	}
}


//moves the piece down everytime the screan is redrawn
//it also checks for intersection, if intersection exists,
//it adds the peice to the board.
void Game_Board::drop() {
	if (check_drop_intersect()) {
		for (int i = 0; i < 4; ++i) {
			row_descriptor[ap.squares[i].y]++;
		}
		add_piece();
		new_piece(random_num(6));
		return;
	}
	for (int i = 0; i < 4; ++i) {
		ap.squares[i].y--;
	}
}

//rotates the active peice right
void Game_Board::rotate_right() {
	if (!check_anticlock_rot_intersect()) {
		for (int i = 0; i < 4; ++i) {
			int relx = ap.squares[i].x - ap.squares[0].x;
			int rely = ap.squares[i].y - ap.squares[0].y;
			ap.squares[i].x = -rely + ap.squares[0].x;
			ap.squares[i].y = relx + ap.squares[0].y;
		}
	}
}
//rotates the active peice LEFT
void Game_Board::rotate_left() {
	if (!check_clock_rot_intersect()) {
		for (int i = 0; i < 4; ++i) {
			ap.squares[i].x = (ap.squares[i].y - ap.squares[0].y) + ap.squares[0].x;
			ap.squares[i].y = -(ap.squares[i].x - ap.squares[0].x) + ap.squares[0].y;
		}
	}
}

//funtion to check if the active piece intersects with any peices below it
bool Game_Board::check_drop_intersect() {
	for (int i = 0; i < 4; ++i) {
		if (check_intersect(ap.squares[i].x, ap.squares[i].y - 1)) {
			return true;
		}
	}
	return false;
}

//funtion to ensure that a player can't move a piece horizontally into another
bool Game_Board::check_horz_intersect(int delta) {
	for (int i = 0; i < 4; ++i) {
		if (check_intersect(ap.squares[i].x + delta, ap.squares[i].y)) {
			return true;
		}
	}
	return false;
}

//intersection checker relied on by check_horz_intersect and check_drop_intersect
bool Game_Board::check_intersect(int x, int y) {
	if (x <= -1 || x == GAME_BOARD_SIZE_X || y <= -1 || y == GAME_BOARD_SIZE_Y || board[x][y]) {
		return true;
	}
	return false;
}


//ensures the piece doesnt hit anything when rotating clockwise
bool Game_Board::check_clock_rot_intersect() {
	for (int i = 0; i < 4; ++i) {
		int rot_x = -(ap.squares[i].y - ap.squares[0].y) + ap.squares[0].x;
		int rot_y = (ap.squares[i].x - ap.squares[0].x) + ap.squares[0].y;
		if (check_intersect(rot_x, rot_y)) {
			return true;
		}
	}
	return false;
}

//ensures the piece doesnt hit anything when rotating counter-clockwise
bool Game_Board::check_anticlock_rot_intersect() {
	for (int i = 0; i < 4; ++i) {
		int rot_x = (ap.squares[i].y - ap.squares[0].y) + ap.squares[0].x;
		int rot_y = -(ap.squares[i].x - ap.squares[0].x) + ap.squares[0].y;
		if (check_intersect(rot_x, rot_y)) {
			return true;
		}
	}
	return false;
}


void Game_Board::quick_drop() {
	while (!check_drop_intersect()) {
		drop();
	}
	add_piece();
	for (int i = 0; i < 4; ++i) {
		row_descriptor[ap.squares[i].y]++;
	}
	new_piece(random_num(6));
}

void Game_Board::add_piece() {
	for (int i = 0; i < 4; ++i) {
		board[ap.squares[i].x][ap.squares[i].y] = true;
	}
	clear_rows();
}

//returns a random number mod max
int Game_Board::random_num(int max) {
	return rand() % (max + 1);
}

//moves piece left
void Game_Board::on_left_arrow() {
	if (!check_horz_intersect(-1)) {
		for (int i = 0; i < 4; ++i) {
			ap.squares[i].x--;
		}
	}
}

//moves piece right
void Game_Board::on_right_arrow() {
	if (!check_horz_intersect(1)) {
		for (int i = 0; i < 4; ++i) {
			ap.squares[i].x++;
		}
	}
}

//rotates piece left
void Game_Board::on_up_arrow() {
	rotate_left();
}

//drops piece with the press of the space bar
void Game_Board::on_space_bar() {
	quick_drop();
}

//rotates piece right
void Game_Board::on_down_arrow() {
	rotate_right();
}

//HELPER FUNCTION
//Funtion used to print the board for debugging
void Game_Board::printBoard() {
	for (int i = 0; i < GAME_BOARD_SIZE_Y; i++) {
		std::cout << row_descriptor[i] << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < GAME_BOARD_SIZE_Y; ++i) {
		for (int j = 0; j < GAME_BOARD_SIZE_X; ++j) {
			if (board[j][i] || isPiece(j, i)) {
				std::cout << 1;
			}
			else {
				std::cout << 0;
			}
		}
		std::cout << std::endl;
	}
	for (int x = 0; x < 4; x++) {
		std::cout << "X: " << ap.squares[x].x << " Y: " << ap.squares[x].y << ", ";
	}
	std::cout << "\n\n\n\n\n";
}

//used for printing board when debugging to show the active
//piece that isn't on the board
bool Game_Board::isPiece(int i, int j) {
	for (int x = 0; x < 4; x++) {
		if (ap.squares[x].x == i && ap.squares[x].y == j) {
			return true;
		}
	}
	return false;
}