#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QtDebug>
#include "game_board.h"
#include <QMessageBox>

namespace Ui {
	class Dialog;
}

class Dialog : public QDialog {
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();
	void quitGame();

private:
	Ui::Dialog *ui;
	void keyPressEvent(QKeyEvent *event);
	void check_loss();

protected:
	void paintEvent(QPaintEvent *e);
};

#endif // DIALOG_H