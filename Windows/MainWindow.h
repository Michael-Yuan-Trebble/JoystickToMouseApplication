#pragma once

#include <qmainwindow.h>
#include <qobject.h>
#include <qtimer.h>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include <qboxlayout.h>

class QPushButton;
class QLabel;
class QWidget;
class QComboBox;

class MainWindow : public QMainWindow 
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	void updateJoysticks();

signals:
	void start(SDL_Joystick* joystick);

private:
	std::vector<QLabel*> labels;
	QPushButton* startBtn;
	QComboBox* combo;
	QWidget* central = new QWidget(this);;
	QVBoxLayout* layout = new QVBoxLayout(central);;
};