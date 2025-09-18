#include "JoystickMouse.h"
#include "Windows/Navigator.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
	{
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	QApplication app(argc, argv);
	QApplication::setStyle("Fusion");

	QPalette darkPal;
	darkPal.setColor(QPalette::Window, QColor(45, 45, 45));
	darkPal.setColor(QPalette::WindowText, Qt::white);
	darkPal.setColor(QPalette::Button, QColor(53, 53, 53));
	darkPal.setColor(QPalette::ButtonText, Qt::white);
	app.setPalette(darkPal);

	Navigator nav;
	nav.start();

	return app.exec();
}