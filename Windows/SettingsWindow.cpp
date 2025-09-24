#include "SettingsWindow.h"
#include <qpushbutton.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qvalidator.h>

SettingsWindow::SettingsWindow(QWidget* parent) : QMainWindow(parent) 
{
	this->resize(500, 500);
	loadSettings();

	QWidget* central = new QWidget(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(central);
	QHBoxLayout* ClickSettings = new QHBoxLayout();
	QHBoxLayout* SensitivityLayout = new QHBoxLayout();
	QHBoxLayout* DPIKeybinds = new QHBoxLayout();
	QHBoxLayout* DPISensitivityLayout = new QHBoxLayout();

	QJsonObject keybinds = obj["keybinds"].toObject();

	QLabel* leftLabel = new QLabel("Left Click", this);
	leftButton = new QPushButton(QString::number(keybinds["left_click"].toInt()), this);
	connect(leftButton, &QPushButton::clicked, this, [this]() {
		changeKeybind(QString("left_click"), leftButton);
	});

	QLabel* rightLabel = new QLabel("Right Click", this);
	rightButton = new QPushButton(QString::number(keybinds["right_click"].toInt()), this);
	connect(rightButton, &QPushButton::clicked, this, [this]() {
		changeKeybind(QString("right_click"), rightButton);
	});

	ClickSettings->addWidget(leftLabel);
	ClickSettings->addWidget(leftButton);
	ClickSettings->addWidget(rightLabel);
	ClickSettings->addWidget(rightButton);

	QLabel* Sensitivity = new QLabel("Sensitivity", this);
	SensitivityEdit = new QLineEdit(this);
	QDoubleValidator* validator = new QDoubleValidator(0.0, 1.0,4, SensitivityEdit);
	SensitivityEdit->setPlaceholderText("Enter a value between 0.0000 and 1.0000");
	SensitivityEdit->setValidator(validator);

	QPushButton* SensitivtyBtn = new QPushButton("Set", SensitivityEdit);

	connect(SensitivityEdit, &QLineEdit::returnPressed, this, &SettingsWindow::saveSensitivity);
	connect(SensitivtyBtn, &QPushButton::clicked, this, &SettingsWindow::saveSensitivity);
	SensitivityLayout->addWidget(Sensitivity);
	SensitivityLayout->addWidget(SensitivityEdit);
	SensitivityLayout->addWidget(SensitivtyBtn);

	QLabel* DPIUpLabel = new QLabel("DPI Up", this);
	DPIUpButton = new QPushButton(QString::number(keybinds["DPI_up"].toInt()), this);
	connect(DPIUpButton, &QPushButton::clicked, this, [this]() {
		changeKeybind(QString("DPI_up"), DPIUpButton);
	});
	DPIKeybinds->addWidget(DPIUpLabel);
	DPIKeybinds->addWidget(DPIUpButton);

	QLabel* DPIDownLabel = new QLabel("DPI Up", this);
	DPIDownButton = new QPushButton(QString::number(keybinds["DPI_down"].toInt()), this);
	connect(DPIDownButton, &QPushButton::clicked, this, [this]() {
		changeKeybind(QString("DPI_down"), DPIDownButton);
	});
	DPIKeybinds->addWidget(DPIDownLabel);
	DPIKeybinds->addWidget(DPIDownButton);

	QLabel* DPI = new QLabel("DPI Sensitivity", this);
	DPISensitivityEdit = new QLineEdit(this);
	QDoubleValidator* DPIvalidator = new QDoubleValidator(0.0, 1.0, 4, DPISensitivityEdit);
	DPISensitivityEdit->setPlaceholderText("Enter a value between 0.0000 and 1.0000");
	DPISensitivityEdit->setValidator(DPIvalidator);

	QPushButton* DPIBtn = new QPushButton("Set", DPISensitivityEdit);

	connect(DPISensitivityEdit, &QLineEdit::returnPressed, this, &SettingsWindow::saveDPI);
	connect(DPIBtn, &QPushButton::clicked, this, &SettingsWindow::saveDPI);
	DPISensitivityLayout->addWidget(DPI);
	DPISensitivityLayout->addWidget(DPISensitivityEdit);
	DPISensitivityLayout->addWidget(DPIBtn);

	mainLayout->addLayout(ClickSettings);
	mainLayout->addLayout(SensitivityLayout);
	mainLayout->addLayout(DPIKeybinds);
	mainLayout->addLayout(DPISensitivityLayout);
	setCentralWidget(central);

}

void SettingsWindow::saveSensitivity() 
{
	if (!SensitivityEdit->text().isEmpty() && SensitivityEdit->hasAcceptableInput()) 
	{
		obj["sensitivity"] = SensitivityEdit->text().toDouble();
		saveKeybind();
	}
}

void SettingsWindow::saveDPI() {
	if (!DPISensitivityEdit->text().isEmpty() && DPISensitivityEdit->hasAcceptableInput()) 
	{
		obj["dpi_sensitivity"] = DPISensitivityEdit->text().toDouble();
		saveKeybind();
	}
}

void SettingsWindow::loadSettings() 
{
	QString exeDir = QCoreApplication::applicationDirPath();
	QString configDir = exeDir + "/Settings";
	QString Filepath = configDir + "/settings.json";
	QFile file(Filepath);
	if (!file.open(QIODevice::ReadOnly)) return;
	QByteArray data = file.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(data);
	obj = doc.object();
}

void SettingsWindow::changeKeybind(const QString& keybind, QPushButton* Button) {
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [this, timer,keybind,Button]()
		{
			SDL_Event event;
			QJsonObject keybinds = obj["keybinds"].toObject();
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					keybinds[keybind] = event.jbutton.button;
					obj["keybinds"] = keybinds;
					Button->setText(QString::number(event.jbutton.button));
					saveKeybind();
					timer->stop();
					timer->deleteLater();
					return;
				}
			}
		});
	timer->start(5);
}

void SettingsWindow::saveKeybind() 
{
	QString exeDir = QCoreApplication::applicationDirPath();
	QString configDir = exeDir + "/Settings";
	QString Filepath = configDir + "/settings.json";
	QFile file(Filepath);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
	file.close();
}

void SettingsWindow::closeEvent(QCloseEvent* event) 
{
	emit settingsClosed();
	QMainWindow::closeEvent(event);
}