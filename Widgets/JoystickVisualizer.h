#pragma once

#include <qobject.h>
#include <qwidget.h>
#include <qpainter.h>

class JoystickVisualizer : public QWidget {
	Q_OBJECT
public:
	explicit JoystickVisualizer(QWidget* parent = nullptr) : QWidget(parent), pos(0, 0) {}

	void setPosition(float x, float y) {
		float nx = std::clamp(x, -1.0f, 1.0f);
		float ny = std::clamp(y, -1.0f, 1.0f);

		float d = std::hypot(nx, ny);
		if (d > 1.0f) { nx /= d; ny /= d; }

		pos = QPointF(nx, ny);
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
            (width() - side) / 2.0,
            (height() - side) / 2.0,
            side, side
        );
        p.drawEllipse(circleRect);

        QPointF center = circleRect.center();
        const float redRadius = 5.0f;
        const float margin = 2.0f;
        float outerRadius = side / 2.0f;
        float usableRadius = outerRadius - redRadius - margin; 

        // pos is normalized [-1..1] so multiply by usableRadius
        QPointF stickPos = center + QPointF(pos.x() * usableRadius, pos.y() * usableRadius);

        p.setBrush(Qt::red);
        p.drawEllipse(stickPos, redRadius, redRadius);

	}

private:
	QPointF pos;
};