#pragma once

#include <qmainwindow.h>
#include <qwidget.h>
#include <qboxlayout.h>
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <memory>
#include "SDL3/SDL.h"
#include <qtimer.h>
#include <iostream>
#include "../Widgets/JoystickVisualizer.h"

class QPushButton;
class QLabel;

class ActivateWindow : public QMainWindow 
{
	Q_OBJECT
public:
	explicit ActivateWindow(QWidget* parent = nullptr);
	void loadSettings();

	Uint8 Left_Click;
	Uint8 Right_Click;

signals:
	void goBack();
	void goSettings();
private:

	// ------------------
	// Qt Variables
	// ------------------
	
	QPushButton* push;
	QPushButton* settingsbtn;
	QLabel* ButtonPressed;
	QLabel* LeftClickLabel;
	QLabel* RightClickLabel;

	QString Filepath;
	QJsonObject obj;
	// ------------------
	// SDL Variables
	// ------------------

	SDL_Joystick* ControlledJoystick;

	// ------------------
	// Standard Variables
	// ------------------

	int lastButtonIndex = -1;
	int DEAD_ZONE;
	double SENSITIVITY;
	double SCROLL_SENSITIVITY;

	JoystickVisualizer* Visual;

	// ------------------
	// Functions
	// ------------------

	void setJoystick(int index, SDL_JoystickID* joysticks);
	void setMovement();
	void checkSettings();
	void initSettings();
	void changeCircle(int x, int y);

};