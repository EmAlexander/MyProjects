#include "gGameObject.h"


gGameObject::gGameObject()
{
}


gGameObject::~gGameObject()
{
	gObj_txt.free();
}


SDL_Rect gGameObject::getPos()
{
	return gObj_pos;
}

void gGameObject::render(SDL_Renderer* mren, SDL_Rect g_camera, txtInfo rotationVal = { NULL, NULL, NULL, SDL_FLIP_NONE })
{
	gObj_txt.render(gObj_pos.x - g_camera.x, gObj_pos.y - g_camera.y, mren, rotationVal.clip, rotationVal.rotation, rotationVal.center, rotationVal.flip);
}

void gGameObject::setPos(int x, int y)
{
	gObj_pos = { x, y, gObj_txt.getWidth(), gObj_txt.getHeight() };
}