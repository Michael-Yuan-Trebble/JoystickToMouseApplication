#pragma once

#include <qmainwindow.h>
#include <qobject.h>
#include <qtimer.h>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include <qboxlayout.h>
#include <qwidget.h>
#include <qjsonobject.h>
#include <qbytearray.h>
#include <qjsondocument.h>
#include <qlabel.h>
#include <qcoreapplication.h>
#include <qboxlayout.h>

class QPushButton;
class QLabel;
class QWidget;

class SettingsWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit SettingsWindow(QWidget* parent = nullptr);

private slots:
	void changeLeft();

	void changeRight();

signals:
	void settingsClosed();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	QJsonObject obj;

	Uint8 lastButton = -1;

	void loadSettings();

	void saveKeybind();

	QPushButton* leftButton;

	QPushButton* rightButton;
};