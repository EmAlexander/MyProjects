#ifndef WIKI_H
#define WIKI_H

#include <QDialog>

namespace Ui {
class Wiki;
}

class Wiki : public QDialog
{
    Q_OBJECT

public:
    explicit Wiki(QWidget *parent = 0);
    ~Wiki();

private:
    Ui::Wiki *ui;
};

#endif // WIKI_H
