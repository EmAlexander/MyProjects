#include "buildoptions.h"
#include "ui_buildoptions.h"

BuildOptions::BuildOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildOptions)
{
    ui->setupUi(this);
}

BuildOptions::~BuildOptions()
{
    delete ui;
}

void BuildOptions::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this,
                                                         tr("Open Directory"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly |
                                                         QFileDialog::DontResolveSymlinks);
    ui->BuildLocation->setText(fileName);
}

void BuildOptions::on_CancelButton_clicked()
{
    updateValues();
    this->hide();
}

void BuildOptions::on_ApplyButton_clicked()
{
    WidthSpinBox_value = ui->WidthSpinBox->value();
    HeightSpinBox_value = ui->HeightSpinBox->value();
    radioButton_down = ui->radioButton->isChecked();
    BuildLocation_value = ui->BuildLocation->text();
    pushButton_2_down = ui->pushButton_2->isChecked();
    pushButton_3_down = ui->pushButton_3->isChecked();
    pushButton_4_down = ui->pushButton_4->isChecked();

}

void BuildOptions::updateValues()
{
    ui->WidthSpinBox->setValue(WidthSpinBox_value);
    ui->HeightSpinBox->setValue(HeightSpinBox_value);
    ui->radioButton->setChecked(radioButton_down);
    ui->BuildLocation->setText(BuildLocation_value);
    ui->pushButton_2->setChecked(pushButton_2_down);
    ui->pushButton_3->setChecked(pushButton_3_down);
    ui->pushButton_4->setChecked(pushButton_4_down);
}
