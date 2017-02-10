#include "gobjclass.h"

GObjClass::GObjClass()
{
    this->preScale.x = 1;
    this->preScale.y = 1;
    this->mscale.scale(1, 1);
    this->lastRot = 0;
}

void GObjClass::setPosition(double x, double y)
{
    double oldX = this->mtrans.data()[12];
    double oldY = this->mtrans.data()[13];

    double newX = x - oldX;
    double newY = y - oldY;

    this->mtrans.translate(newX, newY);
}

void GObjClass::setRotation(double rot)
{
    this->mrot.setToIdentity();
    this->lastRot = rot;
    this->mrot.rotate(rot, 0.0, 0.0, 1.0);
}

void GObjClass::setScale(double x, double y)
{
    xScale.setToIdentity();
    xScale.scale(x, 1);
    yScale.setToIdentity();
    yScale.scale(1, y);
    this->preScale.x = x;
    this->preScale.y = y;
}

void GObjClass::setXScale(double sc)
{
    this->preScale.x = sc;
    xScale.setToIdentity();
    xScale.scale(sc, 1);
}

void GObjClass::setYScale(double sc)
{
    this->preScale.y = sc;
    yScale.setToIdentity();
    yScale.scale(1, sc);
}

 void GObjClass::translate(double x, double y)
 {
     this->mtrans.translate(x, y);
 }

 void GObjClass::rotate(double rot)
 {
     this->lastRot = rot;
     this->mrot.rotate(rot, 0.0, 0.0, 1.0);
 }

 void GObjClass::setTrueColor(QColor color)
 {
     this->trueColor = color;
 }

 void GObjClass::setColorID(QColor ID)
 {
     this->colorID = ID;
 }

 void GObjClass::setObjName(QString name)
 {
     this->objName = name;
 }

 void GObjClass::setScriptLoc(QString loc)
 {
     this->script = loc;
 }

 QColor GObjClass::getTrueColor()
 {
     return this->trueColor;
 }

 QColor GObjClass::getColorID()
 {
    return this->colorID;
 }

 doublePoint GObjClass::getPosition()
 {
     doublePoint rpont;
     rpont.x = this->mtrans.data()[12];
     rpont.y = this->mtrans.data()[13];
     return rpont;
 }

 double GObjClass::getRotation()
 {
     return this->lastRot;
 }

 doublePoint GObjClass::getScale()
 {
    return this->preScale;
 }

 QString GObjClass::getObjName()
 {
     return this->objName;
 }

 QString GObjClass::getScriptLoc()
 {
     return this->script;
 }

 void GObjClass::setVBO(GLuint _v)
 {
     this->vbo = _v;
 }

 void GObjClass::setIBO(GLuint _i)
 {
     this->ibo = _i;
 }

 GLuint GObjClass::getVBO()
 {
     return this->vbo;
 }

 GLuint GObjClass::getIBO()
 {
     return this->ibo;
 }

 void GObjClass::setVertexCount(int vc)
 {
     this->vertexCount = vc;
 }

 int GObjClass::getVertexCount()
 {
     return this->vertexCount;
 }

 void GObjClass::resetTrans()
 {
     this->mtrans.setToIdentity();
 }

 void GObjClass::resetRot()
 {
     this->mrot.setToIdentity();
 }

 void GObjClass::resetScle()
 {
     this->mscale.setToIdentity();
 }

 void GObjClass::setObjType(objectType type)
 {
     this->GObjectType = type;
 }

 objectType GObjClass::getObjType()
 {
     return this->GObjectType;
 }

 void GObjClass::setShaderID(int ID)
 {
     this->renderId = ID;
 }

 int GObjClass::getShaderID()
 {
     return this->renderId;
 }

 void GObjClass::setSelected(bool _selected)
 {
     this->selected = _selected;
 }

 void GObjClass::setSelected(int _selected)
 {
     this->selected = (_selected != 0);
 }

 void GObjClass::setShaderType(shaderType _type)
 {
     this->type = _type;
 }

 shaderType GObjClass::getShaderType()
 {
     return this->type;
 }

 QMatrix4x4 GObjClass::gettrans()
 {
     return this->mtrans;
 }

 QMatrix4x4 GObjClass::getrot()
 {
     return this->mrot;
 }

 QMatrix4x4 GObjClass::getscale()
 {
     return (this->xScale * this->yScale);
 }

