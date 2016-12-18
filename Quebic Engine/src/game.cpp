#include "game.h"
#include "QuebicLib\window.h"
#include "QuebicLib\primitives.h"
#include "QuebicLib\texture.h"
#include "QuebicLib\QMath.h"
#include "QuebicLib\QOpenGl.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include <vector>

quebic::window::GWindow testwindow;
quebic::gl::shader testShader;
quebic::gl::poly testRenP;
//quebic::gl::asset testAsset;

GLuint vbo_colors = 0;
GLint attribute_color = -1;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

GLint uniform_mvp;

bool gameNSpace::init()
{
	//INIT HERE
	if (!quebic::gl::initGl())
	{
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	testwindow.init("test", WINPOS_UNDEFINED, WINPOS_UNDEFINED, 500, 500, true, true, false);
	quebic::primitives::setP2MScale(250, 250);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
	view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, 1.0f*testwindow.getWidth() / testwindow.getHeight(), 0.1f, 10.0f);

	string verTexStr = "";
	string frgTexStr = "";

	verTexStr += "#version 440\r\n";
	verTexStr += "in vec3 LVertexPos;uniform mat4 mvp;void main(){ gl_Position = mvp * vec4(LVertexPos.x, LVertexPos.y, LVertexPos.z, 1);}";
	
	frgTexStr += "#version 440\r\nout vec4 LFragment;void main(){ LFragment = vec4(1.0,0.0,0.0,1.0); }";

	const GLchar* vertexS[] = { verTexStr.c_str() };
	const GLchar* fragmentS[] = { frgTexStr.c_str() };

	testShader.setUpFromStr(vertexS, fragmentS, false, true);

	std::vector < GLfloat > out_vertices;
	std::vector < glm::vec2 > out_uvs;
	std::vector < glm::vec3 > out_normals;
	std::vector < GLfloat > ibo;

	quebic::gl::p3d::loadOBJ("C:/Users/alex/Desktop/gunModel.obj", out_vertices, out_uvs, out_normals, ibo);

	testRenP.setVertexes(out_vertices, ibo);

	//testAsset.loadFromFile("C:/Users/alex/Desktop/testAsset.qea");
	//testAsset.setWorldVars(model, view, projection);

	return true;
}

bool gameNSpace::handleEvents(SDL_Event e)
{
	//HANDLE EVENTS HERE
	testwindow.handleEvents(e);

	if (!testwindow.isShown())
	{
		return false;
	}

	return true;
}

bool gameNSpace::update()
{
	//UPDATE HERE
	if (!testwindow.isMinimized())
	{
		testwindow.clearRenderer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*testAsset.render(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.5, 0.0))
						* glm::rotate(glm::mat4(1.0f), (float)((float)SDL_GetTicks() / (float)1000.0 * (float)45), glm::vec3(0.0, 1.0, 0.0))
						* glm::scale(glm::mat4(), glm::vec3(1.0, 1.0, 1.0)));*/

		testRenP.startRender(&testShader);

		glm::mat4 mvp;
		mvp = projection * view * model * (glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.5, 0.0))
			* glm::rotate(glm::mat4(1.0f), (float)((float)SDL_GetTicks() / (float)1000.0 * (float)45), glm::vec3(0.0, 1.0, 0.0))
			* glm::scale(glm::mat4(), glm::vec3(1.0, 1.0, 1.0)));

		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

		testRenP.render(&testShader, GL_TRIANGLES, false, true);
		testRenP.endRender(&testShader);

		testwindow.renderContents();
	}

	return true;
}

void gameNSpace::close()
{
	testShader.free();
	testRenP.free();
	//testAsset._free();
}