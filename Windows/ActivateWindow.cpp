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
	ButtonPressed = new QLabel("Button Pressed: ", this);
	layout->addWidget(ButtonPressed);

	checkSettings();
	loadSettings();

	layout->addWidget(LeftClickLabel);
	layout->addWidget(RightClickLabel);

	Visual = new JoystickVisualizer(this);
	layout->addWidget(Visual);


	push = new QPushButton("Go Back", this);
	connect(push, &QPushButton::clicked, this, &ActivateWindow::goBack);
	layout->addWidget(push);

	settingsbtn = new QPushButton("Settings", this);
	connect(settingsbtn, &QPushButton::clicked, this, &ActivateWindow::goSettings);
	layout->addWidget(settingsbtn);

	setCentralWidget(central);
	setWindowTitle("Active");

	// ------------------
	// Start SDL Joystick Functions
	// ------------------

	int count = 0;
	SDL_JoystickID* Joysticks = SDL_GetJoysticks(&count);
	if (count == 0) 
	{
		// TODO: Constantly check for inputted deviced and change accordingly
		QLabel* Error = new QLabel("No Joystick detected");
	}
	else 
	{
		setJoystick(0, Joysticks);
		if (!ControlledJoystick) return;
		QTimer* timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &ActivateWindow::setMovement);
		timer->start(10); 
	}
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
	SCROLL_SENSITIVITY = obj["scroll_sensitivity"].toDouble();
	QJsonObject keybinds = obj["keybinds"].toObject();
	Left_Click = keybinds["left_click"].toInt();
	Right_Click = keybinds["right_click"].toInt();

	if (LeftClickLabel || RightClickLabel) {
		LeftClickLabel = new QLabel("Left Click Button: " + QString::number(Left_Click), this);
		RightClickLabel = new QLabel("Right Click Button: " + QString::number(Right_Click), this);
	}
	else {
		LeftClickLabel->setText("Left Click Button: " + QString::number(Left_Click));
		RightClickLabel->setText("Right Click Button: " + QString::number(Right_Click));
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
// TODO: Currently only sets joystick to first one detected, make it changeable
void ActivateWindow::setJoystick(int index, SDL_JoystickID* joysticks) 
{
	ControlledJoystick = SDL_OpenJoystick(joysticks[index]);
}

void ActivateWindow::changeCircle(int x, int y) {
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

		pos.x += static_cast<int>(x * SENSITIVITY);
		pos.y += static_cast<int>(y * SENSITIVITY);

		SetCursorPos(pos.x, pos.y);
	}
	changeCircle(x, y);
}