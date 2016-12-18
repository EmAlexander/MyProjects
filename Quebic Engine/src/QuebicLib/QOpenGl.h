#ifndef __QOPENGL_H_INCLUDED
#define __QOPENGL_H_INCLUDED

#include <SDL.h>
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <SOIL\SOIL.h>
#include <vector>
#include <fstream>
#include <SDL_image.h>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include "stringS.h"

using namespace std;

namespace quebic
{
	namespace gl
	{
		namespace internals
		{
			//internal function for printing the program's errors
			void printProgramLog(GLuint program);

			//internal function for printing the shader's errors
			void printShaderLog(GLuint shader);
		}

		class shader
		{
		public:
			shader();
			~shader();

			//if "is3d" is true the vertex shader will be created to support 3d vertices
			//but if false the shader will be created to support 2d vertices only
			bool setUp(bool _is3d, bool vertxColor);

			//this function allows you to create a shader from glsl source
			//If you use 3d in your shader set is3d to true
			bool setUpFromStr(const GLchar* vertexStr[], const GLchar* fragmentStr[], bool _hasVertexColor = false, bool _is3d = false);

			//frees the shader from the gpu
			void free();

			//INTERNAL FUNCTION
			//gets the "vertexlocation" that is defined in the shader
			GLint getVertexLoc();

			//INTERNAL FUNCTION
			//gets the shader id
			GLuint getSId();

			//INTERNAL FUNCTION
			//gets if 3d is enabled
			bool getIs3d();

			//INTERNAL FUNCTION
			//gets the "gAttributeColor" that is defined in the shader
			GLint getColorLoc();

			//INTERNAL FUNCTION
			bool getVertexColor();
		private:
			GLuint shaderId = 0;

			bool hasVertexColor;
			bool is3d;

			GLint gVertexPosLocation = -1, gAttributeColor = -1;
		};

		class poly
		{
		public:
			poly();
			~poly();

			//set the vertexes of the polygon
			void setVertexes(std::vector<GLfloat> vboVec, std::vector<GLfloat> iboVec);

			//start renderering
			void startRender(shader* _shader);

			//to render you MUST use startRender()
			void render(shader* _shader, GLenum renderMode, bool dontUseIbo = false, bool D3d = false);

			//stop renderering
			void endRender(shader* _shader);

			int getVboLoc();
			int getIboLoc();

			void free();
		private:
			GLuint _vbo = 0, _ibo = 0;
		};

		class QGlTexture
		{
		public:
			QGlTexture();
			~QGlTexture();

			GLuint getTextureId();

			//loads texture from a file and sets texture cords
			bool loadFromFile(string path, shader* _shader, vector<GLfloat> texcoords);

			//creates texture from a sdl surface
			//NOTE: not working needs to allow texture cords
			bool loadFromSdlSurface(SDL_Surface* suf);

			//creates texture from pixels 
			bool loadFromPx(Uint32 *pixels, shader* _shader, vector<GLfloat> texcoords, int _w, int _h);

			void bindTex();
			void unBindTex();

			void _free();

		private:
			GLuint textureId, vbo_texcoords;
			GLint uniform_texture, attribute_texcoord;
		};

		class asset
		{
		public:
			asset();
			~asset();

			bool loadFromFile(std::string filePath);

			void loadFromStr(string str);

			//for 3d only! if your asset is not 3d then this is not needed
			void setWorldVars(glm::mat4 _model, glm::mat4 _camera, glm::mat4 _projection);

			void render(glm::mat4 tranlate);

			void _free();

		private:
			bool is3d;
			bool hasIbo;
			SDL_Point texDiment;
			poly assetPolygon;
			shader assetShader;

			GLenum renderType;

			GLint uniform_mvp;
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;

			QGlTexture texture;
		};

		//init opengl for usage in windows
		bool initGl();

		namespace p3d
		{
			//loads a obj from the file specified and puts the obj's info in the correct vector
			bool loadOBJ(const char * path, std::vector < GLfloat > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals, std::vector < GLfloat > & ibo, bool hasUvs = false);
		}

		namespace p2d
		{
			//puts vertexes of the circle in the vbo vector and the order or renderering in the ibo vector
			//renderering directions:
			//	use GL_LINE_LOOP if not solid
			//	use GL_TRIANGLES if is solid
			void createCirclePolys(std::vector<GLfloat> * vboArray, std::vector<GLfloat> * iboArray, int num_segments, float cX, float cY, float raidus, bool soild = true);
			
			//puts vertexes of the torus in the vbo vector and the order of renderering in the ibo vector
			//renderering directions:
			//	use GL_TRIANGLES
			void createTorusPolys(std::vector<GLfloat> * vboArray, std::vector<GLfloat> * iboArray, int num_segments, float cX, float cY, float innerR, float outterR);
		}
	}
}

#endif//__QOPENGL_H_INCLUDED