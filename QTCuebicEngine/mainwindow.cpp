#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace buildOp
{
bool windows_down;
bool linix_down;
bool mac_down;

int WidthValue = 1280;
int HeightValue = 720;

int EncryptionLevel = 5;

bool NotEncrypted = false;

QString BuildLocation;
}

QSet<int> windowKeysPressed;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    __timer = new QTimer(this);
    connect(__timer, SIGNAL(timeout()), this, SLOT(_update()));
    __timer->start(30);

    bOptIn = new BuildOptions(this);
    bOptIn->hide();
    eOpt = new EngineOptions(this);
    eOpt->hide();

    saved = true;
    savedToFile = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_update()
{
    for(int i = 0; i < windowKeysPressed.size(); i++)
    {
        qDebug() << windowKeysPressed.toList().at(i);
    }

    if(eOpt->updateSettings())
    {
        setVars::setCameraSpeed(eOpt->getCameraSpeed());
        setVars::setCameraFastSpeed(eOpt->getCameraFastS());
    }

    if(!setVars::saved())
    {
        if(savedToFile)
        {
            this->setWindowTitle("Cuebic Engine ["+ savePath +"]*");
        }
    }

    GObjClass* _in = setVars::currentSelection();

    if(_in != 0)
    {
        ui->doubleSpinBox->setEnabled(true);
        ui->doubleSpinBox->setValue(_in->getPosition().x);

        ui->doubleSpinBox_2->setEnabled(true);
        ui->doubleSpinBox_2->setValue(_in->getPosition().y);

        ui->doubleSpinBox_4->setEnabled(true);
        ui->doubleSpinBox_4->setValue(_in->getRotation());

        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_3->setText(_in->getObjName());

        ui->spinBox->setEnabled(true);
        ui->spinBox->setValue(_in->getTrueColor().red());

        ui->spinBox_2->setEnabled(true);
        ui->spinBox_2->setValue(_in->getTrueColor().green());

        ui->spinBox_3->setEnabled(true);
        ui->spinBox_3->setValue(_in->getTrueColor().blue());

        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_2->setText(_in->getScriptLoc());

        ui->pushButton_3->setEnabled(true);

        ui->radioButton_3->setEnabled(true);
        ui->radioButton_3->setChecked(true);

        ui->doubleSpinBox_5->setEnabled(true);//x
        ui->doubleSpinBox_5->setValue(_in->getScale().x);

        ui->doubleSpinBox_3->setEnabled(true);
        ui->doubleSpinBox_3->setValue(_in->getScale().y);
    }
    else
    {
        ui->doubleSpinBox->setEnabled(false);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->doubleSpinBox_4->setEnabled(false);
        ui->lineEdit_3->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        ui->spinBox_3->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->doubleSpinBox_5->setEnabled(false);
        ui->doubleSpinBox_3->setEnabled(false);
    }
}

void MainWindow::on_actionGrid_triggered(bool checked)
{
    setVars::setGrid(checked);
}

void MainWindow::on_actionTranslate_triggered()
{
    setVars::setToolState(TOOL_TRANS);
}

void MainWindow::on_actionRotate_triggered()
{
    setVars::setToolState(TOOL_ROT);
}

void MainWindow::on_actionScale_triggered()
{
    setVars::setToolState(TOOL_SCL);
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));

    ui->lineEdit->setText(fileName);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Script"), "", tr("All Files (*);;Script Files (*.lua *.txt *.py)"));

    ui->lineEdit_2->setText(fileName);
}

void MainWindow::on_RunAndDebugButton_clicked()
{
    QTextStream out(stdout);

    std::string trueFile = ui->lineEdit_2->text().toStdString();
    bool _failed = false;

    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dostring(L, "cue_debuging = true");

    luaMod::luaopen_GObjClass(L);

    if (luaL_dofile(L, trueFile.c_str())) {
      out << "Something went wrong loading the chunk (syntax error?)" << endl;
      out << lua_tostring(L, -1) << endl;
      lua_pop(L,1);
      _failed = true;
    }

    if(!_failed)
    {
        out << "Ran with no errors!\n";
    }

    lua_close(L);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    setVars::setXPos(arg1);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    setVars::setYPos(arg1);
}


void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    setVars::setName(arg1);
}

void MainWindow::on_actionBuild_Options_triggered()
{
    bOptIn->show();
}

void MainWindow::on_actionRectangle_triggered()
{
    setVars::addObj(OBTYPE_RECT);
}

void MainWindow::on_actionWiki_triggered()
{
    wikiWindow = new Wiki(this);
    wikiWindow->show();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    setVars::setRVal(arg1);
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    setVars::setGVal(arg1);
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    setVars::setBVal(arg1);
}

void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    setVars::setRotation(arg1);
}

