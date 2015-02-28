#include "progressbar.h"
#include <QMouseEvent>



ProgressBar::ProgressBar(QWidget *parent) : QProgressBar(parent)
{

}

void ProgressBar::mousePressEvent(QMouseEvent *event)
{
	qint64 xSize = this->size().width();
	qint64 clickedXPoint = event->x();
	qint64 newValue = (clickedXPoint*this->maximum())/(xSize-4);
	emit pressedValue(newValue);
}
