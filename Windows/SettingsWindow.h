#pragma once

#include <qmainwindow.h>
#include <qobject.h>
#include <qtimer.h>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include <qboxlayout.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>

class QPushButton;
class QLabel;
class QWidget;

class SettingsWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit SettingsWindow(QWidget* parent = nullptr);

signals:

private:

};