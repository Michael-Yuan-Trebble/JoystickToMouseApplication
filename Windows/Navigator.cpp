#include "Navigator.h"
#include "MainWindow.h"
#include "ActivateWindow.h"
#include "SettingsWindow.h"

Navigator::Navigator(QObject* parent) : QObject(parent) { }

Navigator::~Navigator() = default;

// ------------------
// Main Screen
// ------------------

void Navigator::start()
{
	mainWindow = new MainWindow();
	connect(mainWindow, &MainWindow::start, this, &Navigator::onActivate);
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
		mainWindow->deleteLater();
		mainWindow = nullptr;
	}

	activateWindow = new ActivateWindow();
	connect(activateWindow, &ActivateWindow::goBack, this, &Navigator::onBack);
	connect(activateWindow, &ActivateWindow::goSettings, this, &Navigator::onSettings);
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
		activateWindow->deleteLater();
		activateWindow = nullptr;
	}
	mainWindow = new MainWindow();
	connect(mainWindow, &MainWindow::start, this, &Navigator::onActivate);
	mainWindow->show();
}

void Navigator::onSettings() {
	settingsWindow = new SettingsWindow(nullptr);
	settingsWindow->setWindowModality(Qt::WindowModal);
	settingsWindow->setWindowTitle("Settings");
	connect(settingsWindow, &SettingsWindow::destroyed, this, [this]() 
		{
			activateWindow->setEnabled(true);
			activateWindow->loadSettings();
		}
	);
		
	settingsWindow->show();
}