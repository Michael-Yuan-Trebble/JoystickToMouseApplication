#pragma once

#include <qobject.h>
#include <qwidget.h>
#include <qpainter.h>

class JoystickVisualizer : public QWidget {
	Q_OBJECT
public:
	explicit JoystickVisualizer(QWidget* parent = nullptr) : QWidget(parent), pos(0, 0) {}

	void setPosition(float x, float y) {
		pos = QPointF(x, y);
		update();
	}

protected:
	void paintEvent(QPaintEvent*) override {
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);

		p.setPen(Qt::white);
		p.setBrush(Qt::darkGray);
		int side = qMin(width(), height());   
		QRectF circleRect(
			(width() - side) / 2, 
			(height() - side) / 2,
			side, side  
		);
		p.drawEllipse(circleRect);

		QPointF center(width() / 2.0, height() / 2.0);
		QPointF stickPos = center + QPointF(pos.x() * (width() / 2 - 10),
			pos.y() * (height() / 2 - 10));
		p.setBrush(Qt::red);
		p.drawEllipse(stickPos, 5, 5);

	}

private:
	QPointF pos;
};