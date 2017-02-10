#ifndef BUILDOPTIONS_H
#define BUILDOPTIONS_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class BuildOptions;
}

class BuildOptions : public QDialog
{
    Q_OBJECT

public:
    explicit BuildOptions(QWidget *parent = 0);
    ~BuildOptions();

    //windows
    bool pushButton_2_down;
    //linix
    bool pushButton_3_down;
    //mac
    bool pushButton_4_down;

    int WidthSpinBox_value;
    int HeightSpinBox_value;

    //encryption level
    int horizontalSlider_value;

    //encryped
    bool radioButton_down;

    QString BuildLocation_value;

    void updateValues();

private slots:
    void on_pushButton_clicked();

    void on_CancelButton_clicked();

    void on_ApplyButton_clicked();

private:
    Ui::BuildOptions *ui;
};

#endif // BUILDOPTIONS_H
