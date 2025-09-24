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

    float deadzone = 0.f;

    void setDeadzone(float radius)
    {
        deadzone = radius;
        update();
    }

protected:
	void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int side = qMin(width(), height());
        QRectF circleRect(
            (width() - side) / 2.0,
            (height() - side) / 2.0,
            side, side
        );

        p.setPen(Qt::white);
        p.setBrush(Qt::darkGray);
        p.drawEllipse(circleRect);

        QPointF center = circleRect.center();
        const float redRadius = 5.0f;
        const float margin = 2.0f;
        float outerRadius = side / 2.0f;
        float usableRadius = outerRadius - redRadius - margin; 

        float deadzoneRadius = deadzone * usableRadius;
        p.setPen(QPen(Qt::black, 2, Qt::DashLine));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(center, deadzoneRadius, deadzoneRadius);

        QPointF stickPos = center + QPointF(pos.x() * usableRadius, pos.y() * usableRadius);


        p.setBrush(Qt::red);
        p.setPen(Qt::NoPen);
        p.drawEllipse(stickPos, redRadius, redRadius);

	}

private:
	QPointF pos;
};