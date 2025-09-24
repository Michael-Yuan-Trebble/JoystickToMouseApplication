#include "ActivateWindow.h"
#include "Windows.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qdir.h>
#include <qcoreapplication.h>
#include <qstandardpaths.h>

ActivateWindow::ActivateWindow(QWidget* parent) : QMainWindow(parent)
{
	QString exeDir = QCoreApplication::applicationDirPath();
	QString configDir = exeDir + "/Settings";
	QDir().mkpath(configDir);
	Filepath = configDir + "/settings.json";

	this->resize(800, 800);
	QWidget* central = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(central);
	QHBoxLayout* Clicks = new QHBoxLayout();
	QHBoxLayout* DPI = new QHBoxLayout();
	ButtonPressed = new QLabel("Button Pressed: ", this);
	layout->addWidget(ButtonPressed);

	Visual = new JoystickVisualizer(this);

	checkSettings();
	loadSettings();

	Clicks->addWidget(LeftClickLabel);
	Clicks->addWidget(RightClickLabel);
	layout->addLayout(Clicks);

	DPI->addWidget(DPIUpLabel);
	DPI->addWidget(DPIDownLabel);
	layout->addLayout(DPI);

	layout->addWidget(Visual);

	push = new QPushButton("Go Back", this);
	connect(push, &QPushButton::clicked, this, &ActivateWindow::goBack);
	layout->addWidget(push);

	settingsbtn = new QPushButton("Settings", this);
	connect(settingsbtn, &QPushButton::clicked, this, &ActivateWindow::goSettings);
	layout->addWidget(settingsbtn);

	setCentralWidget(central);
	setWindowTitle("Active");
}

// ------------------
// JSON Functions
// ------------------


void ActivateWindow::checkSettings()
{
	QDir().mkpath(QFileInfo(Filepath).absolutePath());
	QFile file(Filepath);
	if (!file.exists())
	{
		QJsonObject defaults;
		defaults["sensitivity"] = 0.0005;
		defaults["dpi_sensitivity"] = 0.0001;
		defaults["dead_zone"] = 6000;
		defaults["scroll_sensitivity"] = 0.05;

		QJsonObject keybinds;
		keybinds["left_click"] = 0;
		keybinds["right_click"] = 1;
		keybinds["DPI_up"] = 2;
		keybinds["DPI_down"] = 3;
		defaults["keybinds"] = keybinds;

		QJsonDocument doc(defaults);
		if (file.open(QIODevice::WriteOnly)) {
			file.write(doc.toJson(QJsonDocument::Indented));
			file.close();
		}
	}
}

void ActivateWindow::initSettings()
{
	SENSITIVITY = obj["sensitivity"].toDouble();
	DEAD_ZONE = obj["dead_zone"].toInt();

	Visual->setDeadzone(0.3);
	SCROLL_SENSITIVITY = obj["scroll_sensitivity"].toDouble();
	DPI_ADDON = 0.f;
	DPI_INCREASE = obj["dpi_sensitivity"].toDouble();
	QJsonObject keybinds = obj["keybinds"].toObject();
	Left_Click = keybinds["left_click"].toInt();
	Right_Click = keybinds["right_click"].toInt();
	DPI_Up = keybinds["DPI_up"].toInt();
	DPI_Down = keybinds["DPI_down"].toInt();

	if (LeftClickLabel || RightClickLabel) 
	{
		LeftClickLabel = new QLabel("Left Click Button: " + QString::number(Left_Click), this);
		RightClickLabel = new QLabel("Right Click Button: " + QString::number(Right_Click), this);
	}
	else 
	{
		LeftClickLabel->setText("Left Click Button: " + QString::number(Left_Click));
		RightClickLabel->setText("Right Click Button: " + QString::number(Right_Click));
	}
	if (DPIDownLabel || DPIUpLabel) 
	{
		DPIUpLabel = new QLabel("DPI Up Button: " + QString::number(DPI_Up), this);
		DPIDownLabel = new QLabel("DPI Down Button: " + QString::number(DPI_Down), this);
	}
	else 
	{
		DPIUpLabel->setText("DPI Up Button: " + QString::number(DPI_Up));
		DPIDownLabel->setText("DPI Down Button: " + QString::number(DPI_Down));
	}
}

void ActivateWindow::loadSettings()
{
	QFile file(Filepath);
	if (!file.open(QIODevice::ReadOnly)) return;
	QByteArray data = file.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(data);
	obj = doc.object();
	initSettings();
}

void ActivateWindow::setJoystick(SDL_Joystick* joystick)
{
	ControlledJoystick = joystick;
	if (ControlledJoystick) 
	{
		QTimer* timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &ActivateWindow::setMovement);
		timer->start(10);
	}
	else 
	{
		qDebug() << "Activation Window could not set Joystick";
	}

}

void ActivateWindow::changeCircle(int x, int y) 
{
	float normX = static_cast<float>(x) / 32767.0f;
	float normY = static_cast<float>(y) / 32767.0f;
	Visual->setPosition(normX, normY);
}

// ------------------
// Apply joystick actions to mouse
// ------------------

void ActivateWindow::setMovement() 
{
	if (!ControlledJoystick) return;
	SDL_Event event;
	static int hatState = SDL_HAT_CENTERED;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
			if (event.jbutton.button == Left_Click)
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			if (event.jbutton.button == Right_Click)
				mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
			if (event.jbutton.button == DPI_Up)
				DPI_ADDON += DPI_INCREASE;
			if (event.jbutton.button == DPI_Down)
				DPI_ADDON -= DPI_INCREASE;

			if ((int)event.jbutton.button != lastButtonIndex)
			{
				lastButtonIndex = (int)event.jbutton.button;
				ButtonPressed->setText("Button Pressed: " + QString::number(lastButtonIndex));
			}
			break;

		case SDL_EVENT_JOYSTICK_BUTTON_UP:
			if (event.jbutton.button == Left_Click)
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			if (event.jbutton.button == Right_Click)
				mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			break;

		case SDL_EVENT_JOYSTICK_HAT_MOTION:
			hatState = event.jhat.value;
			break;

		default:
			break;
		}
	}

	int x = SDL_GetJoystickAxis(ControlledJoystick, 0);
	int y = SDL_GetJoystickAxis(ControlledJoystick, 1);

	if (hatState & SDL_HAT_UP)
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, +WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
	if (hatState & SDL_HAT_DOWN)
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
	if (hatState & SDL_HAT_LEFT)
		mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, -WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
	if (hatState & SDL_HAT_RIGHT)
		mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, +WHEEL_DELTA * SCROLL_SENSITIVITY, 0);

	if (abs(x) < DEAD_ZONE) x = 0;
	if (abs(y) < DEAD_ZONE) y = 0;

	if (x != 0 || y != 0) 
	{
		POINT pos;
		GetCursorPos(&pos);

		pos.x += static_cast<int>(x * SENSITIVITY + DPI_ADDON);
		pos.y += static_cast<int>(y * SENSITIVITY + DPI_ADDON);

		SetCursorPos(pos.x, pos.y);
	}
	changeCircle(x, y);
}