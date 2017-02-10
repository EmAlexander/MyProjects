#ifndef EDITORWIDGETCL_H
#define EDITORWIDGETCL_H

#include <QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QtGui>
#include <QtCore>
#include <QEvent>
#include <QVector>
#include "gobjclass.h"

namespace setVars
{
    void setGrid(bool on);
    void setToolState(toolState state);

    void setXPos(double);
    void setYPos(double);
    void setRVal(int);
    void setGVal(int);
    void setBVal(int);
    void setXScale(double);
    void setYScale(double);

    bool saved();
    void setSaved(bool);

    void removeAllObjects();

    void setRotation(double);

    void setName(QString);
    void setScriptLoc(QString);
    void setScriptEnabled(bool);

    GObjClass* currentSelection();

    void addObj(objectType type);

    QVector<GObjClass*> getAll();
    void pushObj(GObjClass);
    void pushObj(GObjClass *);

    void setCameraSpeed(double);
    void setCameraFastSpeed(double);
}

class editorWidgetCl : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit editorWidgetCl(QWidget *parent = 0);

    QTimer *_timer;

    ~editorWidgetCl();

signals:

public slots:
    void _updateGl();

protected:
    void initializeGL();
    void resizeGL(int width, int height);

    void keyPressEvent ( QKeyEvent * e );
    void keyReleaseEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent * event);

    void wheelEvent(QWheelEvent * event);

    void paintGL();
};

#endif // EDITORWIDGETCL_H
