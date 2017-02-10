#ifndef GOBJCLASS_H
#define GOBJCLASS_H

#include <QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QtGui>
#include <QtCore>
#include <QEvent>
#include <QPoint>
#include <QVector>
#include "luawrapper.hpp"

enum shaderType{ SHADER_COLOR = 0, SHADER_TEX = 1, SHADER_TEX_COLOR = 2 };
enum toolState{TOOL_TRANS = 0, TOOL_ROT = 1, TOOL_SCL = 2, TOOL_NON = 3};
enum objectType{OBTYPE_RECT = 0, OBTYPE_TRI = 1};

struct doublePoint
{
    double x;
    double y;
};

class GObjClass
{
public:
    GObjClass();

    void setPosition(double x, double y);
    //translates relitive to its position
    void translate(double x, double y);
    void setRotation(double rot);
    //rotates relitive to its rotation
    void rotate(double rot);
    void setScale(double x, double y);
    void setTrueColor(QColor color);
    void setColorID(QColor ID);
    void setObjName(QString name);
    void setScriptLoc(QString loc);
    void setObjType(objectType);
    void setShaderID(int);
    void setSelected(bool);
    void setSelected(int);
    void setShaderType(shaderType);

    void setXScale(double);
    void setYScale(double);

    void resetTrans();
    void resetRot();
    void resetScle();

    void setVBO(GLuint _v);
    void setIBO(GLuint _i);

    void setVertexCount(int vc);

    doublePoint getPosition();
    double getRotation();
    doublePoint getScale();
    QColor getTrueColor();
    QColor getColorID();
    QString getObjName();
    QString getScriptLoc();
    objectType getObjType();
    int getShaderID();
    bool getSelected();
    shaderType getShaderType();
    QMatrix4x4 gettrans();
    QMatrix4x4 getrot();
    QMatrix4x4 getscale();
    //shaderType getShaderType();

    GLuint getVBO();
    GLuint getIBO();

    int getVertexCount();

    void render();
private:
    GLuint vbo;
    GLuint ibo;
    shaderType type;
    QMatrix4x4 mtrans;
    QMatrix4x4 mrot;
    QMatrix4x4 mscale;
    QColor colorID;
    QColor trueColor;
    bool selected;
    int vertexCount;
    int renderId;
    QString objName;
    objectType GObjectType;
    QString script;
    bool scriptEnabled;
    doublePoint preScale;
    double lastRot;

    QMatrix4x4 xScale;
    QMatrix4x4 yScale;
};

/*lua GObjClass bindings*/

namespace luaMod
{
    GObjClass* GObjClass_new(lua_State* L);

    int GObjClass_getRotation(lua_State* L);

    int GObjClass_getPosition(lua_State* L);

    int GObjClass_getColor(lua_State* L);

    int GObjClass_getScale(lua_State* L);


    int GObjClass_setScale(lua_State* L);

    int GObjClass_setRotation(lua_State* L);

    int GObjClass_setPosition(lua_State* L);

    int GObjClass_setColor(lua_State* L);

    int luaopen_GObjClass(lua_State* L);
}

#endif // GOBJCLASS_H
