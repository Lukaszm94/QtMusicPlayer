#include <QtGui>
#include "ticker.h"




Ticker::Ticker(QWidget *parent) : QWidget(parent)
{
	offset = 0;
	myTimerId = 0;
	QFont font;
	font.setPointSize(10);
	font.setFamily("Verdana");
	font.setBold(true);
	setFont(font);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void Ticker::setText(const QString &newText)
{
	myText = newText;
	update();
	updateGeometry();
}

QSize Ticker::sizeHint() const
{
	return fontMetrics().size(0, text());
}

QSize Ticker::minimumSizeHint()
{
	return sizeHint();
}

void Ticker::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	int textWidth = fontMetrics().width(text());
	if(textWidth < 1)
		return;
	int x = -offset;
	while(x < width()) {
		painter.drawText(x, 0, textWidth, height(), Qt::AlignLeft | Qt::AlignVCenter, text());
		x += textWidth;
	}
}

void Ticker::timerEvent(QTimerEvent *event)
{
	if(event->timerId() == myTimerId) {
		offset++;
		if(offset >= fontMetrics().width(text()))
			offset = 0;
		scroll(-1, 0);
	} else {
		QWidget::timerEvent(event);
	}
}

void Ticker::showEvent(QShowEvent *)
{
	myTimerId = startTimer(50);
}

void Ticker::hideEvent(QHideEvent *)
{
	killTimer(myTimerId);
	myTimerId = 0;
}
