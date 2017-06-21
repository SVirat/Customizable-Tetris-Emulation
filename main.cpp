#include "dialog.h"
#include "game_board.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog w;
	w.show();
	return a.exec();
}
