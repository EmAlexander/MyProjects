#include "engineoptions.h"
#include "ui_engineoptions.h"

EngineOptions::EngineOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EngineOptions)
{
    ui->setupUi(this);
    //layout()->setSizeConstraint(QLayout::SetFixedSize);
}

bool updateSett = false;

EngineOptions::~EngineOptions()
{
    delete ui;
}

bool EngineOptions::updateSettings()
{
    if(updateSett)
    {
        updateSett = false;
        return true;
    }
    return false;
}

void EngineOptions::on_pushButton_2_clicked()
{
    this->hide();
    ui->doubleSpinBox->setValue(cameraSpeed);
    ui->doubleSpinBox_2->setValue(camerafasts);
}

void EngineOptions::on_pushButton_clicked()
{
    /*QFile file("engine.enop");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QDataStream out(&file);
    out << ui->doubleSpinBox->value() << "\n" << ui->doubleSpinBox_2->value();
    file.close();*/

    QFile file("engine.enop");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;

    QTextStream out(&file);
    out << ui->doubleSpinBox->value() << "\n" << ui->doubleSpinBox_2->value() << "\n";

    cameraSpeed = ui->doubleSpinBox->value();
    camerafasts = ui->doubleSpinBox_2->value();

    this->hide();

    updateSett = true;
}

double EngineOptions::getCameraSpeed()
{
    return cameraSpeed;
}

double EngineOptions::getCameraFastS()
{
    return camerafasts;
}

void EngineOptions::setCameraSpeed(double cams)
{
    cameraSpeed = cams;
    ui->doubleSpinBox->setValue(cams);
}

void EngineOptions::setCameraFastS(double cams)
{
    camerafasts = cams;
    ui->doubleSpinBox_2->setValue(cams);
}
