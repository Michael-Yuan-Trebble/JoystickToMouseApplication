#pragma once

#include <qobject.h>
#include <memory>

class ActivateWindow;
class MainWindow;

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

private:
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<ActivateWindow> activateWindow;
};