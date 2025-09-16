#include "Navigator.h"
#include "MainWindow.h"
#include "ActivateWindow.h"

Navigator::Navigator(QObject* parent) : QObject(parent) { }

Navigator::~Navigator() = default;

// ------------------
// Main Screen
// ------------------

void Navigator::start()
{
	mainWindow = std::make_unique<MainWindow>();
	connect(mainWindow.get(), &MainWindow::start, this, &Navigator::onActivate);
	mainWindow->show();
}

// ------------------
// Activation Screen
// ------------------

void Navigator::onActivate() 
{
	if (mainWindow) 
	{
		mainWindow->close();
		mainWindow.reset();
	}

	activateWindow = std::make_unique<ActivateWindow>();
	connect(activateWindow.get(), &ActivateWindow::goBack, this, &Navigator::onBack);
	activateWindow->show();
}

// ------------------
// TODO: Go back to whatever previous screen it was
// ------------------

void Navigator::onBack() 
{
	if (activateWindow) 
	{
		activateWindow->close();
		activateWindow.reset();
	}
	mainWindow = std::make_unique<MainWindow>();
	connect(mainWindow.get(), &MainWindow::start, this, &Navigator::onActivate);
	mainWindow->show();
}