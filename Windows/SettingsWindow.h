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
class QLineEdit;

class SettingsWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit SettingsWindow(QWidget* parent = nullptr);

private slots:

	void changeKeybind(const QString& keybind, QPushButton* Button);

signals:
	void settingsClosed();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	QJsonObject obj;

	void loadSettings();

	void saveKeybind();

	void saveSensitivity();

	void saveDPI();

	QPushButton* leftButton;

	QPushButton* rightButton;

	QPushButton* DPIUpButton;

	QPushButton* DPIDownButton;

	QLineEdit* SensitivityEdit;

	QLineEdit* DPISensitivityEdit;
};