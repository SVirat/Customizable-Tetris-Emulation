#include "dialog.h"
#include "ui_dialog.h"
#include "game_board.h"
#include <QtDebug>
#include <QKeyEvent>
#include <QMessageBox>

extern Game_Board mainBoard;

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
	ui->setupUi(this);
}

Dialog::~Dialog(){
	delete ui;
}

//used to interperet key presses from keyboard
void Dialog::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_D) {
		//std::cout<<"KEY D\n";
		mainBoard.on_right_arrow();
	}
	else if (event->key() == Qt::Key_A) {
		mainBoard.on_left_arrow();
	}
	else if (event->key() == Qt::Key_W) {
		//qDebug()<<"Shift Pressed";
		mainBoard.rotate_right();
	}
	else if (event->key() == Qt::Key_S) {
		//qDebug()<<"Shift Pressed";
		mainBoard.rotate_right();
	}
	else if (event->key() == Qt::Key_Space) {
		mainBoard.on_space_bar();
	}
}


void Dialog::check_loss() {
	for (int i = 0; i < GAME_BOARD_SIZE_X; ++i) {
		if (mainBoard.board[i][GAME_BOARD_SIZE_Y - 1]) {
			quitGame();
		}
	}
}

//Main funtion that draws everything to the screen
void Dialog::paintEvent(QPaintEvent *e) {
	QPainter painter(this);

	//draws all the pieces on the board
	for (int i = 0; i < GAME_BOARD_SIZE_X; ++i) {
		for (int j = 0; j < GAME_BOARD_SIZE_Y; ++j) {
			if (mainBoard.board[i][j]) {
				QRect square(i * 20, 380 - j * 20, 20, 20);
				painter.fillRect(square, Qt::red);
				painter.drawRect(square);
			}
		}
	}

	//draws the active piece
	for (int i = 0; i < NUM_BLOCKS; i++) {
		QRect active(mainBoard.ap.squares[i].x * 20, 380 - mainBoard.ap.squares[i].y * 20, 20, 20);
		painter.fillRect(active, Qt::red);
		painter.drawRect(active);
	}

	mainBoard.drop();
	check_loss();
	QThread::msleep(200);
	update();
}

//creates dialog on game loss
void Dialog::quitGame() {
	QMessageBox msgBox;
	msgBox.setText("Game Over.");
	msgBox.setInformativeText("Would you like to play again?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Yes) {
		//clears board to start a new game
		for (int i = 0; i < GAME_BOARD_SIZE_X; ++i) {
			for (int j = 0; j < GAME_BOARD_SIZE_Y; ++j) {
				mainBoard.board[i][j] = false;
			}
		}
		Game_Board::begin_game();
		return;
	}
	close();
}