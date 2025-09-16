#include "JoystickMouse.h"

void Exit(SDL_JoystickID* Joystick) 
{
	SDL_free(Joystick);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	int count = 0;
	SDL_JoystickID* Joysticks = SDL_GetJoysticks(&count);

	if (count == 0) {
		Exit(Joysticks);
	}

	SDL_Joystick* Joystick = SDL_OpenJoystick(Joysticks[0]);
	if (!Joystick) {
		std::cerr << "Failed to open joystick" << SDL_GetError() << "\n";
	}

	const int DEAD_ZONE = 6000;
	const float SENSITIVITY = 0.0005f;
	const float SCROLL_SENSITIVITY = 0.05f;

	SDL_Event e;
	bool quit = false;
	int hatState = SDL_HAT_CENTERED;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) quit = true;
			if (e.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN) {
				if (e.jbutton.button == 0) {
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				}
				if (e.jbutton.button == 1) {
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				}
				std::cout << (int)e.jbutton.button << std::endl;
			}
			if (e.type == SDL_EVENT_JOYSTICK_BUTTON_UP) {
				if (e.jbutton.button == 0) {
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
			if (e.type == SDL_EVENT_JOYSTICK_HAT_MOTION) {
				hatState = e.jhat.value;
			}
		}

		// --- Poll joystick state continuously ---
		int x = SDL_GetJoystickAxis(Joystick, 0);
		int y = SDL_GetJoystickAxis(Joystick, 1);

			if (hatState & SDL_HAT_UP) {
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, +WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
			}
			if (hatState & SDL_HAT_DOWN) {
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
			}
			if (hatState & SDL_HAT_LEFT) {
				mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, -WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
			}
			if (hatState & SDL_HAT_RIGHT) {
				mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, +WHEEL_DELTA * SCROLL_SENSITIVITY, 0);
			}


		if (abs(x) < DEAD_ZONE) x = 0;
		if (abs(y) < DEAD_ZONE) y = 0;

		if (x != 0 || y != 0) {
			POINT pos;
			GetCursorPos(&pos);

			pos.x += static_cast<int>(x * SENSITIVITY);
			pos.y += static_cast<int>(y * SENSITIVITY);

			SetCursorPos(pos.x, pos.y);
		}

		SDL_Delay(10);
	}

	Exit(Joysticks);
	return 0;
}