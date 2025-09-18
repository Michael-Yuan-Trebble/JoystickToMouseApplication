#include "SettingsWindow.h"
#include <qpushbutton.h>
#include <qfile.h>
#include <qcoreapplication.h>

SettingsWindow::SettingsWindow(QWidget* parent) : QMainWindow(parent) 
{
	loadSettings();

	QWidget* central = new QWidget(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(central);
	QVBoxLayout* firstSetting = new QVBoxLayout();
	QVBoxLayout* secondSetting = new QVBoxLayout();

	QJsonObject keybinds = obj["keybinds"].toObject();

	QLabel* leftLabel = new QLabel("Left Click", this);
	leftButton = new QPushButton(QString::number(keybinds["left_click"].toInt()), this);
	connect(leftButton, &QPushButton::clicked, this, &SettingsWindow::changeLeft);

	firstSetting->addWidget(leftLabel);
	firstSetting->addWidget(leftButton);

	QLabel* rightLabel = new QLabel("Right Click", this);
	rightButton = new QPushButton(QString::number(keybinds["right_click"].toInt()), this);
	connect(rightButton, &QPushButton::clicked, this, &SettingsWindow::changeRight);


	secondSetting->addWidget(rightLabel);
	secondSetting->addWidget(rightButton);

	mainLayout->addLayout(firstSetting);
	mainLayout->addLayout(secondSetting);
	setCentralWidget(central);

}

void SettingsWindow::loadSettings() 
{
	QString exeDir = QCoreApplication::applicationDirPath();
	QString configDir = exeDir + "/Settings";
	QString Filepath = configDir + "/settings.json";
	QFile file(Filepath);
	if (!file.open(QIODevice::ReadOnly)) return;
	QByteArray data = file.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(data);
	obj = doc.object();
}

void SettingsWindow::changeLeft() 
{
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [this, timer]() {
		SDL_Event event;
		QJsonObject keybinds = obj["keybinds"].toObject();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN) {
				keybinds["left_click"] = event.jbutton.button;
				obj["keybinds"] = keybinds;
				leftButton->setText(QString::number(event.jbutton.button));
				saveKeybind();
				timer->stop();
				timer->deleteLater();
				return;
			}
		}
		});
	timer->start(5);
}

void SettingsWindow::changeRight() 
{
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [this, timer]() {
		SDL_Event event;
		QJsonObject keybinds = obj["keybinds"].toObject();
		while (SDL_PollEvent(&event))
		{
			if(event.type==SDL_EVENT_JOYSTICK_BUTTON_DOWN){
				keybinds["right_click"] = event.jbutton.button;
				obj["keybinds"] = keybinds;
				rightButton->setText(QString::number(event.jbutton.button));
				saveKeybind();
				timer->stop();
				timer->deleteLater();
				return;
			}
		}
	});
	timer->start(5);
}

void SettingsWindow::saveKeybind() {
	QString exeDir = QCoreApplication::applicationDirPath();
	QString configDir = exeDir + "/Settings";
	QString Filepath = configDir + "/settings.json";
	QFile file(Filepath);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
	file.close();
}