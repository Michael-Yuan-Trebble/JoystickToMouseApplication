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
	
	void setJoystick(SDL_Joystick* joystick);

	Uint8 Left_Click;
	Uint8 Right_Click;
	Uint8 DPI_Up;
	Uint8 DPI_Down;

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
	QLabel* DPIUpLabel;
	QLabel* DPIDownLabel;

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
	double DPI_ADDON;
	double DPI_INCREASE;

	JoystickVisualizer* Visual;

	// ------------------
	// Functions
	// ------------------
	void setMovement();
	void checkSettings();
	void initSettings();
	void changeCircle(int x, int y);

};