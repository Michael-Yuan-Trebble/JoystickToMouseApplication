#pragma once

#include <qobject.h>
#include <memory>

class ActivateWindow;
class MainWindow;
class SettingsWindow;

class Navigator : public QObject 
{
	Q_OBJECT
public:
	explicit Navigator(QObject* parent = nullptr);
	~Navigator();
	void start();

private slots:
	void onBack();
	void onActivate();
	void onSettings();

private:
	MainWindow* mainWindow;
	ActivateWindow* activateWindow;
	SettingsWindow* settingsWindow;
};