void MainWindow::on_actionSave_as_triggered()
{
    //SAVE
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As..."), "", tr("Cuebic Project Files (*.cpf *.cpr *.pro)"));

    QVector<GObjClass*> allObjs = setVars::getAll();

    QFile _file( fileName );
    if ( _file.open(QIODevice::ReadWrite) )
    {
        QTextStream fsstream( &_file );
        for(int i = 0; i < allObjs.size(); i++)
        {
            fsstream << allObjs.at(i)->getColorID().red() << endl;
            fsstream << allObjs.at(i)->getColorID().green() << endl;
            fsstream << allObjs.at(i)->getColorID().blue() << endl;

            fsstream << allObjs.at(i)->getObjType() << endl;
            fsstream << allObjs.at(i)->getRotation() << endl;
            fsstream << allObjs.at(i)->getPosition().x << endl; //x12 y13
            fsstream << allObjs.at(i)->getPosition().y << endl;
            fsstream << allObjs.at(i)->getObjName() << endl;
            fsstream << allObjs.at(i)->getShaderID() << endl;
            fsstream << allObjs.at(i)->getSelected() << endl;
            //fsstream << allObjs.at(i).trans << endl;

            fsstream << allObjs.at(i)->getTrueColor().red() << endl;
            fsstream << allObjs.at(i)->getTrueColor().green() << endl;
            fsstream << allObjs.at(i)->getTrueColor().blue() << endl;

            fsstream << allObjs.at(i)->getObjType() << endl;
            fsstream << allObjs.at(i)->getVertexCount() << endl;

            fsstream << allObjs.at(i)->getScriptLoc() << endl;

            fsstream << allObjs.at(i)->getScale().x << endl;
            fsstream << allObjs.at(i)->getScale().y << endl;
            fsstream << "---" << endl;
        }

        savePath = fileName;

        this->setWindowTitle("Cuebic Engine ["+ fileName +"]");

        fileName.remove(fileName.length()-3, fileName.length());
        fileName += "cbd";

        /*QFile cbdF(fileName);
        if(cbdF.open(QIODevice::ReadWrite))
        {
            QTextStream _fsstream( &cbdF );

            //Windows
            _fsstream << bOptIn->pushButton_2_down;
            //Linix
            _fsstream << bOptIn->pushButton_3_down;
            //mac
            _fsstream << bOptIn->pushButton_4_down;

            _fsstream << bOptIn->WidthSpinBox_value;
            _fsstream << bOptIn->HeightSpinBox_value;

            //encryped
            _fsstream << bOptIn->radioButton_down;

            _fsstream << bOptIn->BuildLocation_value;
        }*/

        saved = true;
        savedToFile = true;
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(savedToFile)
    {
        QVector<GObjClass*> allObjs = setVars::getAll();

        QFile _file( savePath );
        if ( _file.open(QIODevice::ReadWrite) )
        {
            QTextStream fsstream( &_file );
            for(int i = 0; i < allObjs.size(); i++)
            {
                fsstream << allObjs.at(i)->getColorID().red() << endl;
                fsstream << allObjs.at(i)->getColorID().green() << endl;
                fsstream << allObjs.at(i)->getColorID().blue() << endl;

                fsstream << allObjs.at(i)->getObjType() << endl;
                fsstream << allObjs.at(i)->getRotation() << endl;
                fsstream << allObjs.at(i)->getPosition().x << endl; //x12 y13
                fsstream << allObjs.at(i)->getPosition().y << endl;
                fsstream << allObjs.at(i)->getObjName() << endl;
                fsstream << allObjs.at(i)->getShaderID() << endl;
                fsstream << allObjs.at(i)->getSelected() << endl;
                //fsstream << allObjs.at(i).trans << endl;

                fsstream << allObjs.at(i)->getTrueColor().red() << endl;
                fsstream << allObjs.at(i)->getTrueColor().green() << endl;
                fsstream << allObjs.at(i)->getTrueColor().blue() << endl;

                fsstream << allObjs.at(i)->getObjType() << endl;
                fsstream << allObjs.at(i)->getVertexCount() << endl;

                fsstream << allObjs.at(i)->getScriptLoc() << endl;

                fsstream << allObjs.at(i)->getScale().x << endl;
                fsstream << allObjs.at(i)->getScale().y << endl;
                fsstream << "---" << endl;
            }

            this->setWindowTitle("Cuebic Engine ["+ savePath +"]");

            saved = true;
            setVars::setSaved(true);
        }
    }
    else
    {
        on_actionSave_as_triggered();
    }
}

void MainWindow::on_actionNew_triggered()
{
    QMessageBox msgBox;
    msgBox.setInformativeText("Are you sure you want to make a new project? All unsaved changes will be lost!");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes)
    {
        savedToFile = false;

        savePath = "";

        setVars::removeAllObjects();

        this->setWindowTitle("Cuebic Engine");
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("Cuebic Project Files (*.cpf *.cpr *.pro)"));

    QFile _file( fileName );

    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file!";
    }
    else
    {
        setVars::removeAllObjects();
        savedToFile = true;
        savePath = fileName;
        this->setWindowTitle("Cuebic Engine ["+ savePath +"]");

        fileName.remove(fileName.length()-3, fileName.length());
        fileName += "cbd";

        //bOptIn->setPath(fileName);
    }

    QTextStream in(&_file);
    //QString line = in.readLine();

    //GObjClass* temp = new GObjClass();

    while (true)
    {
        GObjClass* temp = new GObjClass();
        QColor datColor;

        temp->resetTrans();
        temp->resetRot();

        datColor.setRed(in.readLine().toDouble());
        datColor.setGreen(in.readLine().toDouble());
        datColor.setBlue(in.readLine().toDouble());
        temp->setColorID(datColor);
        /*temp->colorID.setRed(in.readLine().toFloat());
        temp->colorID.setGreen(in.readLine().toFloat());
        temp->colorID.setBlue(in.readLine().toFloat());*/

        int _enum = in.readLine().toInt();

        if(_enum == 0)
        {
           //temp->GObjectType = OBTYPE_RECT;
           temp->setObjType(OBTYPE_RECT);
        }

        /*temp->lastRot = in.readLine().toFloat();
        temp->rot.rotate(temp->lastRot, 0, 0, 1.0f);*/
        temp->setRotation(in.readLine().toDouble());
        /*temp->trans.translate(in.readLine().toFloat(), 0);
        temp->trans.translate(0, in.readLine().toFloat());*/
        double x = in.readLine().toDouble();
        double y = in.readLine().toDouble();
        temp->setPosition(x, y);
        //temp->objName = in.readLine();
        temp->setObjName(in.readLine());
        //temp->renderId = in.readLine().toFloat();
        temp->setShaderID(in.readLine().toDouble());
        //temp->selected = in.readLine().toInt();
        temp->setSelected(in.readLine().toInt());
        /*temp->trueColor.setRed(in.readLine().toFloat());
        temp->trueColor.setGreen(in.readLine().toFloat());
        temp->trueColor.setBlue(in.readLine().toFloat());*/
        datColor.setRed(in.readLine().toDouble());
        datColor.setGreen(in.readLine().toDouble());
        datColor.setBlue(in.readLine().toDouble());
        temp->setTrueColor(datColor);

        _enum = in.readLine().toInt();

        if(_enum == 0)
        {
            //temp->type = SHADER_COLOR;
            temp->setShaderType(SHADER_COLOR);
        }
        else if(_enum == 1)
        {
            //temp->type = SHADER_TEX;
            temp->setShaderType(SHADER_TEX);
        }
        else if(_enum == 2)
        {
            //temp->type = SHADER_TEX_COLOR;
            temp->setShaderType(SHADER_TEX_COLOR);
        }

       // temp->vertexCount = in.readLine().toFloat();
        temp->setVertexCount(in.readLine().toInt());

        //temp->script = in.readLine();
        temp->setScriptLoc(in.readLine());

        double scaleX = in.readLine().toDouble();
        double scaleY = in.readLine().toDouble();

        temp->setScale(scaleX, scaleY);

        QString fLine = in.readLine();

        if(fLine.isNull() || fLine == "")
        {
            break;
        }

        if(fLine == "---" || fLine == "---\n")
        {
            setVars::pushObj(temp);
        }

        //temp->GObjectType\ = 0;
        //line = in.readLine();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About cuebic engine", "test");
}

