#include "MainWindow.h"
#include <qpushbutton.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qcombobox.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) 
{
    this->resize(800, 800);

    // ------------------
    // Constantly Checks for Joysticks added/removed
    // ------------------

    QTimer* sdlTimer = new QTimer(this);
    combo = new QComboBox();
    connect(sdlTimer, &QTimer::timeout, this, [this]()
        {
        SDL_Event e;
        while (SDL_PollEvent(&e)) 
        {
            if (e.type == SDL_EVENT_JOYSTICK_ADDED)
            {
                updateJoysticks();
            }
            else if (e.type == SDL_EVENT_JOYSTICK_REMOVED)
            {
                updateJoysticks();
            }
        }
    });
    sdlTimer->start(16);

    startBtn = new QPushButton("Start Joystick Control", this);

    connect(startBtn, &QPushButton::clicked, this, [this]() 
    {
        SDL_JoystickID deviceID = (SDL_JoystickID)combo->currentData().toULongLong();
        SDL_Joystick* joystick = SDL_OpenJoystick(deviceID);
        if (!joystick) 
        {
            qDebug() << "Failed to open joystick:" << SDL_GetError();
            return;
        }
        emit start(joystick);
    });

    layout->addWidget(startBtn);
    layout->addWidget(combo);

    setCentralWidget(central);
    setWindowTitle("Joystick Mouse");

}

// ------------------
// Update Joysticks displayed
// ------------------

void MainWindow::updateJoysticks()
{
    combo->clear();

    int count = 0;
    SDL_JoystickID* Joysticks = SDL_GetJoysticks(&count);
    if (count == 0) 
    {
        combo->addItem("No Joysticks Detected");
        startBtn->setEnabled(false);
    }
    else 
    {
        startBtn->setEnabled(true);
        for (int i = 0; i < count; i++)
        {
            const char* name = SDL_GetJoystickNameForID(Joysticks[i]);

            combo->addItem(name ? name : "Unknown", QVariant::fromValue((qulonglong)Joysticks[i]));
        }
    }
}