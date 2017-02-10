#ifndef ENGINEOPTIONS_H
#define ENGINEOPTIONS_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class EngineOptions;
}

class EngineOptions : public QDialog
{
    Q_OBJECT

public:
    explicit EngineOptions(QWidget *parent = 0);
    ~EngineOptions();

    double getCameraSpeed();
    double getCameraFastS();
    void setCameraSpeed(double);
    void setCameraFastS(double);

    bool updateSettings();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::EngineOptions *ui;

    double cameraSpeed;
    double camerafasts;
};

#endif // ENGINEOPTIONS_H
