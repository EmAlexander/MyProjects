#include "editorwidgetcl.h"

editorWidgetCl::editorWidgetCl(QWidget *parent) :
    QGLWidget(parent)
{
}

//float camX = 0, camY = 0;
float scale = 0;
bool grid = false;
QMatrix4x4 ani;
QMatrix4x4 camera;
QMatrix4x4 projection;
QMatrix4x4 mdl;
QOpenGLShaderProgram shaderProgram;
toolState currentState = TOOL_NON;
QVector<GObjClass*> newObjects;
bool selectObj = false;
bool _saved = true;

bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool shiftPressed = false;

GLuint rectVBO = 0;
GLuint rectIBO = 0;

int winW = 0, winH = 0;

GLuint gridVBO = 0;
GLuint gridIBO = 0;

double cameraSpeed = 0.01;
double cameraFastSpeed = 0.05;

void setVars::setCameraSpeed(double in)
{
    cameraSpeed = in;
}

void setVars::setCameraFastSpeed(double in)
{
    cameraFastSpeed = in;
}

bool setVars::saved()
{
    return _saved;
}

void setVars::removeAllObjects()
{
    //objects.clear();
    for(int i = 0; i < newObjects.size(); i++)
    {
        delete newObjects[i];
    }
    newObjects.clear();
}

void setVars::pushObj(GObjClass obj)
{
    GObjClass* tempClass = new GObjClass();

    tempClass->setRotation(obj.getRotation());
    tempClass->setColorID(obj.getColorID());
    tempClass->setObjName(obj.getObjName());
    tempClass->setPosition(obj.getPosition().x, obj.getPosition().y);
    tempClass->setScale(obj.getScale().x, obj.getScale().y);
    tempClass->setScriptLoc(obj.getScriptLoc());
    tempClass->setTrueColor(obj.getTrueColor());
    tempClass->setIBO(rectIBO);
    tempClass->setVBO(rectVBO);
    tempClass->setVertexCount(obj.getVertexCount());
    tempClass->setShaderType(obj.getShaderType());
    tempClass->setShaderID(obj.getShaderID());
    tempClass->setObjType(obj.getObjType());

    newObjects.push_back(tempClass);

}

void setVars::pushObj(GObjClass* obj)
{
    obj->setIBO(rectIBO);
    obj->setVBO(rectVBO);
    newObjects.push_back(obj);
}

void setVars::setSaved(bool in)
{
    _saved = in;
}

 void setVars::setGrid(bool on)
 {
     grid = on;
 }

 void setVars::setXPos(double args1)
 {
    for(int i = 0; i < newObjects.size(); i++)
    {
        if(newObjects.at(i)->getSelected())
        {
            newObjects[i]->setPosition(args1,newObjects.at(i)->getPosition().y);

            _saved = false;
        }
    }
 }

 void setVars::setXScale(double scale)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setXScale(scale);

             _saved = false;
         }
     }
 }

 void setVars::setYScale(double scale)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setYScale(scale);

             _saved = false;
         }
     }
 }

