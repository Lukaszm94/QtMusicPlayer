#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <QProgressBar>

class ProgressBar : public QProgressBar {

	Q_OBJECT

public:
	ProgressBar(QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *event);
	//void paintEvent(QPaintEvent *event);
signals:
	void pressedValue(qint64 valuePressed);
};

#endif // PROGRESSBAR_H
