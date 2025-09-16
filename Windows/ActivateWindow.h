#pragma once

#include <qmainwindow.h>
#include <qwidget.h>
#include <qboxlayout.h>
#include <qobject.h>
#include <memory>
#include "SDL3/SDL.h"
#include <qtimer.h>
#include <iostream>

class QPushButton;
class QLabel;

class ActivateWindow : public QMainWindow 
{
	Q_OBJECT
public:
	explicit ActivateWindow(QWidget* parent = nullptr);

signals:
	void goBack();
private:
	QPushButton* push;
	QLabel* ButtonPressed;
	int lastButtonIndex = -1;
	SDL_Joystick* ControlledJoystick;
	const int DEAD_ZONE = 6000;
	const float SENSITIVITY = 0.0005f;
	const float SCROLL_SENSITIVITY = 0.05f;
	void setJoystick(int index, SDL_JoystickID* joysticks);
	void setMovement();
};