void setVars::setScriptLoc(QString srcLoc)
{
    for(int i = 0; i < newObjects.size(); i++)
    {
        if(newObjects.at(i)->getSelected())
        {
            newObjects[i]->setScriptLoc(srcLoc);

            _saved = false;
        }
    }
}

 QVector<GObjClass*> setVars::getAll()
 {
     return newObjects;
 }

 void setVars::addObj(objectType type)
 {
    if(type == OBTYPE_RECT)
    {
        GObjClass* tempObj = new GObjClass();

        int tempR = 1;
        int tempG = 1;
        int tempB = 1;

        for(int i = 0; i < 255; i++)
        {
            for(int ii = 0; ii < newObjects.size(); ii++)
            {
                if(tempR == newObjects.at(ii)->getColorID().red())
                {
                    tempR++;
                }
            }
        }

        for(int i = 0; i < 255; i++)
        {
            for(int ii = 0; ii < newObjects.size(); ii++)
            {
                if(tempG == newObjects.at(ii)->getColorID().green())
                {
                    tempG++;
                }
            }
        }

        for(int i = 0; i < 255; i++)
        {
            for(int ii = 0; ii < newObjects.size(); ii++)
            {
                if(tempB == newObjects.at(ii)->getColorID().blue())
                {
                    tempB++;
                }
            }
        }

        qDebug() << tempR << " " << tempG << " " << tempB;

        tempObj->setColorID(QColor(tempR, tempG, tempB));
        tempObj->setObjType(OBTYPE_RECT);
        //tempObj.rotate(0);
        tempObj->setObjName("default");
        tempObj->setShaderID(newObjects.size());
        tempObj->setSelected(false);
        tempObj->setShaderType(SHADER_COLOR);
        tempObj->setVertexCount(4);
        tempObj->setTrueColor(QColor(255, 0 ,0));
        tempObj->setVBO(rectVBO);
        tempObj->setIBO(rectIBO);

        newObjects.push_back(tempObj);

        _saved = false;
    }
 }

 void setVars::setYPos(double args2)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setPosition(newObjects.at(i)->getPosition().x, args2);

             _saved = false;
         }
     }
 }

 void setVars::setRVal(int red)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setTrueColor(QColor(red, newObjects.at(i)->getTrueColor().green(), newObjects.at(i)->getTrueColor().blue()));
             _saved = false;
         }
     }
 }

 void setVars::setGVal(int green)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setTrueColor(QColor(newObjects.at(i)->getTrueColor().red(), green, newObjects.at(i)->getTrueColor().blue()));
             _saved = false;
         }
     }
 }

 void setVars::setBVal(int blue)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setTrueColor(QColor(newObjects.at(i)->getTrueColor().red(), newObjects.at(i)->getTrueColor().green(), blue));
             _saved = false;
         }
     }
 }

 void setVars::setRotation(double rot)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             newObjects[i]->setRotation(rot);

             _saved = false;
         }
     }
 }

 void setVars::setName(QString _nm)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
            /*GObject _temp = objects.at(i);
            objects.remove(i);
            _temp.objName = _nm;
            objects.push_back(_temp);*/

             newObjects[i]->setObjName(_nm);

            _saved = false;
         }
     }
 }

 void setVars::setScriptEnabled(bool in)
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
           //objects[i].scriptEnabled = in;
            _saved = false;
         }
     }
 }

 GObjClass* setVars::currentSelection()
 {
     for(int i = 0; i < newObjects.size(); i++)
     {
         if(newObjects.at(i)->getSelected())
         {
             return newObjects.at(i);
         }
     }

     return 0;
 }

 void setVars::setToolState(toolState state)
 {
     currentState = state;
 }

 void editorWidgetCl::initializeGL()
 {
     setFocusPolicy(Qt::StrongFocus);

     makeCurrent();
     initializeOpenGLFunctions();

     glEnable(GL_DEPTH_TEST);
     //glDepthFunc(GL_LESS);

     QOpenGLShader *vshader1 = NULL;
     QOpenGLShader *fshader1 = NULL;

     const char *vsrc1 =
           "in vec2 pos2d;\n"
           "uniform mat4 camera;\n"
           "uniform mat4 anim;\n"
           "void main( void )\n"
           "{\n"
           "gl_Position = camera * anim * vec4( pos2d.x, pos2d.y, 0, 1 );\n"
           "}\n";

     const char *fsrc1 =
           "uniform vec4 color;\n"
           "void main( void ){\n"
           "gl_FragColor = vec4( color.x, color.y, color.z, 1.0 );\n"
           "}\n";

     vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, this);
     fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, this);

     if( !vshader1->compileSourceCode(vsrc1) )
     {
         exit(EXIT_FAILURE);
     }
     if( !fshader1->compileSourceCode(fsrc1) )
     {
         exit(EXIT_FAILURE);
     }
     if( !shaderProgram.addShader(vshader1) )
     {
         exit(EXIT_FAILURE);
     }
     if( !shaderProgram.addShader(fshader1) )
     {
         exit(EXIT_FAILURE);
     }
     if( !shaderProgram.link() )
     {
         exit(EXIT_FAILURE);
     }

     setAutoBufferSwap (true);

     _timer = new QTimer(this);
     connect(_timer, SIGNAL(timeout()), this, SLOT(_updateGl()));
     _timer->start(30);

     QVector3D eye(0.0, 0.0, 0.0);
     QVector3D center(0.0, 0.0, -4.0);
     QVector3D up(0.0, 1.0, 0.0);

     ani.rotate(45, 0.0f, 0.0f, 1.0f);

     camera.lookAt(eye,center,up);
     mdl.translate(0.0, 0.0, -4.0);
     projection.perspective(60, 1.0, 0.01f, 100.0f);

     glClearColor(0, 0.502f, 0.757f, 1.0f);

     GLfloat vertexData[] =
     {
         -0.5f, -0.5f,
          0.5f, -0.5f,
          0.5f,  0.5f,
         -0.5f,  0.5f
     };

     GLuint indexData[] = { 0, 1, 2, 3 };

     glGenBuffers( 1, &rectVBO );
     glBindBuffer( GL_ARRAY_BUFFER, rectVBO );
     glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

     glGenBuffers( 1, &rectIBO );
     glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rectIBO );
     glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

     /*GObjClass tempObj;

     tempObj.vbo = rectVBO;
     tempObj.vertexCount = 4;
     tempObj.type = SHADER_COLOR;
     tempObj.ibo = rectIBO;
     tempObj.colorID = QColor(250,254,190);
     tempObj.trueColor = QColor(255, 0, 0);
     tempObj.selected = false;
     tempObj.rot.rotate(25, 0.0, 0.0, 1.0);
     tempObj.lastRot = 25;
     tempObj.renderId = objects.size();

     objects.push_back(tempObj);

     tempObj.rot.rotate(-25, 0.0, 0.0, 1.0);
     tempObj.colorID = QColor(120,102,15);
     tempObj.trueColor = QColor(0,255,0);
     tempObj.lastRot = 0;
     tempObj.renderId = objects.size();

     objects.push_back(tempObj);*/

     GLfloat gridData[] =
     {
         -10.0f, 10.0f,
          10.0f, 10.f,
         -10.0f, 9.0f,
          10.0f, 9.0f,
         -10.0f, 8.0f,
          10.0f, 8.0f,
         -10.0f, 7.0f,
          10.0f, 7.0f,
         -10.0f, 6.0f,
          10.0f, 6.0f,
         -10.0f, 5.0f,
          10.0f, 5.0f,
         -10.0f, 4.0f,
          10.0f, 4.0f,
         -10.0f, 3.0f,
          10.0f, 3.0f,
         -10.0f, 2.0f,
          10.0f, 2.0f,
         -10.0f, 1.0f,
          10.0f, 1.0f,
         -10.0f, 0.0f,
          10.0f, 0.0f,

         -10.0f, -10.0f,
          10.0f, -10.f,
         -10.0f, -9.0f,
          10.0f, -9.0f,
         -10.0f, -8.0f,
          10.0f, -8.0f,
         -10.0f, -7.0f,
          10.0f, -7.0f,
         -10.0f, -6.0f,
          10.0f, -6.0f,
         -10.0f, -5.0f,
          10.0f, -5.0f,
         -10.0f, -4.0f,
          10.0f, -4.0f,
         -10.0f, -3.0f,
          10.0f, -3.0f,
         -10.0f, -2.0f,
          10.0f, -2.0f,
         -10.0f, -1.0f,
          10.0f, -1.0f,

          10.0f, -10.0f,
          10.0f,  10.0f,
          9.0f, -10.0f,
          9.0f,  10.0f,
          8.0f, -10.0f,
          8.0f,  10.0f,
          7.0f, -10.0f,
          7.0f,  10.0f,
          6.0f, -10.0f,
          6.0f,  10.0f,
          5.0f, -10.0f,
          5.0f,  10.0f,
          4.0f, -10.0f,
          4.0f,  10.0f,
          3.0f, -10.0f,
          3.0f,  10.0f,
          2.0f, -10.0f,
          2.0f,  10.0f,
          1.0f, -10.0f,
          1.0f,  10.0f,
          0.0f, -10.0f,
          0.0f,  10.0f,

         -10.0f, -10.0f,
         -10.0f,  10.0f,
         -9.0f, -10.0f,
         -9.0f,  10.0f,
         -8.0f, -10.0f,
         -8.0f,  10.0f,
         -7.0f, -10.0f,
         -7.0f,  10.0f,
         -6.0f, -10.0f,
         -6.0f,  10.0f,
         -5.0f, -10.0f,
         -5.0f,  10.0f,
         -4.0f, -10.0f,
         -4.0f,  10.0f,
         -3.0f, -10.0f,
         -3.0f,  10.0f,
         -2.0f, -10.0f,
         -2.0f,  10.0f,
         -1.0f, -10.0f,
         -1.0f,  10.0f,
         -0.0f, -10.0f,
         -0.0f,  10.0f,
     };

     GLuint gridIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                            23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                            43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                            63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83};

     glGenBuffers( 1, &gridVBO );
     glBindBuffer( GL_ARRAY_BUFFER, gridVBO );
     glBufferData( GL_ARRAY_BUFFER, 2 * 84 * sizeof(GLfloat), gridData, GL_STATIC_DRAW );

     glGenBuffers( 1, &gridIBO );
     glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gridIBO );
     glBufferData( GL_ELEMENT_ARRAY_BUFFER, 84 * sizeof(GLuint), gridIndex, GL_STATIC_DRAW );
 }

 void editorWidgetCl::_updateGl()
 {
    if(!shiftPressed)
    {
        if(upPressed)
        {
            camera.translate(0.0, -cameraSpeed);
        }
        if(downPressed)
        {
            camera.translate(0.0, cameraSpeed);
        }
        if(rightPressed)
        {
            camera.translate(-cameraSpeed, 0.0);
        }
        if(leftPressed)
        {
            camera.translate(cameraSpeed, 0.0);
        }
    }
    else
    {
        if(upPressed)
        {
            camera.translate(0.0, -cameraFastSpeed);
        }
        if(downPressed)
        {
            camera.translate(0.0, cameraFastSpeed);
        }
        if(rightPressed)
        {
            camera.translate(-cameraFastSpeed, 0.0);
        }
        if(leftPressed)
        {
            camera.translate(cameraFastSpeed, 0.0);
        }
    }

     paintGL();
 }

 void editorWidgetCl::paintGL()
 {
     makeCurrent();

     //Clear color buffer
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

     if(grid)
     {
         glLineWidth( 1.0f );

         glUseProgram(shaderProgram.programId());

         glEnableVertexAttribArray(shaderProgram.attributeLocation("pos2d"));

         glBindBuffer( GL_ARRAY_BUFFER, gridVBO );
         glVertexAttribPointer( shaderProgram.attributeLocation("pos2d"),
                                2,
                                GL_FLOAT,
                                GL_FALSE,
                                2 * sizeof(GLfloat),
                                NULL );

         QMatrix4x4 nullMatrix;

         shaderProgram.setUniformValue("camera", projection * camera * mdl);
         shaderProgram.setUniformValue("anim", nullMatrix);
         shaderProgram.setUniformValue("color", QColor(200,200,200));

         glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gridIBO);
         glDrawElements( GL_LINES, 84, GL_UNSIGNED_INT, NULL );


         glDisableVertexAttribArray( shaderProgram.attributeLocation("pos2d") );

         glUseProgram(0);
     }

     if(selectObj)
     {
         for(int i = newObjects.size() - 1; i > -1; i--)
          {
             GObjClass* currentObj;

             for(int _i = newObjects.size() - 1; _i > -1; _i--)
             {
                if(newObjects.at(_i)->getShaderID() == i)
                {
                    currentObj = newObjects.at(_i);
                }
             }

              glUseProgram(shaderProgram.programId());

              glEnableVertexAttribArray(shaderProgram.attributeLocation("pos2d"));

              glBindBuffer( GL_ARRAY_BUFFER, currentObj->getVBO() );
              glVertexAttribPointer( shaderProgram.attributeLocation("pos2d"),
                                     2,
                                     GL_FLOAT,
                                     GL_FALSE,
                                     2 * sizeof(GLfloat),
                                     NULL );

              shaderProgram.setUniformValue("camera", projection * camera * mdl);
              shaderProgram.setUniformValue("anim", currentObj->gettrans() * currentObj->getrot() * currentObj->getscale());

              shaderProgram.setUniformValue("color", currentObj->getColorID());

              glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, currentObj->getIBO());
              glDrawElements( GL_TRIANGLE_FAN, currentObj->getVertexCount(), GL_UNSIGNED_INT, NULL );

              glDisableVertexAttribArray( shaderProgram.attributeLocation("pos2d") );

              glUseProgram(0);
          }

         selectObj = false;
     }
     else
     {
         for(int i = newObjects.size() - 1; i > -1; i--)
         {
             GObjClass* currentObj;

             for(int _i = newObjects.size() - 1; _i > -1; _i--)
             {
                if(newObjects.at(_i)->getShaderID() == i)
                {
                    currentObj = newObjects.at(_i);
                }
             }

             if(currentObj->getShaderType() == SHADER_COLOR)
             {
                 glUseProgram(shaderProgram.programId());

                 glEnableVertexAttribArray(shaderProgram.attributeLocation("pos2d"));
             }

             glBindBuffer( GL_ARRAY_BUFFER, currentObj->getVBO() );
             glVertexAttribPointer( shaderProgram.attributeLocation("pos2d"),
                                    2,
                                    GL_FLOAT,
                                    GL_FALSE,
                                    2 * sizeof(GLfloat),
                                    NULL );

             shaderProgram.setUniformValue("camera", projection * camera * mdl);
             shaderProgram.setUniformValue("anim", currentObj->gettrans() * currentObj->getrot() * currentObj->getscale());
             if(currentObj->getShaderType() == SHADER_COLOR)
                shaderProgram.setUniformValue("color", currentObj->getTrueColor());

             glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, currentObj->getIBO());
             glDrawElements( GL_TRIANGLE_FAN, currentObj->getVertexCount(), GL_UNSIGNED_INT, NULL );

             if(currentObj->getShaderType() == SHADER_COLOR)
             {
                 glDisableVertexAttribArray( shaderProgram.attributeLocation("pos2d") );
             }

             glUseProgram(0);

             if(currentObj->getSelected())
             {
                 glDisable(GL_DEPTH_TEST);

                 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

                 glLineWidth( 2.0f );

                 glUseProgram(shaderProgram.programId());

                 glEnableVertexAttribArray(shaderProgram.attributeLocation("pos2d"));

                 glBindBuffer( GL_ARRAY_BUFFER, currentObj->getVBO() );
                 glVertexAttribPointer( shaderProgram.attributeLocation("pos2d"),
                                        2,
                                        GL_FLOAT,
                                        GL_FALSE,
                                        2 * sizeof(GLfloat),
                                        NULL );

                 shaderProgram.setUniformValue("camera", projection * camera * mdl);
                 shaderProgram.setUniformValue("anim", currentObj->gettrans() * currentObj->getrot() * currentObj->getscale());
                 if(currentObj->getShaderType() == SHADER_COLOR)
                    shaderProgram.setUniformValue("color", QColor(255,255,255));

                 glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, currentObj->getIBO());
                 glDrawElements( GL_TRIANGLE_FAN, currentObj->getVertexCount(), GL_UNSIGNED_INT, NULL );

                 glDisableVertexAttribArray( shaderProgram.attributeLocation("pos2d") );

                 glUseProgram(0);

                 glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

                 glEnable(GL_DEPTH_TEST);
             }
         }

         this->swapBuffers();
     }
 }

 void editorWidgetCl::resizeGL(int width, int height)
 {
     winW = width;
     winH = height;
     glViewport(0,0,width,height);
     projection.setToIdentity();
     projection.perspective(60, (float)width/(float)height, 0.01f, 100.0f);
}

 void editorWidgetCl::wheelEvent(QWheelEvent * event)
 {
     scale=(float)((float)event->delta()/(float)900);
     camera.translate(0.0,0.0,scale);
 }

 QSet<int> keysPressed;
 bool m_bFirstRelease = false;

 void editorWidgetCl::keyPressEvent(QKeyEvent * event) {
     /*m_bFirstRelease = true;
     keysPressed+= event->key();*/

    if(event->key() == Qt::Key_Shift)
    {
     shiftPressed = true;
    }
    if(event->key() == Qt::Key_W)
    {
     upPressed = true;
    }
    if(event->key() == Qt::Key_S)
    {
     downPressed = true;
    }
    if(event->key() == Qt::Key_A)
    {
     leftPressed = true;
    }
    if(event->key() == Qt::Key_D)
    {
     rightPressed = true;
    }

    if(event->key() == Qt::Key_Delete)
    {
        for(int i = 0; i < newObjects.size(); i++)
        {
           if(newObjects.at(i)->getSelected())
           {
               for(int _i = 0; _i < newObjects.size(); _i++)
               {
                   if(newObjects.at(_i)->getShaderID() >= newObjects.at(i)->getShaderID())
                   {
                       newObjects[_i]->setShaderID( newObjects.at(_i)->getShaderID() - 1);
                   }
               }

               delete newObjects[i];

               newObjects.remove(i);

               _saved = false;
           }
        }
    }
 }

 void editorWidgetCl::keyReleaseEvent(QKeyEvent * event) {
     /*if(m_bFirstRelease) {
         processMultiKeys(keysPressed);
     }
     m_bFirstRelease = false;
     keysPressed -= event->key();*/

     if(event->key() == Qt::Key_Shift)
     {
      shiftPressed = false;
     }
     if(event->key() == Qt::Key_W)
     {
      upPressed = false;
     }
     if(event->key() == Qt::Key_S)
     {
      downPressed = false;
     }
     if(event->key() == Qt::Key_A)
     {
      leftPressed = false;
     }
     if(event->key() == Qt::Key_D)
     {
      rightPressed = false;
     }
 }

 editorWidgetCl::~editorWidgetCl()
 {
    for(int i = 0; i < newObjects.size(); i++)
    {
        if(newObjects.at(i)->getVBO() != 0)
        {
            //glDeleteBuffers(1, newObjects.at(i)->getVBO());
            //glDeleteBuffers(1, newObjects.at(i)->getIBO());
            newObjects.remove(i);
        }
    }
 }

 void editorWidgetCl::mousePressEvent(QMouseEvent * event)
 {
     if(event->buttons() & Qt::LeftButton)
     {
         selectObj = true;
         paintGL();

         int window_height = winH;
         int x = event->x();
         int y = event->y();

         GLbyte color[4];
         GLfloat depth;
         GLuint index;

         glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
         glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
         glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

         //caculate the true colors
         int trueColor[3];

         if(color[0] < 0)
         {
             trueColor[0] = 256 + color[0];
         }
         else
         {
             trueColor[0] = color[0];
         }

         if(color[1] < 0)
         {
             trueColor[1] = 256 + color[1];
         }
         else
         {
             trueColor[1] = color[1];
         }

         if(color[2] < 0)
         {
             trueColor[2] = 256 + color[2];
         }
         else
         {
             trueColor[2] = color[2];
         }

         int selectedA = 0;

         //select obj that was clicked on
         for(int i = 0; i < newObjects.size(); i++)
         {
             if(newObjects.at(i)->getColorID().red() == trueColor[0] &&
                newObjects.at(i)->getColorID().green() == trueColor[1] &&
                newObjects.at(i)->getColorID().blue() == trueColor[2])
             {
                 if(newObjects.at(i)->getSelected())
                 {
                     newObjects.at(i)->setSelected(false);
                 }
                 else
                 {
                     for(int _i = 0; _i < newObjects.size(); _i++)
                     {
                         newObjects.at(_i)->setSelected(false);
                     }

                     newObjects.at(i)->setSelected(true);
                     selectedA++;
                 }
             }
         }

         //deselect all if clicked on background
         if(selectedA == 0)
         {
             for(int i = 0; i < newObjects.size(); i++)
             {
                 if(newObjects.at(i)->getSelected())
                 {
                     /*GObject tempVar = objects.at(i);
                     tempVar.selected = false;
                     objects.remove(i);
                     objects.push_back(tempVar);*/

                     newObjects.at(i)->setSelected(false);

                     qDebug() << "deslected obj: " << i;
                 }
             }
         }

     }
 }