void MainWindow::on_actionPlay_triggered()
{
    ui->actionPlay->setEnabled(false);

    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_actionStop_triggered()
{
    ui->actionPlay->setEnabled(true);
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    setVars::setScriptLoc(arg1);
}

void MainWindow::on_radioButton_3_clicked(bool checked)
{
    setVars::setScriptEnabled(checked);
}

void MainWindow::on_actionEngine_Settings_triggered()
{ 
    eOpt->show();

    /*QFile file("engine.enop");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        eOpt->setCameraSpeed(0.01);
        eOpt->setCameraFastS(0.05);
        return;
    }

    file.close();*/

    QFile file("engine.enop");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        eOpt->setCameraSpeed(0.01);
        eOpt->setCameraFastS(0.05);
        return;
    }

    QTextStream in(&file);

    double line = in.readLine().toDouble();
    eOpt->setCameraSpeed(line);
    line = in.readLine().toDouble();
    eOpt->setCameraFastS(line);
}

void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    setVars::setXScale(arg1);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    setVars::setYScale(arg1);
}

bool _m_bFirstRelease = false;

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    _m_bFirstRelease = true;
    windowKeysPressed+= event->key();
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
    /*if(m_bFirstRelease)
    {
        processMultiKeys(windowKeysPressed);
    }*/
    _m_bFirstRelease = false;
    windowKeysPressed-= event->key();
}
