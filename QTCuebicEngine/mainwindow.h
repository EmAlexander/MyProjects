#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editorwidgetcl.h"
#include <QFileDialog>
#include <QMessageBox>
#include "lua.hpp"
#include "buildoptions.h"
#include "wiki.h"
#include "engineoptions.h"
#include <QDockWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionGrid_triggered(bool checked);

    void on_actionTranslate_triggered();

    void on_actionRotate_triggered();

    void on_actionScale_triggered();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_RunAndDebugButton_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void _update();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_actionBuild_Options_triggered();

    void on_actionRectangle_triggered();

    void on_actionWiki_triggered();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionAbout_triggered();

    void on_actionPlay_triggered();

    void on_actionStop_triggered();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_radioButton_3_clicked(bool checked);

    void on_actionEngine_Settings_triggered();

    void on_doubleSpinBox_5_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

protected:
    void keyPressEvent(QKeyEvent * );
    void keyReleaseEvent(QKeyEvent *);

private:
    Ui::MainWindow *ui;
    BuildOptions *bOptIn;
    EngineOptions *eOpt;

    Wiki *wikiWindow;

    QTimer *__timer;

    QString savePath;

    bool saved;

    bool savedToFile;
};

#endif // MAINWINDOW_H
