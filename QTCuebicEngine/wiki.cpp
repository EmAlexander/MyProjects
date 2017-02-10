#include "wiki.h"
#include "ui_wiki.h"

Wiki::Wiki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wiki)
{
    ui->setupUi(this);
}

Wiki::~Wiki()
{
    delete ui;
}
