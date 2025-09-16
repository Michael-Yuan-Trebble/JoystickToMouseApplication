#include "ActivateWindow.h"
#include "Windows.h"
#include <qlabel.h>
#include <qpushbutton.h>

ActivateWindow::ActivateWindow(QWidget* parent) : QMainWindow(parent)
{
	this->resize(800, 800);
	QWidget* central = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(central);
	ButtonPressed = new QLabel("Button Pressed: ", this);
	layout->addWidget(ButtonPressed);
	push = new QPushButton("Go Back", this);
	connect(push, &QPushButton::clicked, this, &ActivateWindow::goBack);
	layout->addWidget(push);
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

	// TODO: Add a settings/button change screen for changing keybinds
}

void ActivateWindow::setJoystick(int index, SDL_JoystickID* joysticks) 
{
	ControlledJoystick = SDL_OpenJoystick(joysticks[index]);
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
		if (event.type == SDL_EVENT_QUIT)
		{
			close();
		}
		if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
		{
			if (event.jbutton.button == 0)
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			if (event.jbutton.button == 1)
				mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);

			if ((int)event.jbutton.button != lastButtonIndex)
			{
				lastButtonIndex = (int)event.jbutton.button;
				ButtonPressed->setText("Button Pressed: " + QString::number(lastButtonIndex));
			}
		}
		if (event.type == SDL_EVENT_JOYSTICK_BUTTON_UP)
		{
			if (event.jbutton.button == 0)
			{
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
		}
		if (event.type == SDL_EVENT_JOYSTICK_HAT_MOTION)
		{
			hatState = event.jhat.value;
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
}