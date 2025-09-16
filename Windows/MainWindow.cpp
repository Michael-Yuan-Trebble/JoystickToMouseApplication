#include "MainWindow.h"
#include <qpushbutton.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) 
{
    this->resize(800, 800);

    // ------------------
    // Constantly Checks for Joysticks added/removed
    // ------------------

    QTimer* sdlTimer = new QTimer(this);
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
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::start);
    allJoysticks = new QLabel("All Joysticks: ", this);

    layout->addWidget(startBtn);
    layout->addWidget(allJoysticks);

    setCentralWidget(central);
    setWindowTitle("Joystick Mouse");

}

// ------------------
// Update Joysticks displayed
// ------------------

void MainWindow::updateJoysticks()
{

    for (QLabel* label : labels)
    {
        layout->removeWidget(label);
        label->deleteLater();
    }
    labels.clear();

    int count = 0;
    SDL_JoystickID* Joysticks = SDL_GetJoysticks(&count);
    if (count == 0) 
    {
        QLabel* noJoysticks = new QLabel("Joystick: None Detected", this);
        labels.push_back(noJoysticks);
        layout->addWidget(noJoysticks);
    }
    else 
    {
        for (int i = 0; i < count; i++)
        {
            const char* name = SDL_GetJoystickNameForID(Joysticks[i]);
            SDL_GUID guid = SDL_GetJoystickGUIDForID(Joysticks[i]);
            char guidStr[64];
            SDL_GUIDToString(guid, guidStr, sizeof(guidStr));

            QLabel* joystickLabel = new QLabel
            (
                QString("Joystick %1: %2 (ID %3)")
                .arg(i)
                .arg(name ? name : "Unknown")
                .arg(guidStr)
                ,this
            );
            labels.push_back(joystickLabel);
            layout->addWidget(joystickLabel);
        }
    }
}