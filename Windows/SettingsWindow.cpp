#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget* parent) : QMainWindow(parent) 
{
	QWidget* central = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(central);
	QLabel* label = new QLabel("TEST",this);
	layout->addWidget(label);
}