bool GObjClass::getSelected()
{
    return this->selected;
}

 GObjClass* luaMod::GObjClass_new(lua_State* L)
 {
     return new GObjClass();
 }

 int luaMod::GObjClass_getRotation(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);
     lua_pushnumber(L, _gobjlass->getRotation());
     return 1;
 }

 int luaMod::GObjClass_getPosition(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);

     QVector<double> cord;
     cord.push_back(_gobjlass->getPosition().x);
     cord.push_back(_gobjlass->getPosition().y);

     lua_newtable(L);

     for(int i = 0; i < 2; i++)
     {
         lua_pushnumber(L, i);
         lua_pushnumber(L, cord.at(i));
         lua_rawset(L, -3);
     }

     return 1;
 }

 int luaMod::GObjClass_getColor(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);

     QVector<double> colorda;
     colorda.push_back(_gobjlass->getTrueColor().red());
     colorda.push_back(_gobjlass->getTrueColor().green());
     colorda.push_back(_gobjlass->getTrueColor().blue());

     lua_newtable(L);

     for(int i = 0; i < colorda.size(); i++)
     {
         lua_pushnumber(L, i);
         lua_pushnumber(L, colorda.at(i));
         lua_rawset(L, -3);
     }

     return 1;
 }


 int luaMod::GObjClass_setRotation(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);
     double rotation = luaL_checknumber(L, 2);
     _gobjlass->setRotation(rotation);
     return 0;
 }

 int luaMod::GObjClass_setPosition(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);

     double x = luaL_checknumber(L, 2);
     double y = luaL_checknumber(L, 3);

     _gobjlass->setPosition(x, y);

     return 0;
 }

 int luaMod::GObjClass_setColor(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);

     double r = luaL_checknumber(L, 2);
     double g = luaL_checknumber(L, 3);
     double b = luaL_checknumber(L, 4);

     _gobjlass->setTrueColor(QColor(r, g, b));

     return 0;
 }

 int luaMod::GObjClass_getScale(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);

     QVector<double> scales;
     scales.push_back(_gobjlass->getScale().x);
     scales.push_back(_gobjlass->getScale().y);

     for(int i = 0; i < scales.size(); i++)
     {
         lua_pushnumber(L, i);
         lua_pushnumber(L, scales.at(i));
         lua_rawset(L, -3);
     }

     return 1;
 }

 int luaMod::GObjClass_setScale(lua_State* L)
 {
     GObjClass* _gobjlass = luaW_check<GObjClass>(L, 1);

     double scaleX = luaL_checknumber(L, 2);
     double scaleY = luaL_checknumber(L, 3);

     _gobjlass->setScale(scaleX, scaleY);

     return 0;
 }

 /*ADD MORE LUA FUNCTIONS HERE*/

 static luaL_Reg GObjClass_Table[] =
 {
     { NULL, NULL }
 };

 static luaL_Reg GObjClass_metatable[] =
 {
     { "getRotation", luaMod::GObjClass_getRotation },
     { "setRotation", luaMod::GObjClass_setRotation },
     { "getPosition", luaMod::GObjClass_getPosition },
     { "setPosition", luaMod::GObjClass_setPosition },
     { "getColor", luaMod::GObjClass_getColor },
     { "setColor", luaMod::GObjClass_setColor },
     { "getScale", luaMod::GObjClass_getScale },
     { "setScale", luaMod::GObjClass_setScale },
     { NULL, NULL }
 };

 int luaMod::luaopen_GObjClass(lua_State* L)
 {
     luaW_register<GObjClass>(L, "GObject", GObjClass_Table, GObjClass_metatable, GObjClass_new);
     return 1;
 }
