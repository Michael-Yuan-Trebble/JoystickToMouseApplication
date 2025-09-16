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

class MainWindow : public QMainWindow 
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	void updateJoysticks();

signals:
	void start();

private:
	std::vector<QLabel*> labels;
	QPushButton* startBtn;
	QLabel* allJoysticks;
	QWidget* central = new QWidget(this);;
	QVBoxLayout* layout = new QVBoxLayout(central);;
};